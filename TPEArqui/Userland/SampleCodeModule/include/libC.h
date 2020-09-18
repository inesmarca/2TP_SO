#ifndef __LIB_C_H_
#define __LIB_C_H_

#include <stdint.h>
#include <libFun.h>
#define DEFAULT_LETTER_COLOR 0xFFFFFF
#define DEFAULT_BACKGROUND_COLOR 0x000000
#define BLOCK 127


char * strcpy(char * destination,const char * source);

int strcmp(char * s1, char * s2);

int scanf(const char * format, ...);

int sscanf(const char *str,const char *format, ...);

void putChar(char str);

char getChar();

void printf(const char * format,...);

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

void baseToHexa(char * buff);

void changeLetterColor(int code);

void changeBackgroundColor(int code);


#endif