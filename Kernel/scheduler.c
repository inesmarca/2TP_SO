#include <scheduler.h>
#include <buddyMM.h>
#include <consoleManager.h>
#include <simpleMM.h>
#include <lib.h>
#include <sysCall.h>
#include <defs.h>
#include <stdint.h>
#include <pipes.h>

#define BLOCKED 2
#define KILLED 0
#define ACTIVE 1

// DEFINICION DE FUNCIONES
extern uint64_t * initializeStack(uint64_t * rsp, void * wrapper, void * func, int argc, char * argv[], int pid);
extern void tickInterrupt();
void _hlt();
static void reset_processes();
static int freeProcess();
static int getNewPid();
static void wrapper(void * func(int, char **), int argc, char * argv[], int pid);

// VARIABLES
static uint64_t processMemory[MAX_PROCESS][STACK_SIZE] = {{0}};
static pcb proceses[MAX_PROCESS] = {{0}};
static pcb * active_processes[PRIORITY_LEVELS][MAX_PROCESS] = {{0}};
static pcb * waiting_processes[PRIORITY_LEVELS][MAX_PROCESS] = {{0}};
static int number_of_proceses[PRIORITY_LEVELS] = {0};
static int number_of_proceses_snapshot[PRIORITY_LEVELS] = {0};

static int active_process_pid = -1;
static int curr_priority = PRIORITY_LEVELS - 1;
static int curr_index = 0;
static int next_priority = PRIORITY_LEVELS - 1;
static int next_index = 0;
static int waiting_index[PRIORITY_LEVELS] = {0};
static int cant_active_processes = 0;

void initializeScheduler() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        proceses[i].state = KILLED;
    }
}

// getpid
int getpid() {
    return active_process_pid;
}

// getPCB
pcb * getPCB(int pid) {
    return &proceses[pid];
}

// getListPCB
int getListPCB(infoPCB * buff[]) {
    int cant = 0;
    for (int pid = 0; pid < MAX_PROCESS; pid++) {
        if (proceses[pid].state != KILLED) {
            buff[cant]->pid = pid;
            strcpy(buff[cant]->name, proceses[pid].name);
            buff[cant]->priority = proceses[pid].priority;
            buff[cant]->state = proceses[pid].state;
            uintToBase((uint64_t)proceses[pid].rsp, buff[cant]->stackPointer, 16);
            uintToBase((uint64_t)proceses[pid].mallocPos, buff[cant]->basePointer, 16);
            buff[cant]->fd[0] = proceses[pid].fd[0];
            buff[cant]->fd[1] = proceses[pid].fd[1];
            cant++;
        }
    }
        
    return cant;
}

// swap
uint64_t * swap(uint64_t * rsp) {

    if (active_process_pid != -1 && active_processes[curr_priority][curr_index]->quantum != 0 && active_processes[curr_priority][curr_index]->state == ACTIVE) {
        active_processes[curr_priority][curr_index]->quantum--;
        return rsp;
    }
    
    int aux_total = cant_active_processes;
    if ( aux_total == 0) { //chequea caso no proceso(porbablemente se reemplaze por tirar a un proceso halt predefinido)
        return rsp;
    }
    
    if (active_process_pid != -1) { //ignora el caso de no proceso
        active_processes[curr_priority][curr_index]->rsp = rsp;    // guardo el contexto del proceso actual
        
        waiting_processes[curr_priority][waiting_index[curr_priority]] = active_processes[curr_priority][curr_index]; //lo paso al waiting list
        waiting_index[curr_priority]++;
        active_processes[curr_priority][curr_index] = NULL;//lo pongo en null
    }

    int cant = 0;
    int found = 0;
    
    while (!found) { //agregar contador para salir del while si no hay procesos
        if (curr_index == number_of_proceses_snapshot[curr_priority]) {  //se si tiene que cambiar de prioridad y de hacer el reset o no
            next_index = 0;
            if (curr_priority == PRIORITY_LEVELS - 1) {
                next_priority = 0;
                reset_processes();
            } else {
                next_priority++;
            }
        } else {
            next_index++;
        }

        if (number_of_proceses_snapshot[next_priority]>0&&active_processes[next_priority][next_index] != NULL) {
            if (active_processes[next_priority][next_index]->state == ACTIVE) { //found the next active process
                found = 1;
            } else {   
                if (active_processes[next_priority][next_index]->state != KILLED) {
                    //lo paso al waiting list
                    waiting_processes[next_priority][waiting_index[next_priority]] = active_processes[next_priority][next_index]; 
                    waiting_index[next_priority]++;
                    active_processes[next_priority][next_index] = NULL;
                } else { //si esta killed, borro la referencia
                    active_processes[next_priority][next_index] = NULL;
                }
            }
        }
        curr_index = next_index;
        curr_priority = next_priority;
        cant++;
    }
    active_process_pid = active_processes[next_priority][next_index]->pid;
    active_processes[next_priority][next_index]->quantum = PRIORITY_LEVELS - active_processes[next_priority][next_index]->priority;

    return active_processes[next_priority][next_index]->rsp;   // retorno el puntero del stack del proceso a switchear
}

// create
int createProcess(const char * name, void * func, int priority, int fd[], int argc, char * argv[]){
    if (priority < 0 || priority >= PRIORITY_LEVELS) {
        return -1;
    }

    pcb * newProcess = &proceses[freeProcess()];

    if (newProcess == NULL) {
        _hlt();
    }

    newProcess->pid = getNewPid();
    if (newProcess->pid == -1) {
        return -1;
    }
                            
    newProcess->function = func;
    newProcess->state = ACTIVE;
    memcpy(newProcess->name, name, 255);
    newProcess->priority = priority;
    
    newProcess->fd[0] = fd[0];
    newProcess->fd[1] = fd[1];

    newProcess->mallocPos = processMemory[newProcess->pid];
    newProcess->rsp = newProcess->mallocPos + STACK_SIZE;

    char ** auxArgv = malloc(argc);
    int auxArgc = argc;
    for (int i = 0; i < argc; i++) {
	    auxArgv[i] = malloc(255);
        memcpy(auxArgv[i], argv[i], 255);
    }

    newProcess->rsp = initializeStack(newProcess->rsp, wrapper, newProcess->function, auxArgc, auxArgv, newProcess->pid); // retorna el rsp luego de hacer los push

    waiting_processes[priority][waiting_index[priority]] = newProcess; //pongo el nuevo proceso en la lista de procesos para la proxima ejecucion
    waiting_index[priority]++;

    
    number_of_proceses[priority]++;
    cant_active_processes++;

    return newProcess->pid;
}

static void wrapper(void * func(int, char **), int argc, char * argv[], int pid) {

    (*func)(argc, argv);
    
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);

    kill(pid);
}

// kill
int kill(int pid) {
    if (pid >= 0 && pid < MAX_PROCESS) {
        proceses[pid].state = KILLED;
        number_of_proceses[(int)proceses[pid].priority]--;
        cant_active_processes--;
        if (pid == active_process_pid) 
            tickInterrupt();
        return 1;
    } else return -1;
}

// block
int block(int pid) {
    if (pid >= 0 && pid < MAX_PROCESS && proceses[pid].state != KILLED) {
        proceses[pid].state = BLOCKED;
        if (pid == active_process_pid) 
            tickInterrupt();
        return 1;
    } else return -1;
}

// unblock
int unblock(int pid) {
    if (pid >= 0 && pid < MAX_PROCESS && proceses[pid].state == BLOCKED) {
        proceses[pid].state = ACTIVE;
        return 1;
    } else return -1;
}

// nice
int nice(int pid, int priority) {
    if (priority < 0 || priority >= PRIORITY_LEVELS)
        return -1;

    proceses[pid].priority = priority;
    return 1;
}

// yield
void yield() {
    tickInterrupt();
}

/*          STATIC FUNCTIONS        */
static void reset_processes(){ //reacomoda la lista de active y waiting
    for (int i = 0; i < PRIORITY_LEVELS; i++) {   
        int k = 0;
        for (int j = 0; j < MAX_PROCESS; j++) {
            if (waiting_processes[i][j] != NULL) {
                active_processes[i][k] = waiting_processes[i][j]; //copio los procesos no nulos
                waiting_processes[i][j] = NULL;
                k++;
            }
        }
        while (k < MAX_PROCESS) {
            active_processes[i][k++] = 0; //limpio lo que quede poniendo 0
        }
        
    }

    for (int i = 0; i < PRIORITY_LEVELS; i++) { //reacomodar las cantidades de procesos
        number_of_proceses[i] = waiting_index[i];
        waiting_index[i] = 0;
        number_of_proceses_snapshot[i] = number_of_proceses[i];
    }
}

static int freeProcess() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (proceses[i].state == KILLED)
            return i;
    }
    return NULL; //si no encontro ningun proceso disponible
}

static int getNewPid() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (proceses[i].state == KILLED)
            return i;
    }

    return -1;
}