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

/* http.h -- Declares methods for parsing and dealing with http data, as well *
 * as data structures used to pass around information about http data.        */

#ifndef _EKANS_HTTP_H_
#define _EKANS_HTTP_H_

#include <stdint.h>

#include "tcp.h"

/* Codes to indicate HTTP request types. */
typedef enum {
   HTTP_GET,
   HTTP_POST,
   HTTP_HEAD
}http_req_type;

/* This union contains either a HTTP 1.0 POST, GET, or HEAD request. The      *
 * 'type' field determines which of these it actually is.                     */
typedef union {
   uint8_t type;
   struct {
      uint8_t      type;
      uint8_t      res_type; /* Is the resource a .py file?    */
      long         length;   /* How many bytes is this?        */
      const char * uri;      /* Relative path to the resource. */
      char ** head;          /* Array of HTTP Header Strings.  */
      const char * data;     /* Actual HTTP POST data.         */
   }post;
   struct {
      uint8_t type;
      uint8_t res_type;      /* What kind of resource is it?   */
      const char * uri;      /* Relative path to the resource. */
      char ** head;          /* Array of HTTP Header Strings.  */
   }get, head;
}http_request;

typedef enum {
   RES_HTML,
   RES_HPYTHON,
   RES_PYTHON,
   RES_CSS,
   RES_JAVASCRIPT,
   RES_JPEG,
   RES_GIF,
   RES_PNG,
   RES_TXT,
   RES_UNKNOWN
}http_res_type;

/* Reads data off of the 'client' socket and returns HTTP data. On error NULL *
 * is returned, the socket is closed, and errno is potentially set.           */
http_request * http_read_request(SOCK client);

void http_init_regex(void);

void http_serve_static(SOCK client, char ** hdr, const char * file_name);

char * http_content_type_hdr(http_res_type res_type);

#endif /* http.h */

