/**
* Headeren til history.c
*/
#ifndef __HISTORY_H
#define __HISTORY_H
typedef struct history_m_t history_m;

//
struct history_m_t {
	history_m *next;
	int length;
	unsigned char dataIndex[15]; // pointer to right block
};

//Funksjonene
history_m* h_getFirst();
int h_antallEnteries();
int h_freeblocks();
void h_store(const char *str);
const char* h_str(history_m *ptr);
void h_free();
void h_slett(history_m *current);
void h_destroy();

#ifdef DEBUG
void h_debug_bitmap();
void h_debug_blocks();
#endif

#endif