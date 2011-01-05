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
   RES_PNG,
   RES_TXT,
   RES_UNKNOWN
}http_res_type;

/* Reads data off of the 'client' socket and returns HTTP data. On error NULL *
 * is returned, the socket is closed, and errno is potentially set.           */
http_request * http_read_request(SOCK client);

void http_init_regex(void);

void http_serve_static(SOCK client, const char * file_name);

#endif /* http.h */

