/**
* Header for oppgave 1.
**/

//Lenkelisten
struct node{
	char *linje;
	struct node *next;
}; typedef struct node node;

//Kommando funksjonene
void cmd_print();
void cmd_random();
void cmd_replace();
void cmd_remove();
void cmd_len();

//Program og hjelpe metoder
int sjekkVokal(char *c);
void usage();
FILE* verify_args(char *cmd, char *filnavn);
void lesFil();
void clean();

//Variables
FILE *filINN;
char *cmds[] = {"print", "random", "replace", "remove", "len"};
int linje_teller;
node *forste;