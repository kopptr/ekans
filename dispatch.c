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
#include <stdlib.h>
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
