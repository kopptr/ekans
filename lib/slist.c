#include <stdlib.h>
#include "slist.h"

void slist_free(slist * top) {
   while (top != NULL) {
      slist * tmp = top;
      top = top->s_next;
      free(tmp);
   }
}

