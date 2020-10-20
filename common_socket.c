#define _POSIX_C_SOURCE 200112L

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>

#include "common_socket.h"
#include "common_status.h"
#include "common_server_config.h"
#include "common_client_config.h"

status_t socket_init(socket_t * self, int domain, int type, int protocol){
	if( self == NULL )
		return ERROR_NULL_POINTER;

	self->fd = socket(domain, type, protocol);
	if( self->fd == -1 )
		return ERROR_INITIALIZING_SOCKET;

	return OK;
}

status_t socket_destroy(socket_t * self){
	if( self == NULL )
		return ERROR_NULL_POINTER;

	if( close(self->fd) == -1 )
		return ERROR_DESTROYING_SOCKET;

	return OK;
}

status_t socket_bind_and_listen(socket_t * self, const char * port){
	if( self == NULL || port == NULL )
		return ERROR_NULL_POINTER;

	struct addrinfo *res, *rp, hints;
	status_t status;

	hints.ai_family = SERVER_FAMILY_TYPE;
	hints.ai_socktype = SERVER_SOCKET_TYPE;
	hints.ai_protocol = SERVER_PROTOCOL;
	hints.ai_flags = SERVER_FLAGS;

	if( getaddrinfo(NULL, port, &hints, &res) != 0 )
		return ERROR_GETADDRINFO;

	for( rp = res; rp != NULL; rp = rp->ai_next ){
		status = socket_init(self, rp->ai_family, 
							rp->ai_socktype, rp->ai_protocol);
		if( status != OK ){
			freeaddrinfo(res);
			return status;
		}

		if( bind(self->fd, res->ai_addr, res->ai_addrlen) == -1 ){
			socket_destroy(self);
			freeaddrinfo(res);
			return ERROR_BINDING_SOCKET;
		}
	}
	//should max_conn be part of sv config?
	if( listen(self->fd, MAX_LISTENING_CONNECTIONS) == -1 ){ 
		socket_destroy(self);
		freeaddrinfo(res);
		return ERROR_WHILE_LISTENING;
	}

	freeaddrinfo(res);
	return OK;
}

status_t socket_accept(socket_t * accepter, socket_t * listener){
	if( accepter == NULL || listener == NULL )
		return ERROR_NULL_POINTER;

	int fd;

	fd = accept(accepter->fd, NULL, NULL);
	if( fd == -1 )
		return ERROR_ACCEPTING_SOCKET;

	listener->fd = fd;
	return OK;
}

status_t socket_connect(socket_t * self, const char * host, const char * port){
	if( self == NULL || host == NULL || port == NULL)
		return ERROR_NULL_POINTER;
	
	struct addrinfo *res, *rp, hints;
	status_t status;

	hints.ai_family = CLIENT_FAMILY_TYPE;
	hints.ai_socktype = CLIENT_SOCKET_TYPE;
	hints.ai_protocol = CLIENT_PROTOCOL;
	hints.ai_flags = CLIENT_FLAGS;

	if( getaddrinfo(host, port, &hints, &res) != 0 ) //client config?
		return ERROR_GETADDRINFO;

	for( rp = res; rp != NULL; rp = rp->ai_next ){
		status = socket_init(self, rp->ai_family, 
							rp->ai_socktype, rp->ai_protocol);
		if ( status != OK ){
			freeaddrinfo(res);
			return status;
		}

		if( connect(self->fd, res->ai_addr, res->ai_addrlen) == -1 ){
			freeaddrinfo(res);
			socket_destroy(self);
			return ERROR_CONNECTING_SOCKET;
		}
	}
	freeaddrinfo(res);

	return OK;
}

status_t socket_shutdown(socket_t * self, size_t mode){
	if( self == NULL )
		return ERROR_NULL_POINTER;

	if( mode > 2 ){ /*shutdown mode unknown*/
		socket_destroy(self);
		return ERROR_SOCKET_SHUTDOWN;
	} 

	if( shutdown(self->fd, mode) == -1 ){
		socket_destroy(self);
		return ERROR_SOCKET_SHUTDOWN;
	}

	return OK;
}

status_t socket_send(socket_t * self, char * buff, size_t len){
	if( self == NULL || buff == NULL )
		return ERROR_NULL_POINTER;

	int bytes_sent = 0;
	while ( bytes_sent < len ) {
		int ret_value = send(self->fd, buff + bytes_sent, len - bytes_sent, MSG_NOSIGNAL);
		if ( ret_value == -1 )
			return ERROR_SOCKET_SEND;
		bytes_sent += ret_value;
	}
	return OK;
}

status_t socket_recv(socket_t * self, char * buff, size_t len){
	if( self == NULL || buff == NULL )
		return ERROR_NULL_POINTER;

	int bytes_recv = recv(self->fd, buff, len, 0);

	if ( bytes_recv == -1 )
		return ERROR_SOCKET_RECV;

	if ( !bytes_recv )
		return SOCKET_CLOSED;

	return OK;
}
