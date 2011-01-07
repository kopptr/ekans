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

#ifndef _EKANS_RWQ_H_
#define _EKANS_RWQ_H_

#include <pthread.h>

#include "tcp.h"

/* Since there is only one Reader-Writers Queue in the whole program, we      *
 * don't need to make it general at all.                                      */

typedef struct rwq_node {
   struct rwq_node * next;
   SOCK client;
}rwq_node;

typedef struct rwq {
   rwq_node * top, * end;
   pthread_mutex_t mtx;
   pthread_cond_t  cnd;
   int nbr_clients;
}rwq;

void rwq_init(void);

void rwq_add(SOCK client);

SOCK rwq_get(void);

extern rwq * client_queue;

#endif /* rwq.h */

