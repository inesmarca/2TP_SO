#ifndef __SYSCALL_H_
#define __SYSCALL_H_
#include <stdint.h>

void readKey(char * buf, int cant);

void writeString(const char * string, int letter_color, int background_color);

int getPixelData(int x, int y);

void printPixel(int x, int y, int color);

int getTemperature();

void getRecVec(uint64_t * buff);

void sysClear();

void setCursor(int x, int y);

#endif