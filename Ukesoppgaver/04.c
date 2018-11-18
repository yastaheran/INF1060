#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STREQ(s1, s2) (strcmp(s1,s2) == 0)

int main(int argc, char* argv[]) {

	if(STREQ("A", "A")) {
		printf("Riktig\n");
	}
	
	if(STREQ("A", "B")) {
		printf("Feil\n");
	}
	return 0;
}