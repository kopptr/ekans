#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "tcp.h"

void http_serve_static(SOCK client, const char * file_name) {
   char buffer[512];
   int fd, rv;
   if ((fd = open(file_name, O_RDONLY)) == -1) {
      fprintf(stderr, "Failed to open '%s': error: %s\n", file_name,
	      strerror(errno));
   } else {
      while ((rv = read(fd, buff, 512)) > 0) {
	 send(client, buff, rv, MSG_NOSIGNAL);
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
		 file_name, strerror(errno));
   }
}
