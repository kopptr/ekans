#include "tcp.h"

/* This code is largely based off of examples from "Beej's Guide to Network   *
 * Programming."                                                              */

SOCK get_listener(void) {
   SOCK sockfd;
   struct addrinfo hints, * servinfo, * p;
   int yes = 1;
   int rv;

   memset(&hints, 0, sizeof hints);
   hints.ai_family   = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags    = AI_PASSIVE;

   if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
      return -1;
   }

   for(p = servinfo; p != NULL; p = p->ai_next) {
      if ((sockfd = socket(p->ai_family, p->ai_socktype,
			   p->ai_protocol)) == -1) {
	 continue;
      }

      if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
		     sizeof(int)) == -1) {
	 return -1;
      }

      if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
	 close(sockfd);
	 continue;
      }

      break;
   }

   if (p == NULL) {
      return -1;
   }

   freeaddrinfo(servinfo);

   if (listen(sockfd, 10) == -1) {
      return -1;
   }

   return sockfd;
}


