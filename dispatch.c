#include "init.h"
#include "agent.h"
#include "rwq.h"
#include "tcp.h"

void run_dispatch(void) {
   SOCK listener, client;
   init_phase_1();
   start_agents();
   init_phase_2();
   listener = get_listener(sys_cfg.port);
   for (;;) {
      client = accept(listener, NULL, NULL);
      rwq_add(client);
   }
}
