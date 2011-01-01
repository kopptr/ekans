#ifndef _EKANS_RWQ_H_
#define _EKANS_RWQ_H_

#include <pthread.h>

#include "tcp.h"

/* Since there is only one Reader-Writers Queue in the whole program, we      *
 * don't need to make it general at all.                                      */

typedef struct rwq_node {
   struct rwq_node * fLink;
   SOCK client;
}rwq_node;

typedef struct rwq {
   rwq_node * top, * end;
   pthread_mutex_t mtx;
   pthread_cond_t  cnd;
   int num_clients;
}rwq;

void rwq_init(void);

void rwq_add(SOCK client);

SOCK rwq_get(void);

extern rwq * client_queue;

#endif /* rwq.h */

