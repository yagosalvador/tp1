#ifndef __COMMON_SOCKET_H__
#define __COMMON_SOCKET_H__

#define _POSIX_C_SOURCE 200112L

#include <stdio.h>

#include "common_status.h"

#define MAX_LISTENING_CONNECTIONS 10 
/* deberia ser parte de socket o config del sv???? */

typedef struct {
	int fd;
} socket_t;

/*Handles the initializing of sockets*/
status_t socket_init(socket_t * self, int domain, 
					 int type, int protocol);

/*Handles the destruction of the socket */
status_t socket_destroy(socket_t * self);

/*Takes care of binding and listening*/
status_t socket_bind_and_listen(socket_t * self, const char * port);

status_t socket_accept(socket_t * accepter, socket_t * peer);

status_t socket_connect(socket_t * self, const char * host, 
						const char * port);

status_t socket_shutdown(socket_t * self, size_t mode);

status_t socket_send(socket_t * self, char * buff, size_t len);

status_t socket_recv(socket_t * self, char * buff, 
					 size_t buff_size, size_t * buff_len);

//ANALIZAR SI LA CONFIG DEL CLIENTE Y EL SV HAY QUE PASARLA POR ARGUMENTOS

#endif
