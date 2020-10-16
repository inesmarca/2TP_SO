#include <scheduler.h>
#ifndef __PIPES_H_
#define __PIPES_H_

#define MAX_PIPES 10
#define PIPE_SIZE 255
#define MAX_PROCESS 20

typedef struct infoPipe {
    int nread;
    int nwrite;
    int pids_blocked[MAX_PROCESS];
} infoPipe;

int pipe(int fd[]);

int pipewrite(int fd, const char * string, int n);

int piperead(int fd, char * string, int n);

int pipe_close(int fd);

void initializePipes();

int getPipeList(int * buff);

int getInfoPipe(int id, infoPipe * buff);

#endif