#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int ishex(unsigned char c) {
	return ((c >= '0' && c <= '9') || 
			(c >= 'a' && c <= 'f') || 
			(c >= 'A' && c <= 'F'));
}

int hexval(unsigned char c) {
	if(ishex(c)) {
		if((c >= '0' && c <= '9')) { //hvis tall
			return c - '0';
		} else if (c >= 'a' && c <= 'f') { //hvis bokstav
			return 10 + c - 'a';
		} else if(c >= 'A' && c <= 'F') {
			return 10 + c - 'A';
		}
		return -EINVAL;
	}
}

int main(int argc, char* argv[]) {
	
	if(ishex(argv[1])) {
		printf("Is Hex \n");
		int i = hexval(argv[1]);
		printf("Hex value %d \n", i);
	} else {
		printf("Is not Hex \n");
	}
	
	return 0;
}