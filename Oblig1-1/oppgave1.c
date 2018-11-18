/** 
 * OBLIG 1: oppgave 1
 * Sammen med header 'oppg1.h' er koden komplett.
 *
 * Random-metoden fungerer nesten som den skal, det hender den skriver ut samme linje
 * flere ganger etter hverandre, men resten av metodene skal fungere. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "oppg1.h"

///////////////////////////////KOMMANDO METODENE///////////////////////////////
/**
 * CMD_PRINT METODE: Hvis denne kommandoen er gitt skal programmet gå igjennom lenkelisten 
 * og skrive ut teksten linje for linje. 
 */
void cmd_print() {
	node *tmp = forste;
	while (tmp != NULL) {
		printf("\t %s", tmp->linje);
		tmp = tmp->next;
	}
	printf("\n"); //for at uskriften skal se litt finere ut
}

/**
 * CMD_RANDOM METODEN: skal skrive ut en tilfeldig linje fra teksten. Jeg bruker srand()
 * for å generere et tilfeldig tall.
 */
void cmd_random() {
	time_t sec;
	srand(time(&sec));

	int printLinje = rand() % linje_teller;
	node *tmp = forste;
	int i = 0;

	while (i < linje_teller) {
		if (i != printLinje) {
			tmp = tmp->next;
			i++;
		} else {
			break;
		}
	}
	printf("Random linje: \n");
	printf("\t %s", tmp->linje);
}

/**
 * CMD_REPLACE METODEN: Skal medføre at programmet går i en løkke - en gang for hver vokal. 
 * For hver runde skal programmet sjekke tegn for tegn om det er en vokal eller ikke - 
 * hvis det er en vokal skal man bytte denne ut med den tilsvarende vokalen for denne 
 * runden. Teksten skal skrives ut for hver runde. 
 */
void cmd_replace() {
	char vokaler[] = {'a', 'e', 'i', 'o', 'u', 'y', 'æ', 'ø', 'å'};
	int i;
	
	for (i = 0; i < (int)sizeof(vokaler); i++) {
		printf("vokal '%c' \n", vokaler[i]);
		node *tmp = forste;

		while (tmp != NULL) {
			int j;
			for (j = 0; j < (int)strlen(tmp->linje); j++) {
				if (sjekkVokal(&tmp->linje[j]) == 0) {
					tmp->linje[j] = vokaler[i];
				}
			}
			printf("\t %s", tmp->linje);
			tmp = tmp->next;
		}
		printf("\n");
	}
}

/**
 * CMD_REMOVE METODEN: skal sjekke tegn for tegn om det er en vokal eller ikke - hvis 
 * det er en vokal skal den fjernes (ikke byttes ut med blank). Utskriften skal 
 * være uten vokaler.
 */
void cmd_remove() {
	node *tmp = forste;
	int i;
	while (tmp != NULL) {
		char nyLinje[1000] = "";
		for (i = 0; i < (int)strlen(tmp->linje); i++) {
			if (sjekkVokal(&tmp->linje[i]) != 0) {
				strncat(nyLinje, &tmp->linje[i], 1);
			}
		}
		printf("\t %s", nyLinje);
		tmp = tmp->next;
	}
	printf("\n");
}

/**
 * CMD_TELLER METODEN: skal skrive ut hvor mange tegn det er tilsammen i fila.
 */
void cmd_len() {
	node *tmp = forste;
	int teller = 0;

	while (tmp != NULL) {
		teller += strlen(tmp->linje);
		tmp = tmp->next;
	}
	printf("Det er %d tegn i filen. \n", teller);
}

///////////////////////////////PROGRAM & HELPE METODER ///////////////////////////////

/**
 * FRIGJORMINNE METODEN: fjerner all data fra minne. Dette skal frigjøre litt minne og for å 
 * slippe segmentation faults.
 */
void friMinne() {
	node *tmp;
	node *current = forste->next;

	free(forste->linje);
	forste->linje = NULL;
	free(forste);
	forste = NULL;

	while (current != NULL) {
		tmp = current;
		current = tmp->next;

		free(tmp->linje);
		tmp->linje = NULL;
		free(tmp);
		tmp = NULL;
	}
	current = NULL;
}

/**
 * USAGE METODEN: Forteller bruker hvordan man skal bruke programmet.
 */
void usage() {

	printf("USAGE:./filnavn [command] [filINN] \n");
	printf("where \"command\" is one of the following: \n\n");
	printf("\t %-10s %s \n", "print", "print input file");
	printf("\t %-10s %s \n", "random", "print a random line");
	printf("\t %-10s %s \n", "replace", "replace the vowels with all the other vowels");
	printf("\t %-10s %s \n", "remove", "remove vowels");
	printf("\t %-10s %s \n", "len", "print the number of characters in the input file");
}

/**
 * VERIFY_CMD METODEN: tar en gitt kommando og sjekker om den eksisterer i cmds-arrayen.
 * Hvis den finnes returnes 0, hvis ikke returneres 1.
 */
int verify_cmd(char *cmd) {
	int i;
	for (i = 0; i < (int)sizeof(cmds); i++) {
		if (strcmp(cmd, cmds[i]) == 0) {
			return 1;
		}
	}
	return 0;
}

/**
 * VERIFY_ARGS METODE: verifiserer argumentene gitt til programmet. Dersom filnavn == NULL
 * og verify_cmd == 0 kalles usage, og metoden returnerer NULL. Hvis filnavnet != NULL og
 * verify_cmd != 0, skal filen åpnes og leses. Hvis filen er tom skal perror kalles og
 * metoden returner NULL.
 */
FILE* verify_args(char *cmd, char *filnavn) {	
	if (filnavn == NULL && verify_cmd(cmd) == 0) {
		fclose(filINN);
		usage();
		return NULL;
	} else {
		filINN = fopen(filnavn, "r");
		if(filINN == NULL) {
			perror(filnavn);
			return NULL;
		}
		return filINN;
	}
}

/**
 * SJEKKVOKAL METODEN: sjekker om tegnet er en vokal eller ikke. Hvis det er en vokal
 * returnerer den 0, returnerer 1 ellers.
 */
int sjekkVokal(char *c) {
	char vokaler[] = {'A', 'a', 'E', 'e', 'I', 'i', 'O', 'o', 'U', 'u', 'Y', 'y', 'Æ', 'æ', 'Ø', 'ø', 'Å', 'å'};
	int i;

	for (i = 0; i < (int)sizeof(vokaler); i++) {
		if (*c == vokaler[i]) 
			return 0;
	}
	return 1;
}

/**
 * LESFIL METODEN: leser dataen i input-filen. En buffer tar vare på hver linje.
 * fgets() gjør at en linje > 999 blir delt i to slik at alle linjer er <1000.
 * lengrelinje_teller blir senere brukt av random metoden. stdin. 
 * Når filen er ferdig, lukkes den.
 */
void lesFil() {

	char linje[1000];
	node *tmp;
	linje_teller = 0;

	while (!feof(filINN)) {
		if (forste == NULL) {
			forste = malloc(sizeof(node));
			fgets(linje, sizeof(linje), filINN);
			forste->linje = strdup(linje);
			forste->next = NULL;
			linje_teller++;
		} else {
			tmp = forste;
			while (tmp->next != NULL) {
				tmp = tmp->next;
			}
			tmp->next = malloc(sizeof(node));
			tmp = tmp->next;
			fgets(linje, sizeof(linje), filINN);
			tmp->linje = strdup(linje);
			tmp->next = NULL;
			linje_teller++;
		}
	}
	fclose(filINN);
}

/////////////////////////////// MAIN METODEN ///////////////////////////////
/** 
 * Hvis argc > 3, kalles usage() som viser bruker hvordan programmet skal brukes.
 * Hvis argc == 3, sjekkes både kommandoen og filnavnet, for at programmet skal fortsette
 * må return verdien være != NULL.
 */
int main(int argc, char *argv[]) {

	if (argc == 1 || argc > 3) {
		usage();
		return -1;
	}

	if (argc == 3) {
		if (verify_args(argv[1], argv[2]) != NULL) {
			printf("Gyldig kommando og fil \n");		
		} else {
			return 1;
		}
	}

	lesFil();
	
	//sjekker hvilken kommando som er gitt
	if (strcmp(argv[1], "print") == 0) { 
		cmd_print();
	} else if (strcmp(argv[1], "random") == 0) { 
		cmd_random(); 
	} else if (strcmp(argv[1], "replace") == 0) { 
		cmd_replace();
	} else if (strcmp(argv[1], "remove") == 0) {
		cmd_remove();
	} else if (strcmp(argv[1], "len") == 0) { 
		cmd_len();
	}

	friMinne();
	return 0;
}