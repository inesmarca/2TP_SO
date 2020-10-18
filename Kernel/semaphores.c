#include <semaphores.h>
#include <lib.h>
#include <consoleManager.h>
#include <defs.h>


extern int _xchg(int * lock, int value);

static sem_t sems[MAX_SIZE];
static int active_sems = 0;

int getListSem(infoSem * buff[]) { // FALTA TESTING
	int index = 0;
	int active_sems = 0;
	for (int i = 0; i < MAX_SIZE; i++)
	{
		if (sems[i].semid != -1)
		{
			active_sems++;
			strcpy(buff[index]->name, sems[i].name);
			buff[index]->value = sems[i].value;
			for (int j = 0; j < sems[i].cant_blocked_pids; j++)	{
				buff[index]->blocked_pids[j] = sems[i].blocked_pids[j];
				buff[index]->cant_blocked++;
			}
			index++;
		}		
	}
	return active_sems;
}

void acquire(int * lock) {
	while(_xchg(lock, 1) != 0);
}

void release(int * lock) {
	_xchg(lock, 0);
}

void initializeSems() {
	for (int i = 0; i < MAX_SIZE; i++)
		sems[i].semid = -1;
}

static void sem_init(int index, int value){
	sems[index].value = value;
	sems[index].semid = index;
	sems[index].lock = 0; 
	sems[index].cant_blocked_pids = 0;
	memset(sems[index].blocked_pids, -1, MAX_PROCESS);
	active_sems++;
}

sem_t * semExists(char * semName) {
	for (int i = 0; i < MAX_SIZE; i++) {
		if (strcmp(sems[i].name, semName))
			return &sems[i];		     
	}

	return NULL;
}

void addPidToSem(sem_t * sem, int pid) {
	sem->pids[sem->cant_pids++] = pid;
}

static int getSem(){
	if (active_sems == MAX_SIZE)
		return -1;

    for (int i = 0; i < MAX_SIZE; i++) {
        if (sems[i].semid == -1)
            return i;
    }

    return -1;
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

			int index = getSem();
			if (index == -1) {
				print("Error in index ", 17);
				return NULL;
			} 

			strcpy(sems[index].name, semName);
			sem_init(index, value);

			sem = &sems[index];
			addPidToSem(sem, getpid());
			break;
		case 1:
			sem = semExists(semName);
			if (sem != NULL) {
				addPidToSem(sem, getpid());
			} else {
				print("Error flag 1 ", 17);
			}
			break;     
		default:
			return NULL;
			break;
    }   
	
	return sem; 
}

void wakeup(sem_t * sem) {
	for (int i = 0; i < sem->cant_blocked_pids; i++) {
		unblock(sem->blocked_pids[i]);
		sem->blocked_pids[i] = -1;
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
			sem->blocked_pids[sem->cant_blocked_pids] = getpid();
			sem->cant_blocked_pids++;
			release(&(sem->lock));
			block(getpid());
		}
	}
}

int sem_close(sem_t * sem){
	if (sem == NULL || sem->semid == -1)
		return -1;

	if (sem->cant_pids == 1) {
		sem->semid = -1;
		active_sems--;
	} else {
		sem->cant_pids--;
	}
	return 0;
}

