#include <stdio.h>
#include <stdlib.h>
#include <cstring.h>

int main(int argc, char *argv[]) {
	if(argc < 4){
		printf("Three argument expected: ./04 <Name> <Age> <Filename>\n");
	}
	
	char name = argv[1];
	int age = argv[2];
	
	FILE *in_file = fopen(argv[3], w);
	char sentence[60];
	
	if(in_file == NULL) {
		printf("Error file missing \n");
		exit(-1);
	}
	
	strcat(sentence, "Hello " );
  	strcat(sentence, name);
  	strcat(sentence, ", you are ");
  	strcat(sentence, age);
  	strcat(sentence, " years old.");
	
	fprintf(in_file,"%s", sentence);
   	fclose(in_file);
	
}