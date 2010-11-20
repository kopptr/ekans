/* http.h -- Declares methods for parsing and dealing with http data, as well *
 * as data structures used to pass around information about http data.        */

#ifndef _EKANS_HTTP_H_
#define _EKANS_HTTP_H_

#include <stdint.h>

typedef int SOCK;

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
      const char * headers;  /* Text containing HTTP Headers.  */
      const char * data;     /* Actual HTTP POST data.         */
   }post;
   struct {
      uint8_t type;
      const char * uri;      /* Relative path to the resource. */
      const char * headers;  /* Text containing HTTP Headers.  */
   }get, head;
}http_request;

/* Reads data off of 'client' and returns HTTP data. */
http_request * read_http_request(SOCK client);

/* This is a prepackaged response for 404 requests. */
static const char 404_Page[] = "HTTP/1.0 404 Page Not Found\r\n"
   "\r\n"
   "<!doctype html>\n"
   "<html>\n"
   "  <body>\n"
   "    <h1>404 -- Page Not Found</h1>\n"
   "    <p>\n"
   "      The page you requested could not be located or does not exist.\n"
   "    </p>\n"
   "  </body>\n"
   "</html>\n";



#endif /* http.h */
