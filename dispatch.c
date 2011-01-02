#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "init.h"
#include "agent.h"
#include "cfg.h"
#include "rwq.h"
#include "tcp.h"

void run_dispatch(void) {
   SOCK listener, client;
   init_phase_1();
   start_agents();
   init_phase_2();
   if ((listener = get_listener(sys_cfg.port)) == -1) {
      fprintf(stderr, "Error listening on port '%s': %s.\n"
	      "Ekans aborting.\n", sys_cfg.port, strerror(errno));
      exit(1);
   }
   for (;;) {
      if ((client = accept(listener, NULL, NULL)) == -1) {
	 fprintf(stderr, "Error accepting client: %s.\n", strerror(errno));
      } else {
	 rwq_add(client);
      }
   }
}
