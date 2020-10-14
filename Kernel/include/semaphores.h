#include <scheduler.h>
#define MAX_SEM_VALUE 65535 
#define MAX_SIZE 100
#define NULL 0 


typedef struct sem_t
{
    char * name;
    int value;
    int semid;
    int lock;
    int blocked_pids[MAX_PROCESS];        //no esta implementado todavia
    int cant_blocked_pids;
    int pids[MAX_PROCESS];
    int cant_pids;
} sem_t;

int sem_size();

sem_t * sem_open(char * semName, char createFlag, int value);   
int sem_post(sem_t * sem);
int sem_wait(sem_t * sem);
int sem_close(sem_t * sem);
