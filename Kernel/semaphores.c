#include <semaphores.h>
semaphore * list[MAX_SIZE];
int size = 0;

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
	sem -> value++; 
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
}

int sem_close(semaphore * sem){
	return sem_rem(sem);
}
