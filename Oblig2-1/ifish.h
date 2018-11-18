/**
* Dette er en av headerene som blir brukt til ifish.c
*/
#ifndef __IFISH_H
#define __IFISH_H

#include "history.h"
#include "builtin.h"

//Funksjonene
int erKolon(int c);
char *fullpath(char *cmd);
void prompt();
void strstrip(char *cmd);
int strexplode(char **ut, int nr, char *inn, int (*cb)(int));
char runcmd(const char *cmd, char recursed);

#endif