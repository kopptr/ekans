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

#include <stdlib.h>
#include "rwq.h"

rwq * client_queue = NULL;

void rwq_init(void) {
   client_queue = malloc(sizeof(rwq));
   client_queue->top = client_queue->end = NULL;
   pthread_mutex_init(&client_queue->mtx,  NULL);
   pthread_cond_init( &client_queue->cnd,  NULL);
   client_queue->nbr_clients = 0;
}


/* This is the 'writer' aspect of the reader-writers queue. It places a new   *
 * client on the queue.                                                       */
void rwq_add(SOCK client) {
   rwq_node * n_client = malloc(sizeof(rwq_node));
   n_client->client = client;
   n_client->next = NULL;
   /* Lock the mutex, then from here on out it's the normal queue algorithm.  */
   pthread_mutex_lock(&client_queue->mtx);
   if (client_queue->top == NULL) {
      client_queue->top = client_queue->end = n_client;
   } else {
      client_queue->end->next = n_client;
      client_queue->end       = n_client;
   }
   if (client_queue->nbr_clients == 0) {
      /* If the queue was empty, we send a signal on the condition variable   *
       * to wake up anyone who was waiting for the queue to be non-empty.     */
      pthread_cond_signal(&client_queue->cnd);
   }
   client_queue->nbr_clients++;
   /* Unlock the mutex, let the next waiter come onboard. */
   pthread_mutex_unlock(&client_queue->mtx);
}

/* This is the 'readers' aspect of the reader-writers queue. It gets the next *
 * client off of the queue.                                                   */
SOCK rwq_get(void) {
   SOCK ret;
   rwq_node * r_tmp;
   pthread_mutex_lock(&client_queue->mtx);
   while (client_queue->nbr_clients == 0) {
      pthread_cond_wait(&client_queue->cnd, &client_queue->mtx);
      pthread_cond_signal(&client_queue->cnd);
   }
   /* If we ever get a segfault for this pointer being NULL, rethink locking  *
    * algorithm x_X. I'm pretty confident this will never happen, and it's    *
    * not worth checking for since it will only happen if the locking is bad. */
   r_tmp = client_queue->top;
   client_queue->top = client_queue->top->next;
   client_queue->nbr_clients--;
   pthread_mutex_unlock(&client_queue->mtx);
   ret = r_tmp->client;
   free(r_tmp);
   return ret;
}
