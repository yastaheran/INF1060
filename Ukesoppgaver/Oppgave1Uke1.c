#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	//A - tar en beskjed som kommandolinjearg. lagrer beskjeden i variabelen msg
	//skriver ut beskjeden "Input: " etterfulgt av msg.
	//B - skal gi en feilmelding/hjelpetekst hvis brukeren ikke sender med noe argument
	//på kommandolinjen
	//C - sjekk om msg er et heltall
	if(argc == 0) {
		printf("Usage:  ./a.out <beskjed>");
		return 0;
	}
	char *msg[argc];
	int msgTall;
	int i;
	for(i = 0; i < argc; i++) {
		if(msg[i] == 0 || msg[i] == 1 || msg[i] == 2 || msg[i] == 3 || msg[i] == 4 || msg[i] == 5 || msg[i] == 6 || msg[i] == 7 || msg[i] == 8 || msg[i] == 9) {
			msgTall[i] = msg[i];
		}
		msg[i] = argv[i];
	}
	
	printf("%s\n", msg);
	printf("%d", msgTall);
	return 1;
}