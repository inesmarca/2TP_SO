#ifndef __SEMAPHORES_H_
#define __SEMAPHORES_H_

#include <scheduler.h>
#include <sysCall.h>
#define MAX_SEM_VALUE 65535 
#define MAX_SIZE 100


typedef struct sem_t {
    char name [20];
    int value;
    int semid;
    int lock;
    int blocked_pids[MAX_PROCESS];
    int cant_blocked_pids;
    int pids[MAX_PROCESS];
    int cant_pids;
} sem_t;

typedef struct infoSem {
    char name[50];
    int value;
    int blocked_pids[MAX_PROCESS];
    int cant_blocked;
} infoSem;

int sem_size();

sem_t * sem_open(char * semName, char createFlag, int value);   

int sem_post(sem_t * sem);

int sem_wait(sem_t * sem);

int sem_close(sem_t * sem);

void acquire(int * lock);

void release(int * lock);

int getInfoSem(int id, infoSem * buff);

int getListSem(int * buff);

void initializeSems();

#endif