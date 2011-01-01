#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "cfg.h"
#include "rwq.h"

static pthread_t * agents;

static void * agent_main(void * v_args) {
   fprintf(stderr, "In thread %d\n", (int)v_args);
   pthread_exit(0);
}
/* This function is allowed to kill the program, and will print a report      *
 * to stderr.                                                                 */
void start_agents(void) {
   int i;
   agents = malloc(sys_cfg.n_threads * sizeof(pthread_t));
   for (i = 0; i < sys_cfg.n_threads; ++i) {
      pthread_create(agents + i, NULL, agent_main, (void *)i);
   }
   fprintf(stderr, "%s done.\n", __FUNCTION__);
}

