#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
* Oppgave 1:
* A - Tar en beskjed fra kommandolinjen. Lagrer beskjeden i variabelen msg. Skriver ut
* beskjeden "Input: " etterfult av beskjeden.
* B - Skal gi en feilmelding/hjelptekst hvis brukeren ikke sender med noe argument i
* kommandolinjen
*/

int isint(char *str);
int isint2(char *str);

/**
* Tar en beskjed fra kommandolinjen.
* argc : argument count
* argv : argument vector/array
*/
int main(int argc, char *argv[]) {
	//Lagrer beskjeden i variabelen msg
	char *msg;
	//Lagrer beskjed i variabelen tall, dersom det er et tall
	int tall;
	
	//sjekker om bruker har sendt inn argument i kommandolinjen, hvis false gir den en
	//feilmelding til bruker
	if(argc != 2) {
		printf("Usage: %s <message>\n", argv[0]);
		return 0;
	}
	
	printf("antall argumenter: %d\n", argc);
	printf("argv[0]: %s\n", argv[0]);
	
	msg =  argv[1];
	if(isint2(msg) == 1) {
		print("er et tall:\n");
		tall = atoi(msg);
		printf("tall: %d\n", tall);
	} else {
		printf("er ikke et tall :C\n");
	}
	return 0;
}

/**
* Sjekker om hele str er et tall
* str : stringen som skal sjekkes
*/
int isint(char *str) {
	printf("strlen(str): %zu\n", strlen(str));
	
	for(int i = 0; i < strlen(str); i++) {
		printf("%d: %c\n", i, str[i]);
		if(str[i] >= '0' && str[i] <= '9') {
			 continue;
		}
		return 0;
	}
	
	return 1;
}

/**
* Sjekker om hele str er et tall
* str : stringen som skal sjekkes
*/
int isint2(char *str) {
	char *end;
	long int num;
	
	num = strtol(str, &end, 10);
	if(end[0] == '\0')
		return 1;
	return 0
}