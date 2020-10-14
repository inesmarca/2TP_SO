#define MAX_PROCESS 20

typedef struct sem_t
{
    char * name;
    int value;
    int semid;
    int lock;
    int pids[MAX_PROCESS];        //no esta implementado todavia
    int cant;
}sem_t;