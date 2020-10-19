#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include <stdint.h>
#include <pipes.h>
#include <sysCall.h>
#define MAX_PROCESS 20
#define PRIORITY_LEVELS 3
#define STACK_SIZE 1024 * 4 

typedef struct pcb {
	uint64_t * rsp;
    void * function;
    int state;
    uint64_t * mallocPos;
    int pid;       
    char priority;  
    char name[255];
    int quantum;
    int has_pipe;
    int fd[MAX_PROCESS];
    int foreground;//si es foreground esto es 1 sino es bg y es 0
    int argc;
    char ** argv;
} pcb;

typedef struct infoPCB {
    int pid;
    char name[255];
    int priority;
    int state;
    char stackPointer[10];
    char basePointer[10];
    int foreground;
} infoPCB;

uint64_t * swap(uint64_t * rsp);

int createProcess(const char * name, void * func, int priority, int fd[],int foreground, int argc, char * argv[]);

int kill(int pid);

int block(int pid);

int unblock(int pid);

int getpid();

int nice(int pid, int priority);

void yield();

pcb * getPCB(int pid);

int getListPCB(infoPCB * buff[]);

void initializeScheduler();

int close(int fd);

int getInfoPCB(int pid, infoPCB * buff);

int getListPids(int * buff);

#endif