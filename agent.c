/******************************************************************************
 * This file is part of The Ekans Webserver.                                  *
 * Copyright (C) 2010 Samuel C. Payson                                        *
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
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "http.h"
#include "cfg.h"
#include "rwq.h"

static pthread_t * agents;

static void * agent_main(void * nil) {
   SOCK           client;
   http_request * req;
   char         * hdr[16];
   memset(hdr, 0, sizeof hdr);
   hdr[0] = "Server: ekans\r\n";
   for (;;) {
      client = rwq_get();
      req = http_read_request(client);
      hdr[1] = http_content_type_hdr(req->get.res_type);
      http_serve_static(client, hdr, req->get.uri);
   }
   pthread_exit(0);
}

/* This function is allowed to kill the program, and will print a report to   *
 * stderr.                                                                    */
void start_agents(void) {
   int i, rv;
   agents = malloc(sys_cfg.n_threads * sizeof(pthread_t));
   for (i = 0; i < sys_cfg.n_threads; ++i) {
      if ((rv = pthread_create(agents + i, NULL, agent_main, NULL)) != 0) {
	 fprintf(stderr, "error: pthread_create: %s in %s:%d: %s\n",
		 __FUNCTION__, __FILE__, __LINE__, strerror(rv));
      }
   }
}

/* This gets called when the agents are done. It kills them all and frees all *
 * of the resources allocated by start_agents().                              */
void reap_agents(void) {
   /* TO DO */
}
