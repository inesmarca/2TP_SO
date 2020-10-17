#ifndef __SEM_H_
#define __SEM_H_
//#include <sysLib.h>
#define MAX_PROCESS 20

typedef struct sem_t
{
    char * name;
    int value;
    int semid;
    int lock;
    int blocked_pids[MAX_PROCESS];
    int cant_blocked_pids;
    int pids[MAX_PROCESS];
    int cant_pids;
} sem_t;


#endif