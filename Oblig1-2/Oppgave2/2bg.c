#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//variabler som blir brukt i begge funksjonene
int i, j, sum;
char c;
char* abc = "abcdefghijklmnopqrstuvwxyz";

int stringsum(char* s) {

	sum = 0;
	c = 0;
	
	for(i = 0; i < strlen(s); i++) {
		if((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z')) { //islalpha(s[i])
			
			//checks if upper case, if true changes letter to lower case
			if(isupper(s[i])) {
				c = putchar(tolower(s[i]));
			}
			c = s[i];
			for(j = 0; j < strlen(abc)/2; j++) {
				if(c == abc[j]) {
					sum = sum+(j+1);
				}
			}
		} else {
			return -1;
		}
	}
	return sum;
}

void stringsum2(char* s, int* res) {
	
	sum = 0;
	c = 0;
	
	res = (int *)malloc(sizeof(int));

	for(i = 0; i < strlen(s); i++) {
		if((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z')) { //islalpha(s[i])
			
			//checks if upper case, if true changes letter to lower case
			if(isupper(s[i])) {
				c = putchar(tolower(s[i]));
			}
			c = s[i];
			printf("%c\n", c);
			for(j = 0; j < strlen(abc)/2; j++) {
				if(c == abc[j]) {
					sum = sum+(j+1);
					printf("j verdi = %d og bokstav %c\n", j, abc[j]);
				}
			}
		} else {
			int r = -1;
			res = &r;
		}
	}
	res = &sum;
	free(res);
}