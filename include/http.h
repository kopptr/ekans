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
      const char * head;     /* Text containing HTTP Headers.  */
      const char * data;     /* Actual HTTP POST data.         */
   }post;
   struct {
      uint8_t type;
      const char * uri;      /* Relative path to the resource. */
      const char * head;     /* Text containing HTTP Headers.  */
   }get, head;
}http_request;

/* Reads data off of the 'client' socket and returns HTTP data. On error NULL *
 * is returned, the socket closed, and errno is potentially set.              */
http_request * http_read_request(SOCK client);

#endif /* http.h */
