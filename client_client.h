#ifndef __CLIENT_CLIENT_H__
#define __CLIENT_CLIENT_H__

#include <stdio.h>

#include "common_socket.h"
#include "common_status.h"
#include "common_buffer.h"
#include "common_cypher.h"

#define CLIENT_BUFF_SIZE 64

typedef struct {
	socket_t socket;
} client_t;

status_t client_init(client_t * client);

status_t client_destroy(client_t * client);

status_t client_start(int argc, const char * argv[], 
					  const char * method, const char * key);

status_t client_connect(client_t * client, const char * host, 
						const char * port);

status_t client_send(client_t * client, char * buffer, size_t buff_len);

status_t client_input_process(client_t * client, cypher_t * cypher, 
							  buffer_t * buffer);

#endif
