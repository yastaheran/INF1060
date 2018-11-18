/**
* Headeren til pidlist.c
*/

#ifndef __PIDLIST_H
#define __PIDLIST_H

#include <unistd.h> // pid_t

//funksjonene
void addToPidlist(pid_t pid);
pid_t getFromPidlist();
void removeFromPidlist(pid_t pid);

#endif