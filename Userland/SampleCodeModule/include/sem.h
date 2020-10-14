#define MAX_PROCESS 20

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