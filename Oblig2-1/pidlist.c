/**
* PIDLIST:  Listen med alle PID
*/

#include <unistd.h> // pid_t
#include "common.h"
#include "pidlist.h"

#define DIVISOR (sizeof(DATATYPE) *8)
#define DISTINCT_PIDS 65536
#define DATATYPE unsigned char

//denne metoden tar opp til 8KB av minne
DATATYPE pidTable[DISTINCT_PIDS/DIVISOR];

/**
* ADDTOPIDLIST METODEN: legger til en PID i listen
* @param pid objektet som skal legges til listen
*/
void addToPidlist(pid_t pid) {
	int i = pid/DIVISOR;
	int av = pid%DIVISOR;
	pidTable[i] |= 1 << av;
}

/**
* GETFROMPIDLIST METODEN: returnerer hvilken som helst PID, hvis det finnes en
* @return PID, eller -1 hvis det ikke finnes noe å hente
*/
pid_t getFromPidlist() {
	unsigned int i, av;
	DATATYPE copy;
	
	for(i = 0; i < (DISTINCT_PIDS/DIVISOR); i++) {
		if(pidTable[i] != 0) //hvis det er noe i pidtable[i]
			break;
	}
	
	if(i == DISTINCT_PIDS/DIVISOR) {
		return -1;
	}
	copy = pidTable[i];

	av = 0;
	while((copy & ((DATATYPE)1 << av)) == 0 && av < DIVISOR) {
		av++;
	}
	
	if(av == DIVISOR) {
		err(PROGNAME ": getFromPidlist(): PID should be in range %lu to %lu\n", i*DIVISOR, (i + 1)*DIVISOR);
		return -1;
	}
	
	return (i * DIVISOR) + av; //returnerer PID
}

/**
* REMOVEFROMPIDLIST METONDEN: fjerner en PID fra listen
* @param pid objektet som skal fjernes
*/
void removeFromPidlist(pid_t pid) {
	int pos = pid/DIVISOR;
	int av = pid%DIVISOR;
	
	pidTable[pos] &= (1 << av); //er litt usikker på om denne stemmer
}