#include <scheduler.h>
#include <buddyMM.h>
#include <consoleManager.h>
#include <simpleMM.h>
#include <lib.h>
#include <sysCall.h>
#include <MM.h>

#define MAX_PROCESS 20
#define STACK_SIZE PAGESIZE 
#define BLOCKED 2
#define KILLED 0
#define ACTIVE 1

typedef struct pcb {
	uint64_t * rsp;
    void * function;
    int state;
    uint64_t * mallocPos;
    int pid;
    char priority;  // no lo vamos a implementar ahora
    const char * name;
} pcb;

extern uint64_t * initializeStack(uint64_t * rsp, void * wrapper, void * func, int argc, char * argv[], int pid);
void wrapper(void * func(int, char **), int argc, char * argv[], int pid);
int firstPosFree();
void _hlt();
extern void tickInterrupt();

static pcb list_process[MAX_PROCESS] = {{0}};
uint64_t processMemory[MAX_PROCESS][STACK_SIZE];
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
    while(list_process[index_next].state != ACTIVE && cant < MAX_PROCESS) {
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

int createProcess(const char * name, void * func, int argc, char * argv[]) {
    int pos = dim_process;
    if (active_processes == MAX_PROCESS) {
        return -1;
    }

    if (active_processes != dim_process && dim_process != 0)
        pos = firstPosFree();               // supongamos que nunca tira -1 POR AHORA

    pcb * newProcess = &list_process[pos];

    newProcess->pid = pos;                  // esto vamos a tener que cambiarlo                  
    newProcess->function = func;
    newProcess->state = ACTIVE;
    newProcess->name = name;
    newProcess->mallocPos = processMemory[pos];
    newProcess->rsp = newProcess->mallocPos + STACK_SIZE;
    newProcess->rsp = initializeStack(newProcess->rsp, wrapper, newProcess->function, argc, argv, newProcess->pid); // retorna el rsp luego de hacer los push

    if (pos == dim_process)
        dim_process++;
    active_processes++;
    return newProcess->pid;
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

int kill(int pid) {
    if (pid < dim_process && pid >= 0) {
        list_process[pid].state = KILLED;
        active_processes--;
        if (pid == active_process_index) 
            tickInterrupt();
        return 0;
    }
    return -1;
}

int block(int pid) {
    if (pid < dim_process && pid >= 0 && list_process[pid].state != KILLED) {
        list_process[pid].state = BLOCKED;
        if (pid == active_process_index) 
            tickInterrupt();
        return 0;
    }
    return -1;
}

int unblock(int pid) {
    if (pid < dim_process && pid >= 0 && list_process[pid].state != KILLED) {
        list_process[pid].state = ACTIVE;
        return 0;
    }
    return -1;
}

int getpid() {
    return active_process_index;
}