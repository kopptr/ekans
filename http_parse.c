/******************************************************************************
 * This file is part of The Ekans Webserver.                                  *
 * Copyright (C) 2011 Samuel C. Payson                                        *
 *                                                                            *
 * Ekans is free software: you can redistribute it and/or modify it under the *
 * terms of the GNU General Public License as published by the Free Software  *
 * Foundation, either version 3 of the License, or (at your option) any later *
 * version.                                                                   *
 *                                                                            *
 * Ekans is distributed in the hope that it will be useful, but WITHOUT ANY   *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS  *
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more     *
 * details.                                                                   *
 *                                                                            *
 * You should have received a copy of the GNU General Public License along    *
 * with Ekans. If not, see <http://www.gnu.org/licenses/>.                    *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "tcp.h"
#include "http.h"
#include "html_prefab.h"

#define false 0

static const char * http_txt_ext[] = {"c", "h", "txt", "cpp", "cc", "hh", "hpp",
				      "java", "scm"};

/* This file makes heavy use of the POSIX regular expressions library */
static regex_t http_req_regex, http_uri_regex;

static char * get_regerror(int errcode, regex_t * compiled) {
   size_t length = regerror(errcode, compiled, NULL, 0);
   char * buffer = malloc(length);
   (void)regerror(errcode, compiled, buffer, length);
   return buffer;
}

/* This function initializes the regular expressions we use for validating    *
 * HTTP requests. It must be called before any requests are served.           */
void http_init_regex(void) {
   int r;
   char * error;
   r = regcomp(&http_req_regex,
               "^(GET|HEAD|POST) ([^ ]+)( HTTP/1\\.[10])?",
               REG_EXTENDED);
   if (r != 0) {
      error = get_regerror(r, &http_req_regex);
      fprintf(stderr, "http request regex: %s\n", error);
      free(error);
      exit(1);
   }
   r = regcomp(&http_uri_regex,
               "^/?((([a-zA-Z0-9.\\-_+]|%[a-fA-F0-9][a-fA-F0-9])+/)"
               "*(([a-zA-Z0-9.\\-_+]|%[a-fA-F0-9][a-fA-F0-9])+\\."
               "([a-zA-Z0-9]+)))?",
               REG_EXTENDED);
   if (r != 0) {
      error = get_regerror(r, &http_uri_regex);
      fprintf(stderr, "http URI regex: %s\n", error);
      free(error);
      exit(1);
   }
}

/* Figure out what kind of resource was requested. */

static http_res_type get_res_type(char * ext) {
   int i;
   if (strcmp("html", ext) == 0) {
      return RES_HTML;
   } else if (strcmp("hpy", ext) == 0) {
      return RES_HPYTHON;
   } else if (strcmp("py",  ext) == 0) {
      return RES_PYTHON;
   } else if (strcmp("css", ext) == 0) {
      return RES_CSS;
   } else if (strcmp("js",  ext) == 0) {
      return RES_JAVASCRIPT;
   } else if (strcmp("jpg", ext) == 0) {
      return RES_JPEG;
   } else if (strcmp("png", ext) == 0) {
      return RES_PNG;
   } else {
      for (i = 0; i < sizeof http_txt_ext / sizeof(char *); ++i) {
	 if (strcmp(http_txt_ext[i], ext) == 0) {
	    return RES_TXT;
	 }
      }
      return RES_UNKNOWN;
   }
}

static void unescape_hex(char * str) {
   char c, * look_ahead;
   for (look_ahead = str; *look_ahead; look_ahead++, str++) {
      for (;look_ahead[0] == '.' && look_ahead[1] == '.'; look_ahead++);
      if (*look_ahead == '%') {
         if (*(++look_ahead) >= '0' && *look_ahead <= '9') {
            c = *look_ahead - '0';
         } else if (*look_ahead >= 'a' && *look_ahead <= 'f') {
            c = 10 + *look_ahead - 'a';
         } else if (*look_ahead >= 'A' && *look_ahead <= 'F') {
            c = 10 + *look_ahead - 'A';
         } else {
	    /* I'm not sure what this is, maybe we should just try to recover *
	     * from the error instead of killing the server?                  */
            fprintf(stderr, "PANIC: %s: %s: line %d\n",
                    __FILE__, __FUNCTION__, __LINE__);
            exit(1);
         }
         c <<= 4;
         if (*(++look_ahead) >= '0' && *look_ahead <= '9') {
            c += *look_ahead - '0';
         } else if (*look_ahead >= 'a' && *look_ahead <= 'f') {
            c += 10 + *look_ahead - 'a';
         } else if (*look_ahead >= 'A' && *look_ahead <= 'F') {
            c += 10 + *look_ahead - 'A';
         } else {
	    /* see above. */
            fprintf(stderr, "PANIC: %s: %s: line %d\n",
                    __FILE__, __FUNCTION__, __LINE__);
            exit(1);
         }
         *str = c;
      } else {
         *str = *look_ahead;
      }
   }
   *str = '\0';
}

http_request * http_read_request(SOCK client) {
   int sysret, tmp_res_type;
   http_request * ret;
   char * http_dat, * tmp_uri, ** tmp_head;
   regmatch_t matches[16]; /* We shouldn't need more than this. */

   /* In the future this will be replaced with a fast memory pool. We have    *
    * allocated an extra 1024 bytes to hold the request data. This way the    *
    * whole thing can be free'd with a single call to free().                 */
   ret = malloc(sizeof(http_request) + 1024 + (12 * sizeof(char **)));

   /* malloc() failed, bail out (and I mean BAIL OUT!) */
   if (ret == NULL) {
      goto no_mem;
   }

   memset(ret, 0, sizeof(http_request) + 1024 + (12 * sizeof(char **)));

   http_dat = (char *)(ret + 1);

   tmp_head = (char **)(http_dat + 1024);

   if ((sysret = recv(client, http_dat, 1023, 0)) == -1) {
      /* Error logging should be placed here. */
      goto failure;
   } else if (sysret == 0) {
      /* Client closed immediately after connecting, drop everything. */
      goto failure;
   }

   /* This regex is used to validate the http request in a general sense and  *
    * make sure that it vaguely fits the bill. This regex is also used to get *
    * the HTTP request method.                                                */
   if ((sysret = regexec(&http_req_regex, http_dat, 3, matches, 0)) != 0) {
      goto not_implemented;
   }

   http_dat[matches[1].rm_eo] = '\0';

   if (strcmp(http_dat, "GET") == 0) {
      ret->type = HTTP_GET;
   } else if (strcmp(http_dat, "POST") == 0) {
      ret->type = HTTP_POST;
   } else if (strcmp(http_dat, "HEAD") == 0) {
      ret->type = HTTP_HEAD;
   } else {
      fprintf(stderr, "Regex not behaving as expected. This error should never "
              "occur\n");
      exit(1);
   }

   http_dat += matches[1].rm_eo + 1;

   /* Here we are actually validating the URI stringently, we make sure that   *
    * it follows a subset of the HTTP/1.0 spec., as well as retrieving the     *
    * file extension.                                                          */

   if ((sysret = regexec(&http_uri_regex, http_dat, 7, matches, 0)) != 0) {
      send(client, html_404_page, html_404_length, MSG_NOSIGNAL);
      goto failure;
   }

   http_dat[matches[0].rm_eo] = '\0';
   tmp_uri = http_dat + (http_dat[0] == '/' ? 1 : 0);

   tmp_res_type = get_res_type(http_dat + matches[6].rm_so);



   /* Resolves hex escape sequences, for example, %20 -> ' ' */
   unescape_hex(tmp_uri);

   switch (ret->type) {
      case HTTP_GET:
         ret->get.uri       = tmp_uri;
         ret->get.res_type  = tmp_res_type;
	 ret->get.head      = tmp_head;
      case HTTP_HEAD:
         ret->head.uri      = tmp_uri;
         ret->head.res_type = tmp_res_type;
	 ret->head.head     = tmp_head;
      case HTTP_POST:
         ret->post.uri      = tmp_uri;
         ret->post.res_type = tmp_res_type;
	 ret->post.head     = tmp_head;
   }

   return ret;

  not_implemented:
   send(client, html_501_page, html_501_length, MSG_NOSIGNAL);
  failure:
   free(ret);
  no_mem:
   close(client);
   return NULL;
}
