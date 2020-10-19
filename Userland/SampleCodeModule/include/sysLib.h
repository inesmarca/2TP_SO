#ifndef __SYS_LIB_H_
#define __SYS_LIB_H_

#include <stdint.h>
#include <sem.h>
#define STDIN 0
#define STDOUT 1
#define MAX_PROCESS 20
#define MAX_SEMS 100

typedef struct infoPCB {
    int pid;
    char name[255];
    int priority;
    int state;
    char stackPointer[10];
    char basePointer[10];
    int foreground;
} infoPCB;

typedef struct infoSem {
    char name[50];
    int value;
    int blocked_pids[MAX_PROCESS];
    int cant_blocked;
} infoSem;

typedef struct infoPipe {
    int id;
    int nread;
    int nwrite;
    int write_blocked[MAX_PROCESS];
    int read_blocked[MAX_PROCESS];
} infoPipe;

extern int read(int fd, char * buf, int cant);

extern int write(int fd, const char * string, int cant);

extern int readPixel(int x, int y);

extern void writePixel(int x, int y, int color);

extern int getRTC(int x);

extern int getTemperature();

extern void clear();

extern void getRegisters(uint64_t * buff);

extern void setCursor(int x, int y);

extern void * malloc(int size);

extern void free(void * dir);

extern int create(const char * name, void * func, int priority, int fd[],int foreground, int argc, char * argv[]);

extern int kill(int pid, int state);

extern int getpid();

extern int nice(int pid, int priority);

extern void yield();

extern sem_t * sem_open(char * name, char flag, int value);

extern int sem_wait(sem_t * sem);

extern int sem_post(sem_t * sem);

extern int sem_close(sem_t * sem);

extern void changeColor(int letter, int background);

extern int pipe(int fd[]);

extern int close(int fd);

extern void memState(int fd[]);

extern int getInfoSem(int id, infoSem * buff);

extern int getListSem(int * buff);

extern int getListPids(int * buff);

extern int getInfoPCB(int pid, infoPCB * buff);

extern int getListPipes(int * buff);

extern int getPipeInfo(int id, infoPipe * buff);

#endif