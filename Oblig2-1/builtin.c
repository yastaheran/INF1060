/**
* BUILTIN: innebygde funksjoner 
* Jeg har ikke hatt tid til å rette alle feil
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "builtin.h"
#include "common.h"
#include "history.h"
#include "ifish.h"

/**
* array med alle kommandoene
*/
const builtin_f f[] = {
	{ .navn = "quit",    .function = b_quit    },
	{ .navn = "exit",    .function = b_quit    },
	{ .navn = "type",    .function = b_type    },
	{ .navn = "h",       .function = b_history },
	{ .navn = "cd",      .function = b_cd      },
	{ .navn = "pwd",     .function = b_pwd     },
	{ .navn = NULL,      .function = NULL      }
};

int recursed;

/**
* B_TYPE METODEN: finner ut om det er en funksjon som er builtin eller ikke
* printer ut til skjermen resultatet.
* @param argc antall argumenter
* @param argv kommandoen som er gitt
* @return 0 når metoden er ferdig
*/
b_ret b_type(int argc, char **argv) {
	int i, j;
	char erb; //er builtin
	for(i = 1; i < argc; i++) {
		erb = 0;
		for(j = 0; f[j].navn; j++)
			if(!strcmp(f[j].navn, argv[i])) {
				erb = 1;
				break;
			}
		if(erb) {
			printf("%s is a shell builtin\n", argv[i]);
		} else {
			printf("%s is not a builtin\n", argv[i]);
		}
	}
	return 0;
}

/**
* B_QUIT METODEN: avslutter programmet
* @param argc antall argumenter
* @param argv kommandoen som er gitt
* @return 0 for å avslutte
*/
b_ret b_quit(int argc, char **argv) {
	return EXIT_PROGRAM;
}

/**
* B_HISTORY METODEN: skriver ut de 10 sist brukte kommandoene, eldste først
* @param argc antall argumenter
* @param argv kommandoen som er gitt
* @return
*/
b_ret b_history(int argc, char **argv) {
	history_m *current;
	int antall = h_antallEnteries();
	int av = 0;
	argv[0] += 0;
	
	h_debug_bitmap();
	h_debug_blocks();
	
	if(argc <= 1) {
		history_m **p; //peker til peker
		
		p = malloc(sizeof(history_m *) *antall);
		current = h_getFirst();
		
		while(current) { //går igjennom alle history_m objektene og setter dem inn i p
			p[av++] = current;
			current = current->next;
		}
		
		printf("\nHistory list of the %i last commands:\n", antall);
		for(av = antall - 1; av >= 0; av--) { //går fra eldste til nyeste
			printf("%02i: %s\n", av + 1, h_str(p[av]));
		}
		
		free(p); //frigjør p når vi er ferdig med den
	} else if(argv == 2) {
		av = atoi(argv[1]); //finner verdien for argv[1]
		const char  *cmd;
		
		if(recursed) {
			err(PROGNAME ": h <i> does not repeat recursively\n");
			return 0;
		}
		
		if(av < 1 || av > antall) {
			err(PROGNAME ": Invalid offset to execute: %i\n", av);
			return 0;
		}

		current = h_getFirst();
		while(av > 1) {
			current = current->next;
			av--;
		}
		
		cmd = h_str(current);
		debug("Builtin history: running '%s'\n", cmd);
		
		if(!runcmd(cmd, 1)) { //hvis kommando ikke blir kjørt
			return 0;
		}
	} else if(argc > 2 && !strcmp(argv[1], "-d")) {
		av = atoi[2]; //finner verdien for argv[2]
		if(av < 1 || av > antall) {
			err(PROGNAME ": Invalid offset to delete: %i\n", av);
			return 0;
		}
		
		current = h_getFirst();
		while(av > 1) {
			current = current->next;
			av--;
		}
		
		debug("Deleting history string %s\n", h_str(current));
		h_slett(current);
	}
	
	return 0;
}

/**
* B_CD METODEN: endrer directory
* @param argc antall argumenter
* @param argv kommandoen som er gitt
* @return 0 når jobben er gjort
*/
b_ret b_cd(int argc, char **argv) {
	char *path;

	if(argc < 2) {
		path = getenv("HOME"); //leter etter variabelnavnet, og returnerer en peker til
							   //den korresponderende stringen
	} else {
		path = argv[1];
	}
	
	int retning = chdir(path); //endrer directory til path
	debug("path = %s\n", path);
	
	if(retning != 0) {
		err(PROGNAME ": Could not cd to %s\n", path);
	}
	return 0;
}

/**
* B_PWD METODEN: skal printe ut navnet på gjeldende directory
* @param argc antall argumenter
* @param argv kommandoen som er gitt
* @return 0 når metoden er ferdig
*/
b_ret b_pwd(int argc, char **argv) {
	char *path = getcwd(NULL, 0); //finner path
	printf("%s\n", path);
	free(path); 
	return 0;
}

/**
* B_RUN METODEN: kaller på kommando gitt i kommandolinje
* @param navn navnet på kommandoen
* @param argv kommandoen som er gitt
* @param r sier om kommandoen er recursed, kommer fra bruker eller ikke
* @return funksjonen med samme navn som navn, ellers så returnerer den 1
*/
b_ret b_run(const char *navn, char **argv, char r) {
	int i, argc;
	
	for(argc=0;argv[argc];argc++); //finner antall argumentasjoner argv har
	
	if(r != 0) {
		recursed = 1;
	} else {
		recursed = 0;
	}
	
	for(i = 0; f[i].navn; i++) {
		if(!strcmp(f[i].navn, navn)) {
			return f[i].function(argc, argv);
		}
	}
	
	return B_DID_NOT_RUN; //hvis den ikke kjører
}