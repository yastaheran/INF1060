/**
* IFISH:
*/
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "ifish.h"
#include "safefork.h"
#include "common.h"
#include "pidlist.h"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>

#define MAX_PARAM 20

/**
* MAIN METODEN: først kjører den spørsmålene(kommandolinje) før den tar imot svar
* (kommandoer) fra brukeren.
*/
int main() {
	char cmd[256];
	int exit;
	pid_t pid;
	
	while(prompt(), fgets((void *) cmd, sizeof(cmd), stdin)) {
		while((pid = waitpid(-1, &exit, WNOHANG)) > 0) { //fjerner pid fra lenkelisten
			removeFromPidlist(pid);
			debug("Backgrounded child with pid %i exited with status %i\n", pid, WEXITSTATUS(wexit));
		}
		
		strstrip(cmd);
		debug("typed: '%s'\n", cmd);
		
		if(!runcmd(cmd, 0)) {
			break;
		}
	}
	
	//venter til all bakgrunnspid har blir tatt ut (tom lenkeliste)
	while((pid = getFromPidlist()) > 0) {
		debug("\nWaiting on pid %i\n", pid);
		waitpid(pid, &exit, 0);
		debug("Backgrounded child with pid %i exited with status %i\n", pid, WEXITSTATUS(wexit));
		removeFromPidlist(pid);
	}
	h_destroy();
	return 0;
}

/**
* ERKOLON METODEN: sjekker om c er en kolon
* @param c
* @return 1 dersom true, 0 ellers
*/
int erKolon(int c) {
	if(c == ':') {
		return 1;
	} else {
		return 0;
	}
}

/**
* FULLPATH METODEN: Frigjør minne i PATH
* @param cmd
* @return 
*/
char *fullpath(char *cmd) {
	char *paths[41];
	char *path = strdup(getenv("PATH"));
	char *ut = NULL;
	int i;
	struct stat statbuffer;  //A structure where data about the file will be stored. A detailed look at all of the fields in this structure can be found in the struct stat page.

	strexplode(paths, 40, path, erKolon);
	
	//løkken skal gå igjennom paths og frigjøre minne
	for(i = 0; paths[i]; i++) {
		ut = malloc(strlen(paths[i]) + strlen(cmd) + 2);
		ut[0] = 0;
		strcat(ut, paths[i]);
		strcat(ut, "/");
		strcat(ut, cmd);
		
		if(stat(ut, &statbuffer) < 0 || !S_ISREG(statbuffer.st_mode)) {
			free(ut);
			ut = NULL;
			continue;
		}
		break;
	}
	free(path);
	return ut;
}

/**
* PROMPT METODEN:
*/
void prompt() {
	static int teller = 1;
	char *bruker = getenv("USER");
	printf("%s@" PROGNAME " %i>", bruker, teller++);
	fflush(stdout);
}

/**
* STRSTRIP METODEN: setter cmd[n] lik 0 dersom det er et blank tegn
*/
void strstrip(char *cmd) {
	int i;
	for(i = strlen(cmd) - 1; isspace(cmd[i]) && i >= 0; i--) {
		cmd[i] = 0;
	}
}

/**
* STREXPLODE METODEN: endrer kilden. Det er viktig å passe på at arrayen har plass 
* til nr+1 elementer
* @param ut array av char arrays
* @param nr hvor mange plasser arrayen har for elementer, ekskluderer 0
* @param in stringen som skal endres
* @param cb callback int cb(int) som avgjør om char er mellomrom
* @return lengden av ut-arrayen
*/
int strexplode(char **ut, int nr, char *inn, int (*cb)(int)) {
	int teller = 0;
	while(teller < nr && *inn) {
		while(*inn && cb(*inn)) { //fjerner mellomrom
			*inn = 0;
			inn++;
		}
		
		if(!*inn) {
			break;
		}
		
		ut[teller++] = inn;
		while(*inn && !cb(*inn)) {
			inn++;
		}
	}
	ut[teller] = 0;
	return teller;
}


/**
* RUNCMD METODEN: kjører kommando
* @param recursed hvis 0 er kommando fra bruker, 1 ellers
* @param cmdbuffer
* @return 1 for å fortsette og 0 for å avslutte
*/
char runcmd(const char *cmdbuffer, char recursed) {
	char *param[MAX_PARAM + 1];
	char background;
	char *cmd = strdup(cmdbuffer); //klone av cmd
	pid_t pid;
	b_ret r;
	int wexit, i, ret; //ret return kode for chold execve
	char *path; //fil path
	
	strstrip(cmd);
	
	if(cmd[strlen(cmd) - 1] == '&') {
		background = 1; //finne hvor background er deklarert
		cmd[strlen(cmd)-1] = 0;
		strstrip(cmd);
	} else {
		background = 1;
	}
	
	if(!strlen(cmd)) { //etter at vi har strippet av &, ender vi opp med å være tom
		free(cmd);
		return 1;
	}
	
	if(!recursed) { //Lagrer String så lenge den ikke er recursed
		h_store(cmdbuffer);
	}
	
	strexplode(param, MAX_PARAM, cmd, isspace);
	
	for(i = 0; param[i]; i++) {
		debug("param[%i]: %s\n", i, param[i]);	
	}
	
	r = b_run(param[0], param, recursed);	
	
	if(!(r & B_DID_NOT_RUN)) {
		free(cmd); //frigjør cmd slik at ram ikke lekker
		if(r & EXIT_PROGRAM) {
			debug("Builtin requested exit\n");
			return 0;
		}
		return 1;
	}
	
	pid = safefork();
	
	if(pid == -1) {
		free(cmd);
		return 1;
	} else if(pid > 0) { //parent
		if(!background) {
			waitpid(pid, &wexit, 0); //venter på child
			debug("Child process %i exited with status %i\n", pid, WEXITSTATUS(wexit));
		} else {
			err(PROGNAME ": Child running with pid %i\n", pid);
			addToPidlist(pid); //legger piden i lenkelisten
		}
	} else {
		h_destroy(); // frigør minne
		path = fullpath(param[0]);
		
		if(path == NULL) {
			err(PROGNAME ": %s: command not found\n", param[0]);
			free(cmd);
			exit(127);
			
			//path blir gjenvinnet, _environ er deklarert i unistd.h
			ret = execve(path, param, __environ);	
			free(cmd); //frigjør når vi er ferdig med dem
			free(path);
			exit(ret);
		}
	}
	free(cmd);
	return 1;
}