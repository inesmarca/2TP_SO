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
static sem_t * S[N];
static char * names[N];
static int philPID[N];
static int cant = 0;
static int numPhil = 0;


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
        if(j == i) printf(" E ");
        else printf(" . ");
    }
    printf("\n");    
}

void test(int i){
    if (state[i] == HUNGRY && 
    ((i == 0)? state[cant -1] != EATING : state[i - 1] != EATING) &&
    ((i == cant - 1)? state[0] != EATING : state[i + 1] != EATING)) {    
        state[i] = EATING;
        yield();
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
    yield();

    
    
}

void put_fork(int i){
    sem_wait(mutex);
    state[i] = THINKING;
    if (i == cant - 1) test(0);
    else    test(i + 1);
    if (i == 0) test(cant - 1);
    else    test(i - 1);
    yield();
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
        yield();
        take_fork(i);
        put_fork(i);
    }
    return 0;
}

int addPhiloinit(int i){
    if (cant >= N) return -1;
    S[i] = sem_open(names[i], 0, 0);
    if (S[i] == NULL) {
        printError("Error al crear semaforo");
        return -1;
    }

    // aseguro ambos lados
    int waited[2]={0};

    if (S[0]->value==1) {
        sem_wait(S[0]);
        waited[0]++;
    }
    if (S[i-1]->value==1) {
        sem_wait(S[i-1]);
        waited[1]++;
    }
    
    int fd[MAX_PROCESS];
	memset(fd, -1, MAX_PROCESS);
	fd[0] = STDIN;
	fd[1] = STDOUT;

    philPID[i] = createBackground(names[i], philospher, 2,fd, 0, 0);
    
    cant++;
    if (waited[0]==1) {
        sem_post(S[0]);
    }
    if (waited[1]==1) {
        sem_post(S[i-1]);
    }
    return 0;    
}

int addPhilo(int i){
    sem_wait(mutex);
    addPhiloinit(i);
    sem_post(mutex); //suelto los tenedores si los tengo
    return 0;    
}


void killPhil(int i){
    sem_wait(mutex);
    if (S[i]->value==1) {
        sem_wait(S[i]);
    }    
    if (kill(philPID[i], 0) == -1) {
        printf("Error al matar un filosofo");
    }
    if(sem_close(S[i]) != 0) {
        printf("Error cerrando sem");
    }
    cant--;
    numPhil--;
    sem_post(mutex);
    printf("FILOSOFO APAGADO\n");
}

void exitPhilo(){
    for (int i = cant - 1; i >= 0; i--) {
        killPhil(i);
    }
    for (int i = 0; i < N; i++)
    {
       free(names[i]);
    }
    
    sem_close(mutex);
    
}


int philosphers(){
    putNames();       
    printf("Problema de los filosofos comensales: \n");
    printf("Para agregar filososfos, presione 'a' \n");
    printf("Para remover filososfos, presione 'r' \n");
    printf("Para terminar el programa, presione 'c' \n");
    mutex = sem_open("mutex", 0, 1);
    sem_wait(mutex);
    for (int i = 0; i < 5; i++) {
        addPhiloinit(i);
    }
    sem_post(mutex);
    printf("Filosofos pensando...\n");

    int k, flag = 1;

    while (flag) {
        k = getChar();

        switch (k) {
        case 'a':
            printf("ADD recibido \n");
            addPhilo(cant);
            break;
        case 'r':
            printf("REMOVE recibido \n");
            killPhil(cant - 1);  
            if (cant == 0) flag = 0;
            break;
        case 'c':
            printf("EXIT recibido \n");
            exitPhilo();
            flag = 0;
            break;
        default:
            printf("No es una opcion valida");
            break;
        }
    }
    return 0;
}