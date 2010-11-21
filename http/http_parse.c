#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcp.h"
#include "http.h"
#include "html_prefab.h"

#define false 0

http_request * http_read_request(SOCK client) {
   int sysret;

   /* In the future this will be replaced with a fast memory pool. We have    *
    * allocated an extra 1024 bytes to hold the request data. This way the    *
    * whole thing can be free'd with a single call to free().                 */
   http_request * ret = malloc(sizeof(http_request) + 1024);
   char * http_dat, * peek, * set, * tmp_uri;

   /* malloc() failed, bail out (and I mean BAIL OUT!) */
   if (ret == NULL) {
      goto no_mem;
   }

   /* Set http_dat and peek to point at the 1024 bytes of memory after ret    *
    * and zero out all of the memory to avoid bad stringops.                  */
   memset((http_dat = peek = (char *)(ret + 1)), 0, 1024);

   if ((sysret = recv(client, http_dat, 1023, 0)) == -1) {
      /* Error logging should be placed here. */
      goto failure;
   } else if (sysret == 0) {
      /* Client closed immediately after connecting, drop everything. */
      goto failure;
   }

   /* Isolate the METHOD token and figure out what it is. */
   *(set = strchr(peek, ' ')) = '\0';
   if (strcmp(peek, "GET") == 0) {
      ret->type = HTTP_GET;
   } else if (strcmp(peek, "HEAD") == 0) {
      ret->type = HTTP_HEAD;
   } else if (strcmp(peek, "POST") == 0) {
      ret->type = HTTP_POST;
   } else {
      /* This request type is not implemented. */
      send(client, html_501_Page, sizeof html_501_Page, MSG_NOSIGNAL);
      goto failure;
   }

   tmp_uri = set + 1;
   *(set = strchr(tmp_uri, ' ')) = '\0';

   *(set = strstr(set + 1, "\r\n")) = '\0';

   peek = set + 2;
   *(set = strstr(peek, "\r\n\r\n")) = '\0';


   
   /* We'll work some regex magic on this later, for now we just assume       *
    * blindly that the URI is formatted correctly.   TODO: ADD REGEX SUPPORT  */
   switch (ret->type) {
      case HTTP_GET:
	 ret->get.uri   = tmp_uri;
	 ret->get.head  = peek;
	 break;
      case HTTP_HEAD:
	 ret->head.uri  = tmp_uri;
	 ret->head.head = peek;
	 break;
      case HTTP_POST:
	 ret->post.uri  = tmp_uri;
	 ret->post.head = peek;
	 ret->post.data = set + 4;
	 break;
   }

   return ret;

   /* These are error conditions, I keep them all in one spot so I can keep   *
    * things organized and remember what needs to be done on failure.         */
  failure:
   free(ret);
  no_mem:
   close(client);
   return NULL;
}
