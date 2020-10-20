#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_validate_args.h"

int get_long_opt(const char * opt, char * arg, size_t len){
	if ( opt == NULL || arg == NULL )
		return -1;

	char * ptr = strchr(opt, '=') + 1;
	int ret = snprintf(arg, len, "%s", ptr);
	if ( ret < 0 )
		return 1;
	return 0;
}


int validate_args(int argc, const char * argv[], char * method, 
				  size_t method_len, char * key, size_t key_len){
	if( argv == NULL )
		return -1;

	if( (argc != MAX_ARGS) && (argc != MAX_ARGS - 1) )
		return 1;

	for(size_t i = METHOD_ARG_POS, method_pos; i < argc; i++){
		if( strstr(argv[i], METHOD_ARG) != NULL ){
			method_pos = i;
			get_long_opt(argv[i], method, method_len);
			if( (method_pos + 1 < argc) && 
				(strstr(argv[method_pos + 1], KEY_ARG) != NULL) ){
				get_long_opt(argv[method_pos+1], key, key_len);
				return 0;
			}
			return 1;
		}
	}
	return 1;
}
