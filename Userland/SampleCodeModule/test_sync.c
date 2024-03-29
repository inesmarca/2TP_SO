// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <test_util.h>
#include <libC.h>
#include <sysLib.h>

//void inc(uint64_t sem, int64_t value, uint64_t N);
void inc(int argc, char * argv[]);

#define TOTAL_PAIR_PROCESSES 2
#define SEM_ID "sem"

int64_t global;  //shared memory
int cant = 0;

void slowInc(int64_t * p, int64_t inc) {
  int64_t aux = *p;
  aux += inc;
  yield();
  *p = aux;
}


uint64_t my_create_process(char * name, int sem, int value, int N){ //crea un procesos que altera la shared mem
  char ** buff = malloc(3);
  if (buff == NULL) {
    return -1;
  }
  
	if ((buff[0] = malloc(20)) == NULL){
    free(buff);
    return -1;
  }
    
	if ((buff[1] = malloc(20)) == NULL){
    free(buff[0]);
    free(buff);
    return -1;
  }
    
  if ((buff[2] = malloc(20)) == NULL){
    free(buff[0]);
    free(buff[1]);
    free(buff);
    return -1;
  }
    

	itoa(sem, buff[0], 10);
	itoa(value, buff[1], 10);
	itoa(N, buff[2], 10);

  int fd[MAX_PROCESS];
	memset(fd, -1, MAX_PROCESS);
	fd[0] = STDIN;
	fd[1] = STDOUT;
  
  return createBackground(name, inc, 0, fd, 3, buff);
}

// inc
void inc(int argc, char ** argv){  
  uint64_t i;

  int semValue = atoi(argv[0]);
  int value = atoi(argv[1]);
  int N = atoi(argv[2]);
  
  sem_t * semap = sem_open(SEM_ID, 0, 1);         //creo el sem
  if (semap == NULL){
    printf("ERROR OPENING SEM\n"); 
    return;
  }
    

  for (i = 0; i < N; i++){
    if (semValue) sem_wait(semap);
      slowInc(&global, value);                            //incremento la mem
    if (semValue) sem_post(semap);
  }

  if (semValue) sem_close(semap);                         //cierro el sem
  
  printf("Finished: %d\n", global);

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
    cant++;
  }                                                 //arg3: cantidad de veces que cuenta
  kill(0, 0);
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
  kill(0, 0);
}