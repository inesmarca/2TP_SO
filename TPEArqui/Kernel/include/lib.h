#ifndef _LIB_H
#define _LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

void baseToHexa(char * buff);

#endif