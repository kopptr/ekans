/* slist.h -- singly linked lists */

#ifndef _EKANS_SLIST_H_
#define _EKANS_SLIST_H_

/* An ekans singly linked list is just a node. Operations are simple enough *
 * that the programmer can carry them out on his own. 'slist_free' is there *
 * for the programmer's convenience.                                        */
typedef struct slist_s {
   struct slist_s * s_next;
}slist;

/* Frees the list of which 'top' is the head. */
void slist_free(slist * top);

#endif /* slist.h */
