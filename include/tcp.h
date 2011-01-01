#ifndef _EKANS_TCP_H_
#define _EKANS_TCP_H_

#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>

typedef int SOCK;

SOCK get_listener(const char * );

#endif /* tcp.h */
