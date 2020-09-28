#include <scheduler.h>
#include <buddyMM.h>
#include <consoleManager.h>
#include <simpleMM.h>
#include <lib.h>

#define MAX_PROCESS 4
#define STACK_SIZE 4096 
#define BLOCKED 2
#define KILLED 0
#define ACTIVE 1

typedef struct structProcess {
	uint64_t * rsp;
    void * function;
    int state;
    uint64_t * mallocPos;
    int pid;        // no lo vamos a implementar ahora
    char priority;  // no lo vamos a implementar ahora
} structProcess;

extern uint64_t * initializeStack(uint64_t * rsp, void * wrapper, void * func, int argc, char * argv[], int pid);
void wrapper(void * func(int, char **), int argc, char * argv[], int pid);
void kill(int pid);
int firstPosFree();
void _hlt();
extern void tickInterrupt();

static structProcess list_process[MAX_PROCESS] = {{0}};
static int index_next = 0;
static int dim_process = 0;
static int active_process_index = -1;
static int active_processes = 0;


uint64_t * swap(uint64_t * rsp) {
    if (dim_process == 0) {
        return rsp;
    }

    if (index_next == dim_process)
        index_next = 0;
    
    int cant = 0;
    while(list_process[index_next].state == KILLED && cant < MAX_PROCESS) {
        index_next++;
        cant++;
        if (index_next == dim_process)
            index_next = 0;
    }

    if (cant >= MAX_PROCESS)
        _hlt();

    list_process[active_process_index].rsp = rsp;    // guardo el contexto del proceso actual
    active_process_index = index_next;

    return list_process[index_next++].rsp;   // retorno el puntero del stack del proceso a switchear
}

void createProcess(void * func, int argc, char * argv[]) {
    int pos = dim_process;
    if (active_processes != dim_process && dim_process != 0) {
        pos = firstPosFree();               // supongamos que nunca tira -1 POR AHORA
        
        if (pos < dim_process) {            // si toma el lugar de un proceso no activo tengo que vaciar el stack
            free(list_process[pos].mallocPos);
        }
    }

    structProcess * newProcess = &list_process[pos];

    newProcess->pid = pos;                  // esto vamos a tener que cambiarlo                  
    newProcess->function = func;
    newProcess->state = ACTIVE;

    newProcess->mallocPos = (uint64_t *)malloc(STACK_SIZE);
    newProcess->rsp = newProcess->mallocPos + STACK_SIZE;

    newProcess->rsp = initializeStack(newProcess->rsp, wrapper, newProcess->function, argc, argv, newProcess->pid); // retorna el rsp luego de hacer los push

    if (pos == dim_process)
        dim_process++;
    active_processes++;
}

void wrapper(void * func(int, char **), int argc, char * argv[], int pid) {
    
    (*func)(argc, argv);
    
    kill(pid);
}

int firstPosFree() {
    int i = 0;

    while (list_process[i].state != KILLED && i < MAX_PROCESS)
        i++;

    if (i == MAX_PROCESS)
        return -1;
    
    return i;
}

void kill(int pid) {
    if (pid < dim_process && pid >= 0) {
        list_process[pid].state = KILLED;
        if (pid == active_process_index) 
            tickInterrupt();
    }
}

void block(int pid) {
    if (pid < dim_process && pid >= 0 && list_process[pid].state != KILLED) {
        list_process[pid].state = BLOCKED;
        if (pid == active_process_index) 
            tickInterrupt();
    }
}

void unblock(int pid) {
    if (pid < dim_process && pid >= 0 && list_process[pid].state != KILLED)
        list_process[pid].state = ACTIVE;
}