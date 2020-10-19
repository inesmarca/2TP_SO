/*
*                                   BIBLIOGRAFIA
*
*       https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/
*
*
*/
#include <sem.h>
#include <libC.h>
#include <sysLib.h>
#include <philo.h>
static int state[N];
static sem_t * mutex;
static sem_t * lock;
static sem_t * S[N];
static char * names[N];
static int philPID[N];
static int cant = 0;
static int numPhil = 0;
static int adding = 0;

static void putNames(){
     for (int i = 0; i < N; i++)
    {
        names[i] = malloc(3);
        names[i] = itoa(i, names[i], 10);
    }   
}

void printPhilos(int i){
    for (int j = 0; j < cant; j++)
    {
        if(j ==i) printf(" E ");
        else printf(" . ");
    }
    printf("\n");    
}

void test(int i){
    if (state[i] == HUNGRY && 
    ((i == 0)? state[cant -1] != EATING : state[i - 1] != EATING) &&
    ((i == cant - 1)? state[0] != EATING : state[i + 1] != EATING)) {    
        state[i] = EATING;
        printPhilos(i);
        sem_post(S[i]);
    }
}

void take_fork(int i){
    sem_wait(mutex);
    state[i] = HUNGRY;
    test(i);
    sem_post(mutex);
    sem_wait(S[i]);
}

void put_fork(int i){
    sem_wait(mutex);
    state[i] = THINKING;
    if (i == cant - 1) test(0);
    else    test(i + 1);
    if (i == 0) test(cant - 1);
    else    test(i - 1);
    sem_post(mutex);
}

int philospher(int argc, char ** argv){
    printf("FILOSOFO CREADO\n");
    if (argc != 0){
        printError("Error al iniciar filosofo");
        return -1;
    }
    int i = numPhil++;
    while (1) {
        sem_wait(lock);
        take_fork(i);
        put_fork(i);
        sem_post(lock);
    }
    return 0;
}

int addPhiloinit(int i){
    for (int vueltas = 0; vueltas < i; vueltas++)
    {
        sem_wait(lock);
    }


    
    if (cant >= N) return -1;
    S[i] = sem_open(names[i], 0, 0);
    if (S[i] == NULL)
    {
        printError("Error al crear semaforo");
        return -1;
    }

    int fd[MAX_PROCESS];
	memset(fd, -1, MAX_PROCESS);
	fd[0] = STDIN;
	fd[1] = STDOUT;

    philPID[i] = createBackground(names[i], philospher, 2,fd, 0, 0);
    cant++;
    for (int vueltas = 0; vueltas < i+1; vueltas++)
    {
        sem_post(lock);
    }
    return 0;    
}

int addPhilo(int i){
    for (int vueltas = 0; vueltas < i; vueltas++)
    {
        sem_wait(lock);
    }
     

    for (int vueltas = 0; vueltas < i; vueltas++){
        state[vueltas]=THINKING;
        if(S[vueltas]->value==0){
            sem_post(S[vueltas]);
        }
    }

    //state[0] = THINKING;
    // state[1] = THINKING;
    // state[i-2] = THINKING;
    // state[i-1] = THINKING;

    // if (S[0]->value==1)
    // {
    //     sem_post(S[0]);
    // }else
    // {
    //     sem_post(S[1]);
    // }
    // if (S[i-2]->value==1)
    // {
    //     sem_post(S[i-2]);
    // }else
    // {
    //     sem_post(S[i-1]);
    // }
    
    


    if (cant >= N) return -1;
    S[i] = sem_open(names[i], 0, 0);
    if (S[i] == NULL)
    {
        printError("Error al crear semaforo");
        return -1;
    }
    
    int fd[MAX_PROCESS];
	memset(fd, -1, MAX_PROCESS);
	fd[0] = STDIN;
	fd[1] = STDOUT;
    philPID[i] = createBackground(names[i], philospher, 2,fd, 0, 0);
    cant++;

    // sem_post(S[0]);
    // sem_post(S[i-1]);
    for (int vueltas = 0; vueltas < i+1; vueltas++)
    {
        sem_post(lock);
    }
    return 0;    
}

void killPhil(int i){
    kill(philPID[i], 0);
    cant--;
}

int philosphers(){
    putNames();
       
    printf("Problema de los filosofos comensales: \n");
    mutex = sem_open("mutex", 0, 1);
    lock = sem_open("lock", 0, 5);
    for (int i = 0; i < 5; i++) {
        sem_wait(lock);
        addPhiloinit(i);
        sem_post(lock);
    }
    printf("Filosofos pensando...\n");
    int k;
    while (1)
    {
        k = getChar();
        switch (k)
        {
        case 'a':
            printf("ADD recibido \n");
            
            addPhilo(cant);
           
            break;
        case 'r':
            killPhil(cant);           
            break;
        default:
            break;
        }
    }
}