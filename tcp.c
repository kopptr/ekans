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

#include <string.h>

#include "tcp.h"

/* This code is largely based off of examples from "Beej's Guide to Network   *
 * Programming."                                                              */

SOCK get_listener(const char * port) {
   SOCK sockfd;
   struct addrinfo hints, * servinfo, * p;
   int yes = 1;
   int rv;

   memset(&hints, 0, sizeof hints);
   hints.ai_family   = AF_UNSPEC;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags    = AI_PASSIVE;

   if ((rv = getaddrinfo(NULL, port, &hints, &servinfo)) != 0) {
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


