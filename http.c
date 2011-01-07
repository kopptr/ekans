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

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "http.h"
#include "html_prefab.h"
#include "tcp.h"

static const char http_ok[] = "HTTP/1.0 200 OK\r\n";

void http_serve_static(SOCK client, char ** hdr, const char * file_name) {
   char buffer[512];
   int  fd, rv;
   /* We use MSG_NOSIGNAL because we don't want SIGPIPE crashing the whole    *
    * server.                                                                 */
   if ((fd = open(file_name, O_RDONLY)) == -1) {
      send(client, html_404_page, html_404_length, MSG_NOSIGNAL);
   } else {
      send(client, http_ok, sizeof http_ok - 1, MSG_NOSIGNAL);
      for (; *hdr != NULL; ++hdr) {
	 send(client, *hdr, strlen(*hdr), MSG_NOSIGNAL);
      }
      send(client, "\r\n", 2, MSG_NOSIGNAL);
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

char * http_content_type_hdr(http_res_type res_type) {
   switch (res_type) {
      case RES_HTML:
      case RES_HPYTHON:
      case RES_PYTHON:
	 return "Content-Type: text/html\r\n";
      case RES_CSS:
	 return "Content-Type: text/css\r\n";
      case RES_JAVASCRIPT:
	 return "Content-Type: text/javascript\r\n";
      case RES_JPEG:
	 return "Content-Type: image/jpeg\r\n";
      case RES_GIF:
	 return "Content-Type: image/gif\r\n";
      case RES_PNG:
	 return "Content-Type: image/png\r\n";
      case RES_TXT:
	 return "Content-Type: text/plain\r\n";
      case RES_UNKNOWN:
      default:
	 return "Content-Type: application/octet-stream\r\n";
   }
}
