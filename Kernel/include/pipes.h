#include <scheduler.h>
#ifndef __PIPES_H_
#define __PIPES_H_

#define MAX_PIPES 10
#define PIPE_SIZE 255
#define MAX_PROCESS 20

typedef struct infoPipe {
    int id;
    int nread;
    int nwrite;
    int write_blocked[MAX_PROCESS];
    int read_blocked[MAX_PROCESS];
} infoPipe;

int pipe(int fd[]);

int pipewrite(int fd, const char * string, int n);

int piperead(int fd, char * string, int n);

int pipe_close(int fd);

void initializePipes();

int getPipeList(int * buff);

int getPipeInfo(int id, infoPipe * buff);

void insertPidToPipe(int fd, int pid);

void removePidFromPipe(int fd, int pid);

int mkfifo(char * name);

int open(char * name, int flag);

#endif