#include <pipes.h>
#include <semaphores.h>
#include <scheduler.h>
#include <defs.h>
#include <lib.h>

typedef struct pipe_t {
    int alive;
    int closed_ends[2];
    int lock;
    char data[PIPE_SIZE];
    int nread;                //bytes read
    int nwrite;                //bytes write
    int pids[2];
} pipe_t;

pipe_t pipes[MAX_PIPES];
int cant_pipes;

int getFirstPipe() {
    for (int i = 0; i < cant_pipes; i++) {
        if (pipes[i].alive == 0)
            return i;
    }
    return cant_pipes;
}

int pipe(int fd[]) {
    if (cant_pipes == MAX_PIPES)
        return -1;
    
    int index = getFirstPipe();
    pipes[index].alive = 1;
    pipes[index].closed_ends[0] = 2;
    pipes[index].closed_ends[1] = 2;
    pipes[index].lock = 0;
    pipes[index].nread = 0;
    pipes[index].nwrite = 0;
    pipes[index].pids[0] = -1;
    pipes[index].pids[1] = -1;
    memset(pipes[index].data, -1, PIPE_SIZE);

    fd[0] = index*2 + 2;
    fd[1] = index*2 + 3;
    return 0;
}

pipe_t * getPipe(int fd) {
    int index = (fd - 2) / 2;
    return &pipes[index];
}

int writePipeBuff(pipe_t * p, const char * string, int n) {
    int i;
    for (i = 0; i < n && string[i] != 0; ++i) {
        p->data[p->nwrite++ % PIPE_SIZE] = string[i];
    }
    if (string[i] == 0)
        p->data[p->nwrite % PIPE_SIZE] = string[i++];

    return i;
}

int pipewrite(int fd, const char * string, int n) {
    pipe_t * p = getPipe(fd);

    int cant;
    int avail_space;
    int dim = 0;

    while (n > 0 && *string != 0) {
        acquire(&(p -> lock));
        p->pids[0] = -1;
        avail_space = (p->nread + PIPE_SIZE) - p->nwrite;

        if (avail_space > n)
            avail_space = n;

        if (avail_space != 0) {
            cant = writePipeBuff(p, string, avail_space);
            string += cant;
            n -= cant;
            dim += cant;

            if (p->pids[1] != -1) {
                unblock(p->pids[1]);
            }

            release(&(p->lock));
        } else {
            release(&(p->lock));
            p->pids[0] = getpid();
            block(getpid());
        }
    }
    
    return dim;
}

int readPipeBuff(pipe_t * p, char * buff, int cant) {
    int i;
    for (i = 0; i < cant && p->data[p->nread % PIPE_SIZE] != 0; i++) {
        buff[i] = p->data[p->nread++ % PIPE_SIZE];
        buff[i + 1] = 0;
    }

    if (i < cant) {
        buff[i++] = 0;
        p->nread++;
    }

    return i;
}

int piperead(int fd, char * buff, int n){
    pipe_t * p = getPipe(fd);

    int avail_read;
    int cant;
    int dim = 0;

    while (n != 0 && p->data[p->nread % PIPE_SIZE] != 0) {
        acquire(&(p -> lock));
        p->pids[1] = -1;
        avail_read = p->nwrite - p->nread;

        if (avail_read > n)
            avail_read = n;

        if (avail_read <= 0) {
            release(&(p->lock));
            p->pids[1] = getpid();
            block(getpid());
        } else {
            cant = readPipeBuff(p, buff, avail_read);
            buff += cant;
            n -= cant;
            dim += cant;

            if (p->pids[0] != -1) {
                unblock(p->pids[0]);
            }
                
            release(&(p->lock));
        }
    }

    return dim;
}

int pipe_close(int fd) {
    pipe_t * p = getPipe(fd);

    if (p->alive == 0 || p == NULL)
        return -1;
        
    int index = fd % 2;

    if (p->closed_ends[index] != 0)
        p->closed_ends[index]--;
    else 
        return -1;

    if (p->closed_ends[0] == 0 && p->closed_ends[1] == 0)
        p->alive = 0;

    return 0;
}
