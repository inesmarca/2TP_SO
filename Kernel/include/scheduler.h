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
    const char name[255];
    int quantum;
    int fd[MAX_PIPES];
} pcb;

typedef struct infoPCB {
    int pid;
    char name[50];
    int priority;
    int state;
    char stackPointer[10];
    char basePointer[10];
    int fd[2];
} infoPCB;

uint64_t * swap(uint64_t * rsp);

int createProcess(const char * name, void * func, int priority, int fd[], int argc, char * argv[]);

int kill(int pid);

int block(int pid);

int unblock(int pid);

int getpid();

int nice(int pid, int priority);

void yield();

pcb * getPCB(int pid);

int getListPCB(infoPCB * buff[]);

void initializeScheduler();

#endif