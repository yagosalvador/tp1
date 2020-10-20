#ifndef __COMMON_BUFFER_H__
#define __COMMON_BUFFER_H__

#include <stdio.h>
#include <stdbool.h>

#define BUFF_SIZE 64

typedef struct{
	char buff[BUFF_SIZE];
	size_t buff_size;
	size_t buff_len;
	bool eof;
} buffer_t;

int buffer_init(buffer_t * buffer);

int buffer_read(FILE * file, buffer_t * buffer);

bool buffer_eof(const buffer_t buffer);

int buffer_get(const buffer_t buffer, char * output, size_t len);

size_t buffer_get_size(const buffer_t buffer);

#endif