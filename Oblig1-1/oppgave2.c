/** 
 * OBLIG 1: OPPGAVE 2
 * Sammen med header 'oppg2.h' er denne koden komplett.
 * Dette programmet rakk jeg ikke å teste.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "oppg2.h"

////////////////////////////////PRINT KOMMANDO////////////////////////////////
/**
 * CMD_PRINT METODE: Hvis denne kommandoen er gitt skal programmet gå igjennom og 
 * skrive ut tegn for tegn. 
 */
void cmd_print() {
	int ch, i, n, sd = 0;
	char stri[n];
	
	while ((ch = fgetc(filINN)) != EOF) {
		stri[n++] = ch;
	}
	
    for(i = 0; i < n; i++) {
       	putchar(stri[i]);
		putchar(' '); 
    }
    fclose(filINN);
}

//////////////////////////////ENCODE/COMPRESS KOMMANDO////////////////////////////////
/**
 * METODEN: denne tar imot en array med fire tegn, og bytter deres bytes slik at de kan 
 * bli gjort om til enkle byte. De blir så skrevet inn i output-filen.
 */
void convRest(char tmp[]) {
	tmp[0] = tmp[0] << 6;
	tmp[1] = tmp[1] << 4;
	tmp[2] = tmp[2] << 2;
	
	//sørger for at en byte kan inneholde alle fire tegn.
	char ny = tmp[0] | tmp[1] | tmp[2] | tmp[3]; 
	
	putc(ny, filUT);
}

/**
 * CMD_ENCODE METODEN: denne metoden skal komprimere input-filen til en fil med bare 
 * 2bits-verdier(output-filen). Hvis totChar er høyre enn writtenChar kalles 
 * convRest, slik at alle tegn blir komprimert.
 */
void cmd_encode() {
/*	int i = 0;
	char inn[4];
	long totChar = 0;
	long writtenChar = 0;

	while (!feof(filINN)) { //kjører så lenge input-filen har tegn
		if(i < 4) {
			inn[i] = fgetc(filINN);
			if(inn[i] == ' ') {
				inn[i] = 0;
			} else if(inn[i] == ':') {
				inn[i] = 1;
			} else if(inn[i] == '@') {
				inn[i] = 2;
			} else if(inn[i] == '\n') {
				inn[i] = 3;
			} else {
				inn[i] = 0;
			}
			
			totChar++;
			i++;
		} else if(i == 4) {
			convRest(inn);
			writtenChar +=4;
			for(i = 0; i < 4; i++){
				inn[i] = 0;
			}
			i = 0;
		}
	}
	
	if(writtenChar < totChar) {
		convRest(inn);
	}
	
	fclose(filINN);
	fclose(filUT); */
}

///////////////////////////////DECODE/UNCOMPRESS KOMMANDO////////////////////////////////
/**
 * CMD_DECODE METODEN: gjør det motsatte av cmd_encode.
 */
void cmd_decode() {
/*	int i;
	char inn[4];

	while(!feof(filINN)) {
		char e = fgetc(filINN); //filen med de krypterte tegnene
		inn[0] = e;
		inn[1] = e;
		inn[2] = e;
		inn[3] = e;
	
		inn[0] = e << 6;
		inn[1] = e << 4;
		inn[2] = e << 2;
		
		for(i = 0; i < 4; i++) {
			inn[i] = inn[i] & 3;
			if(inn[i] == 0) {
				inn[i] = ' ';
			} else if(inn[i] == 1) {
				inn[i] = ':';
			} else if(inn[i] == 2) {
				inn[i] = '@';
			} else if(inn[i] == 3) {
				inn[i] = '\n';
			}
			putc(inn[i], filUT);
		}
	}
	fclose(filINN);
	fclose(filUT); */
}
///////////////////////////////PROGRAM & HELPE METODER ///////////////////////////////

/**
 * USAGE METODEN: Forteller bruker hvordan man skal bruke programmet.
 */
void usage() {
	printf("USAGE: ./filname [command] [input_file] [output_file] \n");
	printf("where \"command\" is one of the following: \n\n");
	printf("\t %-10s %s \n", "p", "print input file");
	printf("\t %-10s %s \n", "e", "encode/compress input_file to output_file");
	printf("\t %-10s %s \n", "d", "decode/uncompress and print input_file");
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
 * VERIFY_ARGS METODEN: sjekker om argumentene som er gitt er riktige. Dersom filen 
 * (inn og ut) == NULL, eller dersom filene er tomme skal metoden returnere null og perror
 * blir kalt. Hvis alt går som det skal returneres input-filen.
 */
FILE* verify_args(char *cmd, char *filnavnINN, char *filnavnUT) {
	
	if (filnavnINN == NULL && verify_cmd(cmd) == 0) {
		usage();
		return NULL;
	} else { 
/*		if(filnavnUT == NULL) {
			filUT = stdout;
		} */
		if((filINN = fopen(filnavnINN,"r")) == NULL) {
			perror(filnavnINN);
			return NULL;
		}
	}
	return filINN;
}

/**
 * MAIN METODEN
 */
int main(int argc, char *argv[]) {

	if(argc == 4) {
		if (verify_args(argv[1], argv[2], argv[3]) == NULL && strcmp(argv[1], "e")) {
			usage();
			return -1;
		}
		
	} else if(argc == 3) {
		if (verify_args(argv[1], argv[2], NULL) == NULL) {
			return -1;
		}
	} else if(argc > 4) {
		usage();
		return -1;
	}
	
	//sjekker hvilken kommando som er gitt
	if (strcmp(argv[1], "p") == 0) { 
		cmd_print();
	} else if (strcmp(argv[1], "e") == 0) { 
		cmd_encode(); 
	} else if (strcmp(argv[1], "d") == 0) { 
		cmd_decode();
	}

	return 0;
}