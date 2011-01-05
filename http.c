#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "html_prefab.h"
#include "tcp.h"

static const char http_ok[] = "HTTP/1.0 200 \r\n\r\n";

void http_serve_static(SOCK client, const char * file_name) {
   char buffer[512];
   int fd, rv;
   /* We use MSG_NOSIGNAL because we don't want SIGPIPE crashing the whole *
    * server.                                                              */
   if ((fd = open(file_name, O_RDONLY)) == -1) {
      fprintf(stderr, "Failed to open '%s': error: %s\n", file_name,
	      strerror(errno));
      send(client, html_404_page, html_404_length, MSG_NOSIGNAL);
   } else {
      send(client, http_ok, sizeof http_ok, MSG_NOSIGNAL);
      while ((rv = read(fd, buffer, 512)) > 0) {
	 send(client, buffer, rv, MSG_NOSIGNAL);
      }
      if (rv == -1) {
	 fprintf(stderr, "Failed to read from '%s': error: %s\n", file_name,
		 strerror(errno));
      }
      if (close(fd) == -1) {
	 fprintf(stderr, "Failed to close '%s': error: %s\n", file_name,
		 strerror(errno));
      }
   }
   if (close(client) == -1) {
	 fprintf(stderr, "Failed to close client socket: error: %s\n",
		 strerror(errno));
   }
}
