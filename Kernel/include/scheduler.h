#include <stdint.h>
#define MAX_PROCESS 20
#define PRIORITY_LEVELS 3
#define STACK_SIZE 1024 * 4 

uint64_t * swap(uint64_t * rsp);

int createProcess(const char * name, void * func, int priority, int argc, void * argv[]);

int kill(int pid);

int block(int pid);

int unblock(int pid);

int getpid();

int nice(int pid, int priority);

void yield();