#include <stdlib.h>
#include "rwq.h"

rwq * client_queue = NULL;

void rwq_init(void) {
   client_queue = malloc(sizeof rwq);
   client_queue->top = client_queue->end = NULL;
   pthread_mutex_init(&client_queue->mtx,  NULL);
   pthread_cont_init( &client_queue->cnd,  NULL);
   client_queue->num_clients = 0;
}

void rwq_add(SOCK client) {
   
}

SOCK rwq_get(void) {
   
}
