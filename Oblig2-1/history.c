/**
* HISTORY: inneholder funksjonene som skal være med på å ta være på en liste over de 
* siste kommandoene, hvor den eldste kommando ligger øverst og den nyeste nederst.
*/
#include <stdlib.h>
#include <string.h>

#include "history.h"
#include "common.h"

//Variabler
history_m *forste = NULL; //forste objektet
char hbuffer[64*8 + sizeof(unsigned long long)]; //skal ha plass til 64 8-byte blokker 
unsigned long long *bitmap = (unsigned long long *)hbuffer;
char *hist = hbuffer + sizeof(unsigned long long);

char *tmpstr = NULL; //for å returnere Strings

/**
* H_GETFIRST METODEN: henter forste history_m
* @return forste history_m objektet
*/
history_m* h_getFirst() {
	return forste;
}

/**
* H_ANTALLENTERIES METODEN:
* @return
*/
int h_antallEnteries() {
	history_m *current = forste;
	int antall = 0;
	while(current != 0) {
		current = current->next;
		antall++;
	}
	return antall;
}

/**
* H_FREEBLOCKS METODEN: frigjør blokker
* @return antall frigjorte blokker
*/
int h_freeblocks() {
	int antall = 0;
	unsigned long long map = ~*bitmap;
	
	while(map != 0) {
		if(map & 1) {
			antall++;
		}
		map = map >> 1;
	}
	return antall;
}

/**
* H_STORE METODEN: Lagrer strings
* @param str objektet som skal lagres
*/
void h_store(const char *str){
	history_m *meta = malloc(sizeof(history_m));
	int length, i;

	meta->next = forste;
	meta->length = 0;

	//Regner ut antall blokker som må allokeres
	length = strlen(str);
	if(length % 8){
		length = length + 8 - (length % 8);
	}
	length = length / 8;
	
	if(length > 15) {
		length = 15;
	}
	
	//For at det skal være nok plass må noen blokker frigjøres
	while(h_freeblocks() < length) {
		h_free();
	}
	
	for(i = 0; i < 64 && length; i++) {
		if(*bitmap & ((unsigned long long)1 << i)) { //hopper over brukte blokker
			continue;
		}
		
		meta->dataIndex[meta->length++] = i;
		*bitmap |= ((unsigned long long)1 << i);
		length--;

		if(strlen(str) >= 8) {
			memcpy(hist + (i*8), str, 8);
			str += 8;
		} else {
			memset(hist + (i*8), 0, 8);
			memcpy(hist+(i*8), str, strlen(str));
			str += strlen(str);
		}
	}
	forste = meta;
}

/**
* H_STR METODEN: lagrer String versjon av kommandoen som er gitt
* @param p peker til history_m objektet som er brukt
* @return en string 
*/
const char* h_str(history_m *p) {
	int i;

	if(tmpstr) { //fjern og alloker nt
		free(tmpstr);
	}
	
	tmpstr = malloc((p->length*8) + 1);

	tmpstr[p->length*8] = 0; //for worst case

	for(i = 0; i < p->length; i++) { //lager en kopi
		memcpy(tmpstr + (8*i), hist + (p->dataIndex[i]*8), 8);
	}

	return tmpstr;
}

/**
* H_FREE METODEN: frigjør den siste history_m i listen
*/
void h_free() {
	history_m *siste = forste;
	
	while(siste->next != NULL) {
		siste = siste->next;
	}
	
	if(siste == NULL) {
		err("Error: h_free called, but there is nothing to free\n");
		return;
	}
	
	h_slett(siste);
}

/**
* H_SLETT METODEN: sletter/frigjør en history_m
* @param current objektet som skal slettes
*/
void h_slett(history_m *current) {
	history_m *forrige = NULL;
	int i, frigjort = 0;
	unsigned char av;
	
	if(forste != current) {
		forrige = forste;
		while(forrige->next != current && forrige) {
			forrige = forrige->next;
		}
		
		if(!forrige) {
			err("Error, could not find parent of the node to free\n");
			return;
		}
	}
	
	for(i = 0; i < current->length; i++) {
		av = current->dataIndex[i];
		*bitmap &= ~((unsigned long long)1 << av);
		memset(hist + (av * 8), 0, 8);
		frigjort++;
	}
	
	debug("h_slett freed %i blocks\n", frigjort);
	
	if(forrige == NULL) {
		forste = current->next;
	} else {
		forrige->next = current->next;
	}
	
	free(current);
}

/**
* H_DESTROY METODEN: Frigjør minne, starter fra første history_m og frigjør en og en
*/
void h_destroy() {
	history_m *tmp;
	
	while(forste != NULL) {
		 tmp = forste;
		 forste = tmp->next;
		 free(tmp);
	}
	
	if(tmpstr) {
		free(tmpstr);
	}
	
	tmpstr = NULL;
	bitmap = 0;
}

//#ifdef DEBUG
/**
* H_DEBUG_BITMAP METODEN: debugger bitmapen
*/
void h_debug_bitmap() {
	unsigned long bitmh = (*bitmap >> 32);
	unsigned long bitmlen = *bitmap & 0xffffffff;
	int i;
	err("DEBUG - BITMAP: \n\n");
	
	for(i = 0; i < 32; i++) { //går igjennom hver linje
		err("%s", ((bitmh&1) ? "1" : "0")); //hvis sant skriver ut 1, 0 ellers
		bitmh >>= 1;
	}
	err("\n");
	for(i = 0; i < 32; i++) { //går igjennom hver linje
		err("%s", ((bitmlen&1) ? "1" : "0")); //hvis sant skriver ut 1, 0 ellers
		bitmlen >>= 1;
	}
	err("\n");
}

/**
* H_DEBUG_BLOCKS METODEN: debugger datablokkene
*/
void h_debug_blocks() {
	int i, j;
	char c;
	err("DEBUG - DATABLOCKS:\n\n");
	
	for(i = 0; i < 64; i++) { //det skal være 64
		if(i%4 == 0) {
			err("##");
		}
		
		for(j = 0; j < 8; j++) { //8-bytes blokker
			c = hist[(i * 8) + j];
			
			if(c == 0) {
				c = ' ';
			}
			fwrite(&c, 1, 1, stderr);
		}
		err("##");
		if(i % 4 == 3) {
			err("\n");
		}
	}
}
//#endif