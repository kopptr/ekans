#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "http.h"

void p_err(const char * str) {
   fprintf(stderr, "%s: errno %d: %s\n", str, errno, strerror(errno));
}

int main(int argc, char * argv[]) {
   http_request * hrq;
   SOCK sockfd, client;  // listen on sock_fd, new connection on new_fd
   struct addrinfo hints, * servinfo, * p;
   int yes=1;
   int rv;

   http_init_regex();

   if (argc != 2) {
      fprintf(stderr, "usage: %s [portNbr]\n", *argv);
      exit(1);
   }

   memset(&hints, 0, sizeof hints);
   hints.ai_family = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_PASSIVE; // use my IP

   if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
      return 1;
   }

   // loop through all the results and bind to the first we can
   for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
			   p->ai_protocol)) == -1) {
	 p_err("server: socket");
	 continue;
      }

      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
		     sizeof(int)) == -1) {
	 p_err("setsockopt");
	 exit(1);
      }

      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	 close(sockfd);
	 p_err("server: bind");
	 continue;
      }

      break;
   }



   if (p == NULL)  {
      p_err("server: failed to bind");
      return 2;
   }

   freeaddrinfo(servinfo); // all done with this structure

   if (listen(sockfd, 10) == -1) {
      p_err("listen");
      exit(1);
   }

   while (1) {
      client = accept(sockfd, NULL, NULL);
      hrq = http_read_request(client);
      if (hrq != NULL) {
	 switch (hrq->type) {
	    case HTTP_GET:
	       printf("GET:\nURI: %s\nHeaders: %s\n", hrq->get.uri, (char *)(hrq->get.head ? hrq->get.head : "no headers"));
	       break;
	    case HTTP_HEAD:
	       printf("HEAD:\nURI: %s\nHeaders: %s\n", hrq->head.uri, (char *)(hrq->head.head ? hrq->head.head : "no headers"));
	       break;
	 }
	 free(hrq);
	 close(client);
      }
   }
   return 0;
}

















