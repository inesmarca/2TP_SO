#include <semaphores.h>

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

/*
void see_sems(){
	printf("IMPRIMO LOS SEMS\n");
	for (int i = 0; i < size; ++i)
	{
		printf("%d\n", list[i] -> semid);
	}
	printf("//////////////////////////////////\n");

}*/

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

/*
static int sem_rem(sem_t * sem){
	for (int i = 0; i < MAX_SIZE; ++i)
	{
		if (list[i] -> semid == sem -> semid)
		{
			list[i] -> semid = -1;
			size--;
			return 0;	
		}
	}
	return -1;
}
*/

static int sem_init(sem_t * sem, int value){
	sem -> value = value;
	sem -> semid = sem_add(sem);
	sem -> lock = 0;
	sem -> cant = 0;
	if (sem -> semid == -1)
	{
		return sem -> semid;
	}
	return 0;
}

void wakeup(sem_t * sem) {
	for (int i = 0; i < sem->cant; i++) {
		unblock(sem->pids[i]);
	}
	sem->cant = 0;
}

int sem_post(sem_t * sem){
	if (sem == NULL)
		return -1;
    acquire(&(sem->lock));

	sem->value++;
	wakeup(sem);

	release(&(sem->lock));
	return 0;
}

int sem_wait(sem_t * sem){
	if (sem == NULL)
		return -1;
	acquire(&(sem->lock));
	if (sem->value > 0) {
		sem->value--;
	} else {
		release(&(sem->lock));
		sem->pids[sem->cant] = getpid();
		block(getpid());
		acquire(&(sem->lock));
		sem->value--;
	}

	release(&(sem->lock));
	return 0;
}

int sem_close(sem_t * sem){
	if (sem == NULL)
		return -1;
	sem -> semid = -1;
	size--;
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

sem_t * sem_open(char * semName, char createFlag, int value){   
    switch (createFlag) {
    case 0:;
        sem_t * sem = getSem();
        if (sem == NULL) return NULL;
        sem -> name = semName;    
        if (sem_init(sem, value) == 0) {
            return sem;
        }
        return NULL;
        break;
    case 1:
        for (int i = 0; i < MAX_SIZE; i++) {
            if (list[i] -> name == semName)
            {
                return list[i];
            }                
        }
        return sem_open(semName, 0, value);        
        break;
    default:
        return NULL;
        break;
    }   
}