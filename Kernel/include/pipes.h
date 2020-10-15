#ifndef __PIPES_H_
#define __PIPES_H_

#define MAX_PIPES 10
#define PIPE_SIZE 255

int pipe(int fd[]);

int pipewrite(int fd, const char * string, int n);

int piperead(int fd, char * string, int n);

int pipe_close(int fd);

#endif