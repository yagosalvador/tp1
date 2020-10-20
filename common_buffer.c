#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "common_status.h"
#include "common_buffer.h"

int buffer_init(buffer_t * buffer){
	memset(buffer->buff, '\0', BUFF_SIZE); //set buffer to \0
	buffer->buff_size = BUFF_SIZE;
	buffer->buff_len = 0;
	buffer->eof = 0;

	return 0;
}

int buffer_read(FILE * file, buffer_t * buffer){
	memset(buffer->buff, '\0', BUFF_SIZE); //in case it has to be ovewritten

	size_t bytes = fread(buffer->buff, 1, buffer->buff_size, file);
	if( bytes != buffer->buff_size ){
		if( feof(file) )
			buffer->eof = 1;
		else
			return 1;
	}
	buffer->buff_len = bytes;

	return 0;
}

bool buffer_eof(const buffer_t buffer){
	return buffer.eof;
}

int buffer_get(const buffer_t buffer, char * output, size_t len){
	if( snprintf(output, len, "%s", buffer.buff) < 0 )
		return 1;
	return 0;
}
