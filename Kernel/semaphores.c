#include <semaphores.h>
#include <lib.h>
#include <sysCall.h>

static semaphore * list[MAX_SIZE];
static semaphore sems[MAX_SIZE];
static char used[MAX_SIZE];        //Indica semaforos libres y ocupados
static int size = 0;

int sem_size(){
	return size;
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
static int sem_add(semaphore * sem){					//retorna 0 si no agrega, 1 si agrega 
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

static int sem_rem(semaphore * sem){
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


int sem_init(semaphore * sem, int pshare, int value){
	sem -> value = value;
	sem -> pshare = pshare;
	sem -> semid = sem_add(sem);
	if (sem -> semid == -1)
	{
		return sem -> semid;
	}
	return 0;
}

int sem_post(semaphore * sem){
	return ++sem -> value; 
}

int sem_wait(semaphore * sem){
	if (sem -> value > 0)
	{
		sem -> value--;
	}
	if (sem -> value == 0)
	{
			//LOCK
	}
    return sem -> value;
}

int sem_close(semaphore * sem){
	return sem_rem(sem);
}

semaphore * getSem(){
    for (int i = 0; i < MAX_SIZE; i++)
    {
        if (used[i] == 0)
        {
            return &sems[i];
        }
    }
    return NULL;
}


semaphore * sem_open(char * semName, char createFlag, int value){   
    semaphore * sem = getSem();
    switch (createFlag) {
    case 0:
        if (sem == NULL) return NULL;
        sem -> name = semName;    
        if (sem_init(sem,1, value) == 0) {
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
        return NULL;        
        break;
    default:
        return NULL;
        break;
    }   
}