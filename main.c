#include <stdio.h>
#include "http.h"
#include "dispatch.h"
#include "rwq.h"
#include "cfg.h"

int main(int argc, char * argv[]) {
   parse_args(argc, argv);
   printf("Ekans webserver starting up on port %s\n", sys_cfg.port);
   rwq_init();
   http_init_regex();
   run_dispatch();
   return 0;
}
