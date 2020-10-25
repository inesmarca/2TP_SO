#ifndef __LIB_FUNC_H_
#define __LIB_FUNC_H_

void triggerException0();

void printTime();

void printTemperature();

void printmem(char * parameters);

void inforeg();

void printCPUInfo();

extern void triggerException6();

void printError(char * string);

void printBlock();

void loop();

void mem();

int createBackground(const char * name, void * func, int priority,int fd[], int argc, char * argv[]);

int createForeground(const char * name, void * func, int priority,int fd[], int argc, char * argv[]);

void filter();

void cat();

void ps();

void wc();

void sem();

void philo();

void pipeInfo();

void nice_shell(int argc,char * argv[]);

void kill_shell(int argc,char *argv[]);
   
void block_shell(int argc,char *argv[]);
    
void unblock_shell(int argc,char *argv[]);

void testNamedPipes();
   

#endif