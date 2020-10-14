#ifndef __SYS_LIB_H_
#define __SYS_LIB_H_

#include <stdint.h>
#include <sem.h>

extern void readKeyBuff(char * buf, int cant);

extern void writeScreen(const char * string, int letter_color, int background_color);

extern int readPixel(int x, int y);

extern void writePixel(int x, int y, int color);

extern void sysChangeScreen(int screen);

extern int getRTC(int x);

extern int getTemperature();

extern void clear();

extern void getRegisters(uint64_t * buff);

extern void setCursor(int x, int y);

extern void * malloc(int size);

extern void free(void * dir);

extern int create(const char * name, void * func, int priority, int argc, char * argv[]);

extern int kill(int pid, int state);

extern int getpid();

extern int nice(int pid, int priority);

extern void yield();

extern sem_t * sem_open(char * name, char flag, int value);

extern int sem_wait(sem_t * sem);

extern int sem_post(sem_t * sem);

extern int sem_close(sem_t * sem);

#endif