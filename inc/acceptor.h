#ifndef __ACCEPTOR_H_
#define __ACCEPTOR_H_

#include <netinet/in.h>

typedef struct sockaddr_in ipaddr_t;
typedef int (*acceptor_handler_t)(acceptor_t*);

typedef struct acceptor_s acceptor_t;
struct acceptor_s {
	int 				fd;
	int 				backlog;
	ipaddr_t 			local;
	acceptor_handler_t 	accept;
};

acceptor_t*
acceptor_new(const char* host, unsigned short port, int backlog);

int
acceptor_listen();

int
acceptor_accept(acceptor_t* a);

#endif
