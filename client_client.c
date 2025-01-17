#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_socket.h"
#include "common_status.h"
#include "common_buffer.h"
#include "common_cypher.h"
#include "client_client.h"

status_t client_destroy(client_t * client){
	if ( client == NULL )
		return ERROR_NULL_POINTER;

	status_t status = socket_shutdown(&client->socket, 2);
	if( status != OK ){
		socket_destroy(&client->socket);
		return status;
	}

	status = socket_destroy(&client->socket);
	if( status != OK )
		return status;

	return OK;
}

status_t client_connect(client_t * client, const char * host, 
						const char * port){
	if( client == NULL || host ==  NULL || port == NULL)
		return ERROR_NULL_POINTER;

	status_t status = socket_connect(&client->socket, host, port);
	if( status != OK ){
		client_destroy(client);
		return status;
	}
	return OK;
}

status_t client_send(client_t * client, char * buffer, size_t buff_len){
	if( client == NULL || buffer == NULL )
		return ERROR_NULL_POINTER;

	status_t status = socket_send(&client->socket, buffer, buff_len);
	if( status != OK ){
		client_destroy(client);
		return status;
	}
	return OK;
}

status_t client_input_process(client_t * client, cypher_t * cypher, 
							  buffer_t * buffer){
	if( client == NULL || buffer == NULL )
		return ERROR_NULL_POINTER;

	char output[CLIENT_BUFF_SIZE];
	memset(output, '\0', CLIENT_BUFF_SIZE);

	if( buffer_read(stdin, buffer) != 0 ){
		client_destroy(client);
		return ERROR_BUFFER_READ;
	}

	//usar buff_cpy
	size_t buffer_len = buffer_get_len(buffer);
	if( cypher_digest(cypher, (unsigned char *)buffer->buff, buffer_len, 
					  (unsigned char *)output, CLIENT_BUFF_SIZE) != 0){
		client_destroy(client);
		return ERROR_CYPHER_DIGEST;
	}
	for(size_t i = 0; i < buffer_len; i++)
		fprintf(stdout, "%x", output[i]);
	fprintf(stdout, "\n");
	status_t status = client_send(client, output, buffer_len);
	if( status != OK ){
		client_destroy(client);
		return status;
	}
	return OK;
}

status_t client_start(int argc, const char * argv[], 
					  const char * method, const char * key){
	if( argv == NULL )
		return ERROR_NULL_POINTER;

	client_t client;
	status_t status = client_connect(&client, argv[1], argv[2]);
	if( status != OK ){
		client_destroy(&client);
		return status;
	}

	cypher_t cypher;
	int ret = cypher_init(&cypher, 0, key, method);
	if ( ret == 1 )
		return ERROR_CYPHER_INIT;
	if ( ret == -1 )
		return ERROR_CYPHER_UNKNOWN_METHOD;
	
	buffer_t buffer;
	if( buffer_init(&buffer) != 0 )
		return ERROR_BUFFER_INIT;

	while( !buffer_eof(&buffer) ){
		status = client_input_process(&client, &cypher, &buffer);
		if( status != OK )
			return status;
	}
	status = client_destroy(&client);
	if( status != OK )
		return status;
	
	return OK;
}
