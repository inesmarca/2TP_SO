#include <test_util.h>
#include <libC.h>
#include <sysLib.h>

//void inc(uint64_t sem, int64_t value, uint64_t N);
void inc(int argc, char * argv[]);

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

int64_t global;  //shared memory

void slowInc(int64_t * p, int64_t inc) {
  int64_t aux = *p;
  aux += inc;
  yield();
  *p = aux;
}

uint64_t my_create_process(char * name, int sem, int value, int N){ //crea un procesos que altera la shared mem
  char ** buff = malloc(3);
	if ((buff[0] = malloc(20)) == NULL)
    return -1;
	if ((buff[1] = malloc(20)) == NULL)
    return -1;
  if ((buff[2] = malloc(20)) == NULL)
    return -1;

	itoa(sem, buff[0], 10);
	itoa(value, buff[1], 10);
	itoa(N, buff[2], 10);

  int fd[MAX_PROCESS] = {STDIN, STDOUT, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
  
  return createBackground(name, inc, 0, fd, 3, buff);
}

// inc
void inc(int argc, char ** argv){  
  uint64_t i;

  int sem = atoi(argv[0]);
  int value = atoi(argv[1]);
  int N = atoi(argv[2]);
  
  sem_t * semap = sem_open(SEM_ID, 0, 1);         //creo el sem
  if (sem && semap == NULL)
    printf("ERROR OPENING SEM\n");
    
  
  for (i = 0; i < N; i++){
    if (sem) sem_wait(semap);
    slowInc(&global, value);                            //incremento la mem
    if (sem) sem_post(semap);
  }

  if (sem) sem_close(semap);                         //cierro el sem
  
  printf("Finished: %d \n", global);

  // free de los mallocs hechos en create
  free(argv[0]);
  free(argv[1]);
  free(argv[2]);
  free(argv);
}

void test_sync(){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITH SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    if (my_create_process("inc", 1, 1, 1000000) == -1) printf("Error in create");        //arg1: flag que indica si usa sem, 
    if (my_create_process("inc", 1, -1, 1000000) == -1) printf("Error in create");       //arg2: incremento que usa la func. inc
    printf("PROCESSES CREATED\n");
  }                                                 //arg3: cantidad de veces que cuenta
}

void test_no_sync(){
  uint64_t i;

  global = 0;

  printf("CREATING PROCESSES...(WITHOUT SEM)\n");

  for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
    my_create_process("incMainNo", 0, 1, 1000000);
    my_create_process("decMainNo", 0, -1, 1000000);
    printf("PROCESSES CREATED\n");
  }
}