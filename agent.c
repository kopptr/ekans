#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "http.h"
#include "cfg.h"
#include "rwq.h"

static pthread_t * agents;

static void * agent_main(void * nil) {
   SOCK client;
   http_request * req;
   for (;;) {
      client = rwq_get();
      req = http_read_request(client);
      http_serve_static(client, req->get.uri);
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
