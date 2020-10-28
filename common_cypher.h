#ifndef __COMMON_CYPHER_H__
#define __COMMON_CYPHER_H__

#include <stdio.h>
#include <stdbool.h>

#define SIZE 256
#define N_CYPHER_FUNC 3

struct cypher;

typedef int (*cypher_func_t) (struct cypher * cypher, 
							 const unsigned char * input, const size_t in_len,
							 unsigned char * output, 
							 const size_t out_len);

typedef enum {
	CESAR = 0,
	VIGENERE = 1,
	RC4 = 2
} cypher_func;

typedef struct cypher {
	bool decrypt;
	unsigned char key[SIZE];
	size_t key_length;
	size_t i, j;
	cypher_func_t func;
	unsigned char permut_arr[SIZE];
} cypher_t;

int cypher_cesar(cypher_t * self, const unsigned char * string, size_t str_len,
				unsigned char * output, const size_t out_len);

int cypher_vigenere(cypher_t * self, const unsigned char * string, 
					size_t str_len, unsigned char * output, const size_t out_len);

int cypher_rc4(cypher_t * self, const unsigned char * string, size_t str_len, 
			   unsigned char * output, const size_t out_len);

int cypher_ksa(const unsigned char * key, const size_t key_length, 
			   unsigned char * permut_arr);

int cypher_prga(cypher_t * cypher, unsigned char * output, 
	            const size_t len, unsigned char * permut_arr);

int swap(void * a, void * b);

int cypher_init(cypher_t * cypher, bool decrypt, 
	            const char * key, const char * method_str);

int cypher_digest(cypher_t * cypher, const unsigned char * string,  
				  const size_t str_len, unsigned char * output, const size_t len);

int cypher_get_func(const char * str, cypher_func * func);

#endif
