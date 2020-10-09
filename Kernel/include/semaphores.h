#define MAX_SEM_VALUE 65535 
#define MAX_SIZE 10
#define NULL 0 

typedef struct semaphore
{
    char * name;
    int value;
    int semid;
    int pshare;
}semaphore;

int sem_size();

int sem_init(semaphore * sem, int pshare, int value);
int sem_post(semaphore * sem);
int sem_wait(semaphore * sem);
int sem_close(semaphore * sem);
