#include <pipes.h>
#include <semaphores.h>
#include <scheduler.h>
#include <defs.h>

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

    fd[0] = index*2 + 2;
    fd[1] = index*2 + 3;
    return 0;
}

pipe_t * getPipe(int fd) {
    int index = (fd - 2) / 2;
    return &pipes[index];
}

void writePipeBuff(pipe_t * p, const char * string, int n) {
    for (int i = 0; i < n; ++i) {
        p->data[p->nwrite++ % PIPE_SIZE] = string[i];
    }
}

int pipewrite(int fd, const char * string, int n) {
    pipe_t * p = getPipe(fd);

    int avail_space;
    while (n != 0) {
        acquire(&(p -> lock));
        avail_space = (p->nread + PIPE_SIZE) - p->nwrite;
        if (avail_space > n)
            avail_space = n;

        if (avail_space == 0) {
            release(&(p->lock));
            p->pids[0] = getpid();
            block(getpid());
        }
        writePipeBuff(p, string, avail_space);
        string += avail_space;
        n -= avail_space;

        if (p->pids[1] != -1)
            unblock(p->pids[1]);

        release(&(p->lock));
    }
    return n;
}

void readPipeBuff(pipe_t * p, char * buff, int cant) {
    for (int i = 0; i < cant; i++) {
        buff[i] = p->data[p->nread++ % PIPE_SIZE];
    }
}

int piperead(int fd, char * string, int n){
    pipe_t * p = getPipe(fd);

    int avail_read;
    while (n != 0) {
        acquire(&(p -> lock));
        avail_read = p->nwrite - p->nread;

        if (avail_read > n)
            avail_read = n;

        if (avail_read <= 0) {
            release(&(p->lock));
            p->pids[1] = getpid();
            block(getpid());
        }

        readPipeBuff(p, string, avail_read);
        string += avail_read;
        n -= avail_read;

        if (p->pids[0] != -1)
            unblock(p->pids[0]);
            
        release(&(p->lock));
    }
    return n;
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
