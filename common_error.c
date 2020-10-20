#include <stdio.h>

#include "common_status.h"
#include "common_error.h"

static char * error_dict[MAX_ERRORS] = {
	"OK",
	"Error null pointer",
	"Error initializing socket",
	"Error destroying socket",
	"Error in function gettaddrinfo",
	"Error binding socket",
	"Error while socket was listening",
	"Error accepting socket",
	"Error connecting socket",
	"Error at socket shutdown",
	"Socket closed",
	"Error while socket was receiving",
	"Error while socket was sending",
	"Error reading buffer",
	"Error cypher init",
	"Error buffer init",
	"Error cypher digest",
	"Error buffer get",
	"Error cypher unknown method",
	"Error arguments"
};

void print_error(FILE * file, status_t status){
	fprintf(file, "%s", error_dict[status]);
	return;
}
