#ifndef WORKSHOP_MANAGER_H
#define WORKSHOP_MANAGER_H

#include "maki/mktype.h"
#include <stdio.h> // for size_t

typedef enum source { LOCAL, STEAMWORKSHOP, GAMEMAPS } source;

typedef struct dltket
{
	const source src;
	const uint32_t id;
	const char* fname;
} dltket;

typedef struct dlqueue dlqueue;
struct dlqueue
{
	size_t size;
	dltket *tkets;
	dltket *front, *rear;
	void (*push)(dlqueue*, const dltket*);
	void (*sift)(dlqueue*);
	void (*shrink)(dlqueue*);
};

extern dlqueue* init_dlqueue  (dlqueue* queue);
extern void close_dlqueue (dlqueue* queue);

#endif