#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "common_socket.h"
#include "common_status.h"
#include "server_server.h"
#include "common_cypher.h"


status_t server_destroy(server_t * server){
	if( server == NULL )
		return ERROR_NULL_POINTER;

	status_t status;

	status = socket_destroy(&server->accepter);
	if ( status != OK )
		return status;

	status = socket_destroy(&server->listener);
	if ( status != OK )
		return status;

	return OK;
}

status_t server_connect(server_t * server, const char * port){
	if( server == NULL )
		return ERROR_NULL_POINTER;

	status_t status = socket_bind_and_listen(&server->accepter, port);
	if( status != OK )
		return status;

	status = socket_accept(&server->accepter, &server->listener);
	if( status != OK )
		return status;

	return OK;
}

status_t server_receive(server_t * server, char * buff, size_t len){
	if( server == NULL || buff == NULL )
		return ERROR_NULL_POINTER;

	status_t status = socket_recv(&server->listener, buff, SERVER_BUFF_SIZE);
	if( ( status != OK ) && ( status != SOCKET_CLOSED ) )
		server_destroy(server);

	return status;
}

status_t server_start(int argc, const char * argv[], const char * method, const char * key){
	if( argv == NULL )
		return ERROR_NULL_POINTER;

	server_t server;
	char buff[SERVER_BUFF_SIZE], output[SERVER_BUFF_SIZE];
	cypher_t cypher;

	if ( cypher_init(&cypher, 1, key, method) != 0 )
		return ERROR_CYPHER_INIT;

	memset(buff, '\0', SERVER_BUFF_SIZE);
	memset(output, '\0', SERVER_BUFF_SIZE);

	status_t status = server_connect(&server, argv[1]);
	if( status != OK ){
		server_destroy(&server);
		return status;
	}

	status = server_receive(&server, buff, SERVER_BUFF_SIZE);
	if( ( status != OK ) && ( status != SOCKET_CLOSED ) ){
		server_destroy(&server);
		return status;
	}
	while( status != SOCKET_CLOSED ){
		
		if( cypher_digest(&cypher,(unsigned char *) buff,(unsigned char *) output, SERVER_BUFF_SIZE) != 0 ){
			server_destroy(&server);
			return ERROR_CYPHER_DIGEST;
		}

		fprintf(stdout, "%s", output);

		status = server_receive(&server, buff, SERVER_BUFF_SIZE);
		if( ( status != OK ) && ( status != SOCKET_CLOSED ) ){
			server_destroy(&server);
			return status;
		}
	}

	status = server_destroy(&server);
	if( status != OK )
		return status;

	return OK;
}