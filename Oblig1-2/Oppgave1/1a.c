#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	
	//checks if arguments past are less or more then two, if true it will print usage
	if(argc < 2 || argc > 2) {
		printf("Usage:\n %s <your sentence>", argv[0]);
	}
	
	//prints the sentence
	printf("%s\n", argv[1]);
}