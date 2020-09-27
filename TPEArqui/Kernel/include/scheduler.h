#include <stdint.h>

uint64_t * swap(uint64_t * rsp);

void createProcess(void * func, int argc, char * argv[]);

void kill(int pid);

void block(int pid);

void unblock(int pid);