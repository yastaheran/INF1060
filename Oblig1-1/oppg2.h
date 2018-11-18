/**
 * Header for oppgave 2.
 */

//Variablene
FILE *filINN, *filUT;
char *cmds[] = {"p", "e", "d"};    

//Functions
void usage();
void cmd_print();
void cmd_decode();
void cmd_encode();
void lesFil(char *filINN);
FILE* verify_args(char *cmd, char *filINN, char *filUT);