#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Variables used in both functions */
int i, occurence; 

int distance_between(char* s, char c) {; 
	occurence = 0;
	int distance = 0;
	
	for(i = 0; i < strlen(s); i++) {
		if(s[i] == c && occurence == 0) {
			occurence++;
		} else if(s[i] == c && occurence == 1) {
			occurence++;
			distance++;
			return distance;
		} else if(s[i] != c && occurence == 1) {
			distance++;
		}
	}
	
	return -1;
} 

char* string_between(char* s, char c) {
	
	char* string = (char *) malloc(strlen(s));
	int j = 0;
	occurence = 0;
	
	for(i = 0; i < strlen(s); i++) {
		if(s[i] == c && occurence == 0) {
			occurence++;
		} else if(s[i] == c && occurence == 1) {
			occurence++;
			free(string);
			return string;
		} else if(s[i] != c && occurence == 1) {
			string[j] = s[i];
			printf("String: %c \t", string[j]);
			printf("j: %d \n", j);
			j++;
		}
	}
	free(string);
	return NULL;
}