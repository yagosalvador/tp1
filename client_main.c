#include <stdio.h>

#include "client_client.h"
#include "common_status.h"
#include "common_validate_args.h"
#include "common_error.h"

int main(int argc, const char * argv[]){
	char key[20], method[20];
	status_t status = OK;

	if( validate_args(argc, argv, method, 20, key, 20) != 0 ){
		status = ERROR_ARGS;
		print_error(stderr, status);
		return status;
	}

	status = client_start(argc, argv, method, key);
	if( status != OK ){
		print_error(stderr, status);
		return status;
	}
	return 0;
}
