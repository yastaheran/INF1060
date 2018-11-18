#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
	if(argc < 3){
		printf("Two argument expected: ./03 <Word> <Filename>\n");
	}
	
	int count = 0;
	int i;
	char word = argv[1]
	char tmp;
	
	FILE *in_file = fopen(argv[2], r);
	
	if(if_file == NULL) {
		printf("Error file missing \n");
		exit(-1);
	}
		
	printf("%s \n", word);
	
	while(!feof(in_file)) {
		fscanf(in_file,"%s", tmp);

  		if(strcmp(tmp, word)==0){
  			count++
    		printf("A match has been found");
  		}
	}
	
	fclose(in_file);
}