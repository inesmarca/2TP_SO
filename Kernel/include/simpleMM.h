#ifndef __SIMPLE_MM_H_
#define __SIMPLE_MM_H_

void * malloc_simple(int size);

void free_simple(char * puntero);

int getTotalMem_Simple();

int getUsedMem_Simple();

#endif