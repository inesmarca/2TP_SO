#include <stdint.h>

uint64_t * swap(uint64_t * rsp);

int createProcess(const char * name, void * func, int argc, char * argv[]);

int kill(int pid);

int block(int pid);

int unblock(int pid);

int getpid();