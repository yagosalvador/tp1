#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common_status.h"
#include "common_cypher.h"

int swap(void * a, void * b){
	if( a == NULL || b == NULL )
		return -1;

	unsigned char aux = *(unsigned char *) a;
	*(unsigned char *) a = *(unsigned char *) b;
	*(unsigned char *) b = aux;

	return 0;
}

int cypher_cesar(cypher_t * cypher, const unsigned char * string, 
			     size_t str_len, unsigned char * output, const size_t out_len){
	size_t cesar_offset = strtoul((char *) cypher->key, NULL, 10);

	for(size_t pos = 0; pos < out_len; pos++){
		if( pos >= str_len )
			output[pos] = '\0';
		else if ( cypher->decrypt )
			output[pos] = (string[pos] - cesar_offset) % 256;
		else
			output[pos] = (string[pos] + cesar_offset) % 256;
	}

	return 0;
}

int cypher_vigenere(cypher_t * cypher, const unsigned char * string,
					size_t str_len, unsigned char * output, const size_t out_len){
	for(size_t pos = 0; pos < out_len; pos++, cypher->i++){
		if( cypher->i == cypher->key_length )
			cypher->i = 0;
		if( pos >= str_len )
			output[pos] = '\0';
		else if ( cypher->decrypt )
			output[pos] = (string[pos] - cypher->key[cypher->i]) % 256;
		else
			output[pos] = (string[pos] + cypher->key[cypher->i]) % 256;
	}
	return 0;
}

int cypher_ksa(const unsigned char * key, const size_t key_length, 
			   unsigned char * permut_arr){
	for(size_t i = 0; i < 256; i++)
		permut_arr[i] = i;

	for(size_t i = 0, j = 0; i < 256; i++){
		j = (j + permut_arr[i] + key[i % key_length]) % 256;
		swap(&permut_arr[i], &permut_arr[j]);
	}

	return 0;
}

int cypher_prga(cypher_t * cypher, unsigned char * output, const size_t len, 
				unsigned char * permut_arr){
	for(size_t k = 0; k < len; k++){
		cypher->i = (cypher->i + 1) % 256;
		cypher->j = (cypher->j + permut_arr[cypher->i]) % 256;
		swap(&permut_arr[cypher->i], &permut_arr[cypher->j]);
		output[k] = permut_arr[(permut_arr[cypher->i]+permut_arr[cypher->j]) % 256];
	}
	return 0;
}

int cypher_rc4(cypher_t * cypher, const unsigned char * string,
			   size_t str_len, unsigned char * output, const size_t out_len){
	unsigned char permut_arr[256];

	memset(permut_arr, '\0', 256);

	int ret = cypher_ksa(cypher->key, cypher->key_length, permut_arr);
	if( ret != 0 )
		return ret;

	ret = cypher_prga(cypher, output, out_len, permut_arr);
	if( ret != 0 )
		return ret;

	for(size_t i = 0; i < out_len; i++){
		if( i >= str_len )
			output[i] = '\0'; //complete missing chars with null char
		else
			output[i] = string[i] ^ output[i];
	}

	return 0;
}

int cypher_digest(cypher_t * cypher, const unsigned char * string, 
				  const size_t str_len, unsigned char * output, const size_t out_len){
	int ret = cypher->func(cypher, string, str_len, output, out_len);
	if ( ret != 0 )
		return ret;

	return 0;
}

int cypher_init(cypher_t * cypher, bool decrypt, 
			    const char * key, const char * method_str){
	static cypher_func_t cypher_func_dict[N_CYPHER_FUNC] = {cypher_cesar, 
															cypher_vigenere, 
															cypher_rc4};

	cypher->i = cypher->j = 0;
	cypher->decrypt = decrypt;

	cypher_func fun;
	if( cypher_get_func(method_str, &fun) != 0 )
		return -1;

	cypher->func = cypher_func_dict[fun];

	int ret = snprintf((char *)cypher->key, SIZE, "%s", key);
	if(ret < 0)
		return 1;
	
	cypher->key_length = strlen(key);
	return 0;
}

int cypher_get_func(const char * str, cypher_func * func){
	static char * cypher_func_names[N_CYPHER_FUNC] = {"cesar", 
													  "vigenere", 
													  "rc4"};

	for(size_t i = 0; i < N_CYPHER_FUNC; i++){
		if(!strcmp(cypher_func_names[i], str)){
			*func = i;
			return 0;
		}
	}
	return 1;
}
