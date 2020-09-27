#include <scheduler.h>
#include <buddyMM.h>

#define MAX_PROCESS 4
#define STACK_SIZE 4096 
typedef struct structProcess {
	uint64_t * rsp;
    void * function;
    int active;
    uint64_t * mallocPos;
    int pid;        // no lo vamos a implementar ahora
    char priority;  // no lo vamos a implementar ahora
} structProcess;

extern uint64_t * initializeStack(uint64_t * rsp, void * wrapper, void * func, int argc, char * argv[]);
void wrapper(void * func(int, char **), int argc, char * argv[], structProcess * process);



static structProcess list_process[MAX_PROCESS];
static int index = 0;
static int dim = 0;

uint64_t * swap(uint64_t * rsp) {
    if (index == dim)
        index = 0;
    list_process[index-1].rsp = rsp;    // guardo el contexto del proceso actual
    return list_process[index++].rsp;   // retorno el puntero del stack del proceso a switchear
}

uint64_t * createProcess(int argc, char * argv[], void * func) {

    if (dim == MAX_PROCESS) {
        for (int i = 0; i < dim; i++) {
            if (list_process[i].active == 0)
                free(list_process[i].mallocPos);
        }
    }

    structProcess * newProcess = &list_process[dim++];

    newProcess->pid = 0;
    newProcess->function = func;
    newProcess->active = 1;

    newProcess->mallocPos = (uint64_t *)malloc(STACK_SIZE);
    newProcess->rsp = newProcess->mallocPos + STACK_SIZE;

    newProcess-> rsp = initializeStack(newProcess->rsp, wrapper, newProcess->function, argc, argv); // retorna el rsp luego de hacer los push
    return newProcess->rsp;
}

void wrapper(void * func(int, char **), int argc, char * argv[], structProcess * process) {
    
    (*func)(argc, argv);
    
    process->active = 0;
    // free(process->mallocPos); resolver luego
}