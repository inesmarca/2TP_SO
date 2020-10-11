#include <test_util.h>
#include <libC.h>
#include <semaphores.h>

//void inc(uint64_t sem, int64_t value, uint64_t N);
int incMain(int argc, char ** argv);
int decMain(int argc, char ** argv);
int incMainNo(int argc, char ** argv);
int decMainNo(int argc, char ** argv);

uint64_t my_create_process_inc(char * name, int sem, int value, int N){ //crea un procesos que altera la shared mem
  if (sem)
  {
    return create(name, incMain, 0, 0, 0);
  }
  return create(name, incMainNo, 0, 0, 0);
  
}

uint64_t my_create_process_dec(char * name, int sem, int value, int N){ //crea un procesos que altera la shared mem
  if (sem)
  {
      return create(name, decMain, 0, 0, 0);
  }
  return create(name, decMainNo, 0, 0, 0);
}

semaphore * my_sem_open(char *sem_id, uint64_t initialValue){
  return sem_open(sem_id, 0, initialValue);
}

uint64_t my_sem_wait(semaphore * sem){
  return sem_wait(sem);
}

uint64_t my_sem_post(semaphore * sem){
  return sem_post(sem);
}

uint64_t my_sem_close(semaphore * sem){
  return sem_close(sem);
}

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

int64_t global;  //shared memory
void yield(){
    for (int i = 0; i < 1000; i++)
    {
    }
    
}

void slowInc(int64_t *p, int64_t inc){
  int64_t aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

void inc(uint64_t sem, int64_t value, uint64_t N){  
  uint64_t i;
    
    semaphore * semap = my_sem_open(SEM_ID, 1);         //creo el sem
    if (sem && semap == NULL)
        printf("ERROR OPENING SEM\n");
    
  
  for (i = 0; i < N; i++){
    if (sem) my_sem_wait(semap);
    slowInc(&global, value);                            //incremento la mem
    if (sem) my_sem_post(semap);
  }

  if (sem) my_sem_close(semap);                         //cierro el sem
  
  printf("Final value: %d\n",global);
}

void test_sync(){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process_inc("incMain", 1, 1, 1000000);        //arg1: flag que indica si usa sem, 
    my_create_process_dec("decMain", 1, -1, 1000000);       //arg2: incremento que usa la func. inc
    printf("PROCESSES CREATED");
 
  }                                                 //arg3: cantidad de veces que cuenta
}

void test_no_sync(){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITHOUT SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process_inc("incMainNo", 0, 1, 1000000);
    my_create_process_dec("decMainNo", 0, -1, 1000000);
  }
}

int incMain(int argc, char ** argv){
    inc(1, 1, 100000);
    return 0;
}
int decMain(int argc, char ** argv){
    inc(1, -1, 100000);
    return 0;
}


int incMainNo(int argc, char ** argv){
    inc(0, 1, 100000);
    return 0;
}
int decMainNo(int argc, char ** argv){
    inc(0, -1, 100000);
    return 0;
}

/*
int main(){
  test_sync();
  return 0;
}
*/