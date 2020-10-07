#include <scheduler.h>
#include <buddyMM.h>
#include <consoleManager.h>
#include <simpleMM.h>
#include <lib.h>
#include <sysCall.h>

#define MAX_PROCESS 20
#define PRIORITY_LEVELS 3
#define STACK_SIZE 4096 
#define BLOCKED 2
#define KILLED 0
#define ACTIVE 1
#define NULL 0

typedef struct pcb {
	uint64_t * rsp;
    void * function;
    int state;
    uint64_t * mallocPos;
    int pid;        // no lo vamos a implementar ahora
    char priority;  // no lo vamos a implementar ahora
    const char * name;
} pcb;

extern uint64_t * initializeStack(uint64_t * rsp, void * wrapper, void * func, int argc, char * argv[], int pid);
void wrapper(void * func(int, char **), int argc, char * argv[], int pid);
int firstPosFree();
void _hlt();
extern void tickInterrupt();
int createProcessPriority(const char * name,void * func, int argc, char * argv[],int priority);

uint64_t processMemory[MAX_PROCESS][STACK_SIZE]={{0}};

static pcb proceses[MAX_PROCESS] = {0};
static pcb * active_processes[PRIORITY_LEVELS][MAX_PROCESS]={{0}};
static pcb * waiting_processes[PRIORITY_LEVELS][MAX_PROCESS]={{0}};
static int pid_list[MAX_PROCESS]={0};
static int number_of_proceses[PRIORITY_LEVELS]={0};
static int number_of_proceses_snapshot[PRIORITY_LEVELS]={0};

static int active_process_pid=-1;
static int curr_priority=PRIORITY_LEVELS-1;
static int curr_index=0;
static int next_priority=PRIORITY_LEVELS-1;
static int next_index=0;
static int waiting_index[PRIORITY_LEVELS]={0};
static int scheduler_initialized=0;


void reset_processes(){ //reacomoda la lista de active y waiting
    for (int i = 0; i < PRIORITY_LEVELS; i++)
    {   
        int k=0;
        for (int j = 0; j < MAX_PROCESS; j++)
        {
            if (waiting_processes[i][j]!=NULL)
            {
                active_processes[i][k]=waiting_processes[i][j]; //copio los procesos no nulos
                waiting_processes[i][j]=NULL;
                k++;
            }
        }
        while (k<MAX_PROCESS)
        {
            active_processes[i][k++]=0; //limpio lo que quede poniendo 0
        }
        
    }
    for (int i = 0; i < PRIORITY_LEVELS; i++) //reacomodar las cantidades de procesos
    {
        number_of_proceses[i]=waiting_index[i];
        waiting_index[i]=0;
        number_of_proceses_snapshot[i]=number_of_proceses[i];
    }
    
    int found=0;
    for (int i = 0; i < PRIORITY_LEVELS && !found; i++)   //reacomodar el current
    {
        for (int j = 0; j < number_of_proceses[i]&& !found; j++)
        {
            if (active_processes[i][j]->pid==active_process_pid)
            {
                curr_priority=i;
                curr_index=j;
                found=1;
            }
            
        }
        
    }
    if (found==0)
    {
        //something has gone terribly wrong
    }
    
    
}
int getpid() {
    return active_process_pid;
}

int getNewPid(){
    for (int i = 0; i < MAX_PROCESS; i++) //retorna el primer pid libre
    {
        if (pid_list[i]==0)
        {
            pid_list[i]++;
            return i; 
        }
        
    }
    return -1;
}
void freePid(int pid){
    if(! (pid<0||pid>=MAX_PROCESS)) //libera el pid si es un pid razonable
    {
        pid_list[pid]=0;
    }
    
}


void initializeScheduler(){
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        proceses[i].state=KILLED;
    }
}

pcb * freeProcess(){
    for (int i = 0; i < MAX_PROCESS; i++)
    {
        if (proceses[i].state==KILLED)
        {
            return &proceses[i];
        }
    }
    return NULL; //si no encontro ningun proceso disponible
}
int total_processes(){ //ojo no cuenta bien los que estan waiting
    int suma=0;
    for (int i = 0; i < PRIORITY_LEVELS; i++)
    {
        suma+=number_of_proceses[i];
    }
    return suma;
    
}

uint64_t * swap(uint64_t * rsp) {
    if (scheduler_initialized==0)
    {
        initializeScheduler();
        scheduler_initialized++;
    }
    
    int aux_total=total_processes();
    if ( aux_total== 0) { //chequea caso no proceso(porbablemente se reemplaze por tirar a un proceso halt predefinido)
        return rsp;
    }

   
    
    if (active_process_pid!=-1) //ignora el caso de no proceso
    {
        active_processes[curr_priority][curr_index]->rsp = rsp;    // guardo el contexto del proceso actual
        
        waiting_processes[curr_priority][waiting_index[curr_priority]]=active_processes[curr_priority][curr_index]; //lo paso al waiting list
        waiting_index[curr_priority]++;
        active_processes[curr_priority][curr_index]=NULL;//lo pongo en null
    }
    
    

    int cant=0;
    int found=0;
    
    while (!found) //agregar contador para salir del while si no hay procesos
    {
        if (curr_index==number_of_proceses_snapshot[curr_priority]) {  //se si tiene que cambiar de prioridad y de hacer el reset o no
            next_index = 0;
            if (curr_priority==PRIORITY_LEVELS-1)
            {
                next_priority=0;
                reset_processes();
            }
            else
            {
                next_priority++;
            }
            
        }
        else
        {
            next_index++;
        }
        if (number_of_proceses_snapshot[next_priority]>0&&active_processes[next_priority][next_index]!=NULL)
        {
            if (active_processes[next_priority][next_index]->state==ACTIVE) //found the next active process
            {
                found=1;
            }
            else
            {   
                if (active_processes[next_priority][next_index]->state!=KILLED)
                {
                      //lo paso al waiting list
                    waiting_processes[next_priority][waiting_index[next_priority]]=active_processes[next_priority][next_index]; 
                    waiting_index[next_priority]++;
                    active_processes[next_priority][next_index]=NULL;
                }
                else //si esta killed, borro la referencia
                {
                    active_processes[next_priority][next_index]=NULL;
                }
                
            }
        }
        curr_index=next_index;
        curr_priority=next_priority;
        cant++;
    }
     
    //no entendi que hacia el codigo de abajo
    
    

    // if (cant >= MAX_PROCESS) 
    //     _hlt();

    

    curr_priority=next_priority;
    curr_index=next_priority;

    active_process_pid=active_processes[next_priority][next_index]->pid;

    return active_processes[next_priority][next_index]->rsp;   // retorno el puntero del stack del proceso a switchear
}
int createProcess(const char * name,void * func, int argc, char * argv[]){
    return createProcessPriority(name,func,argc,argv,0);
}
int createProcessPriority(const char * name,void * func, int argc, char * argv[],int priority) {
    if (priority<=0||priority>=PRIORITY_LEVELS)
    {
        //error handling de prioridades
    }
    
    
    
    

    pcb * newProcess = freeProcess();

    if (newProcess==NULL)
    {
        // hay que meter error handling para cuando quiere instanciar mas procesos que el max
    }
    

    newProcess->pid = getNewPid();
    if (newProcess->pid==-1)
    {
        //error handling
    }
                               
    newProcess->function = func;
    newProcess->state = ACTIVE;
    newProcess->name = name;
    newProcess->priority=priority;

    newProcess->mallocPos = processMemory[newProcess->pid];
    newProcess->rsp = newProcess->mallocPos + STACK_SIZE;

    newProcess->rsp = initializeStack(newProcess->rsp, wrapper, newProcess->function, argc, argv, newProcess->pid); // retorna el rsp luego de hacer los push

    waiting_processes[priority][waiting_index[priority]]=newProcess; //pongo el nuevo proceso en la lista de procesos para la proxima ejecucion
    waiting_index[priority]++;

    
    number_of_proceses[priority]++;
    return newProcess->pid;
}

void wrapper(void * func(int, char **), int argc, char * argv[], int pid) {
    
    (*func)(argc, argv);
    
    kill(pid);
}


int kill(int pid) {
    
    int found=0;
    for (int i = 0; i < MAX_PROCESS && !found; i++)
    {
        if (proceses[i].pid==pid)
        {
            proceses[i].state=KILLED;
            freePid(pid);
            number_of_proceses[(int)proceses[i].priority]--;
            found=1;
            if (pid == active_process_pid) 
            tickInterrupt();
        }
        
    }
    return found == 0 ? -1 : 0;

}

int block(int pid) {
    int found=0;
    for (int i = 0; i < MAX_PROCESS && !found; i++)
    {
        if (proceses[i].pid==pid && proceses[i].state!=KILLED)
        {
            proceses[i].state=BLOCKED;
            found=1;
            if (pid == active_process_pid) 
            tickInterrupt();
        }
        
    }
    return found == 0 ? -1 : 0;
}

int unblock(int pid) {
    int found=0;
    for (int i = 0; i < MAX_PROCESS && !found; i++)
    {
        if (proceses[i].pid==pid && proceses[i].state==BLOCKED)
        {
            proceses[i].state=ACTIVE;
            found=1;
        }   
    }
    return found == 0 ? -1 : 0;
}