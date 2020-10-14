#include <semaphores.h>
#include <lib.h>
#include <consoleManager.h>

extern int _xchg(int * lock, int value);

static sem_t * list[MAX_SIZE];
static sem_t sems[MAX_SIZE];
static char used[MAX_SIZE];        //Indica semaforos libres y ocupados
static int size = 0;

int sem_size(){
	return size;
}

void acquire(int * lock) {
	while(_xchg(lock, 1) != 0);
}

void release(int * lock) {
	_xchg(lock, 0);
}

static int sem_add(sem_t * sem){					//retorna 0 si no agrega, 1 si agrega 
	if (size == MAX_SIZE)
	{
		return -1;
	}
	for (int i = 0; i < size; ++i)
	{
		if (list[i] -> semid == -1)
		{
			size++;
			list[i] = sem;
			return i;
		}
	}
	list[size++] = sem;
	return size;
}

static int sem_init(sem_t * sem, int value){
	sem -> value = value;
	sem -> semid = sem_add(sem);
	sem -> lock = 0;
	sem -> cant_blocked_pids = 0;
	if (sem -> semid == -1)
	{
		return sem -> semid;
	}
	return 0;
}

void wakeup(sem_t * sem) {
	for (int i = 0; i < sem->cant_blocked_pids; i++) {
		unblock(sem->blocked_pids[i]);
	}
	sem->cant_blocked_pids = 0;
}

int sem_post(sem_t * sem){
	if (sem == NULL || sem->semid == -1)
		return -1;
    acquire(&(sem->lock));

	sem->value++;
	wakeup(sem);

	release(&(sem->lock));
	return 0;
}

int sem_wait(sem_t * sem){
	if (sem == NULL || sem->semid == -1)
		return -1;

	while(1) {
		acquire(&(sem->lock));
		if (sem->value > 0) {
			sem->value--;
			release(&(sem->lock));
			return 0;
		} 
		else {
			release(&(sem->lock));
			sem->blocked_pids[sem->cant_blocked_pids] = getpid();
			sem->cant_blocked_pids++;
			block(getpid());
		}
	}
}

int sem_close(sem_t * sem){
	if (sem == NULL || sem->semid == -1)
		return -1;
	if (sem->cant_pids == 1) {
		sem -> semid = -1;
		size--;
	} else {
		sem->cant_pids--;
	}
	return 0;
}

static sem_t * getSem(){
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (used[i] == 0)
        {
            return &sems[i];
        }
    }
    return NULL;
}

sem_t * semExists(char * semName) {
	for (int i = 0; i < MAX_SIZE; i++) {
		if (list[i] -> name == semName)
		{
			return list[i];				
		}                
	}
	return NULL;
}

void addPidToSem(sem_t * sem, int pid) {
	sem->pids[sem->cant_pids++] = pid;
}

sem_t * sem_open(char * semName, char createFlag, int value){   
	sem_t * sem;
    switch (createFlag) {
		case 0:;
			sem = semExists(semName);
			if (sem != NULL) {
				addPidToSem(sem, getpid());
				return sem;
			}

			sem = getSem();
			if (sem == NULL) return NULL;

			sem -> name = semName;  
			if (sem_init(sem, value) != 0) {
				return NULL;
			}
			addPidToSem(sem, getpid());
			break;
		case 1:
			sem = semExists(semName);
			if (sem != NULL) {
				addPidToSem(sem, getpid());
			} 
			break;     
		default:
			return NULL;
			break;
    }   
	
	return sem; 
}