#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char** split(char* s) {
    
//char **split_line(char *str) {
    char* d = " ";			//the delimiter
    char* str = s;
    char* token = strtok(str, d);

    int buffSize = 1024;
    char **tokens = /*(char**)*/ malloc(buffSize * sizeof(char*));
    int position = 0;

    while(token[position] != NULL) { //token != NULL) { //Bus error: 10
    	printf("%s\n", token);
        tokens[position] = token;
        printf("%s\n", tokens[position]);
        token = strtok(NULL, d);
        position++;
    }
    tokens[position] = NULL;
	
	if (tokens) {
        int i;
        for (i = 0; *(tokens + i); i++)
        {
            printf("month=[%s]\n", *(tokens + i));
            free(*(tokens + i));
        }
        printf("\n");
        free(tokens);
    }
    
    return tokens;
//}
	
}
