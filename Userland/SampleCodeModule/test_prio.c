#include <stdint.h>
#include <sysLib.h>
#include <libC.h>

#define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
#define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice

static void endless_loop();

static uint64_t my_getpid(){
  return getpid();
}

static uint64_t my_create_process(char * name){
  int fd[2] = {-1, 1};
  return createBackground(name, endless_loop, 0, fd, 0, 0);
}

static uint64_t my_nice(uint64_t pid, uint64_t newPrio){
  return nice(pid, newPrio);
}

static uint64_t my_kill(uint64_t pid){
  return kill(pid, 0);
}

static uint64_t my_block(uint64_t pid){
  return kill(pid, 1);
}

static uint64_t my_unblock(uint64_t pid){
  return kill(pid, 2);
}

void bussy_wait(uint64_t n){
  uint64_t i;
  for (i = 0; i < n; i++);
}

static void endless_loop(){
  uint64_t pid = my_getpid();

  while(1){
    printf("%d ",pid);
    bussy_wait(MINOR_WAIT);
  }
}

#define TOTAL_PROCESSES 3

void test_prio(){
  uint64_t pids[TOTAL_PROCESSES];
  uint64_t i;

  for(i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = my_create_process("endless_loop");

  bussy_wait(WAIT);
  printf("\nCHANGING PRIORITIES...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        my_nice(pids[i], 0); //lowest priority 
        break;
      case 1:
        my_nice(pids[i], 1); //medium priority
        break;
      case 2:
        my_nice(pids[i], 2); //highest priority
        break;
    }
  }

  bussy_wait(WAIT);
  printf("\nBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++) {
    if (my_block(pids[i]) == -1)
      printf("Error BLOCK");
  }

  printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
  for(i = 0; i < TOTAL_PROCESSES; i++){
    switch (i % 3){
      case 0:
        if (my_nice(pids[i], 1) == -1) //medium priority 
           printf("Error NICE");
        break;
      case 1:
        if (my_nice(pids[i], 1) == -1) //medium priority
           printf("Error NICE");
        break;
      case 2:
        if (my_nice(pids[i], 1) == -1) //medium priority
           printf("Error NICE");
        break;
    }
  }

  printf("UNBLOCKING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(WAIT);
  printf("\nKILLING...\n");

  for(i = 0; i < TOTAL_PROCESSES; i++) {
    if (my_kill(pids[i]) == -1) 
      printf("Error al matar");
  }
  printf("\nFinished Test\n");
}