#ifndef __COMMON_VALIDATE_ARGS_H__
#define __COMMON_VALIDATE_ARGS_H__


#include <stdio.h>

#define METHOD_ARG "--method="
#define METHOD_ARG_POS 2
#define KEY_ARG "--key="
#define MAX_ARGS 5

int validate_args(int argc, const char * argv[], char * method, 
				  size_t method_len, char * key, size_t key_len);

int parse_long_opt(const char * opt, char * arg, size_t len);

#endif
