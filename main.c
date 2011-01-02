#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "http.h"
#include "rwq.h"

void p_err(const char * str) {
   fprintf(stderr, "%s: errno %d: %s\n", str, errno, strerror(errno));
}

int main(int argc, char * argv[]) {
   rwq_init();
   http_init_regex();
   
}
