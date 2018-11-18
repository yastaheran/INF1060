/**
* Headeren til builtin.c
*/
#ifndef __BUILTIN_H
#define __BUILTIN_H

#include "history.h"

#define B_DID_NOT_RUN 1
#define EXIT_PROGRAM  2

typedef struct builtin_function_t builtin_f;
typedef unsigned char b_ret;

struct builtin_function_t { //nodelisten med alle de innebygde funksjonene
	const char *navn;
	b_ret (*function)(int argc, char **argv);
};

//de forskjellige kommandoene
b_ret b_type(int argc, char **argv);
b_ret b_quit(int argc, char **argv);
b_ret b_history(int argc, char **argv);
b_ret b_cd(int argc, char **argv);
b_ret b_pwd(int argc, char **argv);

/* Get a zero-terminated array of builtins */
b_ret b_run(const char *name, char **argv, char recursed);

#endif