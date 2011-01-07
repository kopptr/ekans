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
#include "cfg.h"

system_configs sys_cfg;

static int dec_str(const char * str) {
   int ret = 0;
   for (; *str; ++str) {
      if (*str >= '0' && *str <= '9') {
	 ret *= 10;
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

static int validate_decimal(const char * str) {
   for (; *str; str++) {
      if (*str < '0' || *str > '9') return 0;
   }
   return 1;
}

/* This procedure is allowed to kill the program. It will print a useage      *
 * statement if it does.                                                      */
void parse_args(int argc, char * argv[]) {
   int i;

   enum arg_type {N_THREADS, PORT, PENDING } next_arg = PENDING;

   /* Port defaults to 80, flag can switch it. */
   sys_cfg.port = "80";
   /* Number of threads defaults to 15. */
   sys_cfg.n_threads = 15;
   for (i = 1; i < argc; ++i) {
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
	       if (validate_decimal(argv[i])) {
		  sys_cfg.n_threads = dec_str(argv[i]);
	       } else {
		  fprintf(stderr, "number of threads must be a positive decimal"
			  " integer\n");
		  exit(1);
	       }
	       next_arg = PENDING;
	       break;
	    case PORT:
	       if (validate_decimal(argv[i]) && dec_str(argv[i]) < 1 << 16) {
		  sys_cfg.port = argv[i]; 
	       } else {
		  fprintf(stderr, "port number must be a decimal number between"
			  " 0 and 65535\n");
	       }
	       next_arg = PENDING;
	       break;
	    default:
	       break;
	 }
      }
   }
}
