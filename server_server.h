#ifndef __SERVER_SERVER_H__
#define __SERVER_SERVER_H__

#include <stdio.h>

#include "common_socket.h"
#include "common_status.h"

#define SERVER_BUFF_SIZE 64

struct cypher;

typedef struct {
	socket_t accepter, listener;
} server_t;

status_t server_destroy(server_t * server);

status_t server_start(int argc, const char * argv[], 
					  const char * method, const char * key);

status_t server_connect(server_t * sever, const char * port);

status_t server_receive(server_t * server, char * buff, 
						size_t buff_size, size_t * buff_len);

status_t server_process(server_t * server, struct cypher * cypher);

#endif
