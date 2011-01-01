#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cfg.h"

system_configs sys_cfg;

static int dec_str(const char * str) {
   int ret = 0;
   for (; *str; ++str) {
      if (*str >= '0' && *str <= '9') {
	 ret += *str - '0';
      }
   }
   return ret;
}

static void print_usage(const char * name) {
   fprintf(stderr,
	   "\n   usage: %s [options]\n\n"
	   "\n     -p [#]   --port     Tells the server to bind on port [#].\n"
	   "\n     -t [#]   --threads  Tells the server to use [#] threads \n"
	   "                           (handle [#] requests simultaneously.\n"
	   "\n     -h       --help     Display this message.", name);
}

/* This procedure is allowed to kill the program. It will print a useage      *
 * statement if it does.                                                      */
void parse_args(int argc, const char * argv[]) {
   int i;
   enum {
      N_THREADS,
      PORT,
      PENDING
   }next_arg = PENDING;
   /* Port defaults to 80, flag can switch it. */
   sys_cfg.port = "80";
   for (i = 0; i < argc; ++i) {
      if (next_arg == PENDING) {
	 if (!strcmp(argv[i], "-t") || !strcmp(argv[i], "--threads")) {
	    next_arg = N_THREADS;
	 } else if (!strcmp(argv[i], "-p") || !strcmp(argv[i], "--port")) {
	    next_arg = PORT;
	 } else if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
	    print_usage(*argv);
	    exit(1);
	 } else {
	    fprintf(stderr,
		    "Unrecognized command line argument '%s'.\n", argv[i]);
	    print_usage(*argv);
	    exit(1);
	 }
      } else {
	 switch (next_arg) {
	    case N_THREADS:
	       sys_cfg.n_threads = dec_str(argv[i]);
	       next_arg = PENDING;
	       break;
	    case PORT:
	       sys_cfg.port = argv[i];
	       next_arg = PENDING;
	       break;
	    default:
	       break;
	 }
      }
   }
}
