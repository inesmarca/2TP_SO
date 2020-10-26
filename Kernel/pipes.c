// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <pipes.h>
#include <semaphores.h>
#include <scheduler.h>
#include <defs.h>
#include <lib.h>
#include <sysCall.h>
#include <consoleManager.h>

#define R_ONLY 0
#define W_ONLY 1

typedef struct pipe_t {
    char name[255];
    int alive;
    int lock;
    char data[PIPE_SIZE];
    int nread;                //bytes read
    int nwrite;                //bytes write
    int write_blocked[MAX_PROCESS];
    int cant_write;
    int read_blocked[MAX_PROCESS];
    int cant_read;
    int pids[MAX_PROCESS];
    int cant_pids;
    int fd[2];
} pipe_t;

pipe_t pipes[MAX_PIPES];
int cant_pipes;

pipe_t * getPipe(int fd) {
    int index = (fd - 2) / 2;
    return &pipes[index];
}

void initializePipes() {
    for (int i = 0; i < MAX_PIPES; i++) {
        pipes[i].alive = 0;
    }
}

// getPipeList
int getPipeList(int * buff) {
    int cant = 0;
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i].alive != 0) {
            buff[cant++] = i;
        }
    }

    return cant;
}

// getInfoPCB
int getPipeInfo(int id, infoPipe * buff) {

    if (id < 0 || id >= MAX_PIPES)
        return -1;

    buff->id = id;
    buff->nread = pipes[id].nread;
    buff->nwrite = pipes[id].nwrite;

    int pos_read = 0;
    int pos_write = 0;
    memset(buff->write_blocked, -1, MAX_PROCESS);
    memset(buff->read_blocked, -1, MAX_PROCESS);
    for (int j = 0; j < MAX_PROCESS; j++) {
        buff->write_blocked[pos_write++] = pipes[id].write_blocked[j];
        buff->read_blocked[pos_read++] = pipes[id].read_blocked[j];
    }
        
    return 0;
}

int getFirstPipe() {
    if (cant_pipes == MAX_PIPES)
        return -1;

    for (int i = 0; i < cant_pipes; i++) {
        if (pipes[i].alive == 0)
            return i;
    }
    return cant_pipes;
}

void insertPidToPipe(int fd, int pid) {
    pipe_t * pipe = getPipe(fd);
    pipe->pids[pid] = 1;
    pipe->cant_pids++;
}

void removePidFromPipe(int fd, int pid) {
    pipe_t * pipe = getPipe(fd);
    if (pipe->alive == 0 || pipe->pids[pid] != 1)
        return;

    pipe->pids[pid] = 0;
    pipe->cant_pids--;
    if (pipe->cant_pids == 0) {
        pipe->alive = 0;
        cant_pipes--;
    }   
}

int pipeExists(char * name) {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipes[i].alive != 0 && strcmp(pipes[i].name, name)) {
            return i;
        }
    }
    return -1;
}

void pipeInit(int index, char * name) {
    pipes[index].alive = 1;
    pipes[index].lock = 0;
    pipes[index].nread = 0;
    pipes[index].nwrite = 0;
    pipes[index].cant_read = 0;
    pipes[index].cant_write = 0;
    memset(pipes[index].write_blocked, -1, MAX_PROCESS);
    memset(pipes[index].read_blocked, -1, MAX_PROCESS);
    memset(pipes[index].data, -1, PIPE_SIZE);
    pipes[index].cant_pids = 0;
    strcpy(pipes[index].name, name);

    pipes[index].fd[0] = index*2 + 2;
    pipes[index].fd[1] = index*2 + 3;
    
    cant_pipes++;
}

int mkfifo(char * name) {
    if (pipeExists(name) != -1) {
        int index = getFirstPipe();
        if (index == -1)
            return -1;
        pipeInit(index, name);
    }

    return 0;
}

int open(char * name, int flag) {
    int id;
    if ((id = pipeExists(name)) == -1) 
        return -1;

    insertPidToPipe(pipes[id].fd[0], getpid());
    if (flag == R_ONLY) {
        return changeFd(pipes[id].fd[0], getpid());
    } else if (flag == W_ONLY) {
        return changeFd(pipes[id].fd[1], getpid());
    }

    return -1;
}

int pipe(int fd[]) {
    int index = getFirstPipe();
    if (index == -1)
        return -1;
    pipeInit(index, "");
    fd[0] = index*2 + 2;
    fd[1] = index*2 + 3;

    return 0;
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

void wakeupRead(pipe_t * p) {
    for (int i = 0; i < p->cant_read; i++) {
        unblock(p->read_blocked[i]);
    }
    p->cant_read = 0;
}

int pipewrite(int fd, const char * string, int n) {
    pipe_t * p = getPipe(fd);

    int cant;
    int avail_space;
    int dim = 0;

    while (n > 0 && *string != 0) {
        acquire(&(p -> lock));
        avail_space = (p->nread + PIPE_SIZE) - p->nwrite;

        if (avail_space > n)
            avail_space = n;

        if (avail_space != 0) {
            cant = writePipeBuff(p, string, avail_space);
            string += cant;
            n -= cant;
            dim += cant;

            if (p->cant_read != 0) {
                wakeupRead(p);
            }

            release(&(p->lock));
        } else {
            release(&(p->lock));
            p->write_blocked[p->cant_write++] = getpid();
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

void wakeupWrite(pipe_t * p) {
    for (int i = 0; i < p->cant_write; i++) {
        unblock(p->write_blocked[i]);
    }
    p->cant_write = 0;
}

int piperead(int fd, char * buff, int n){
    pipe_t * p = getPipe(fd);

    int avail_read;
    int cant;
    int dim = 0;

    while (n != 0 && p->data[p->nread % PIPE_SIZE] != 0) {
        acquire(&(p -> lock));
        avail_read = p->nwrite - p->nread;

        if (avail_read > n)
            avail_read = n;

        if (avail_read <= 0) {
            release(&(p->lock));
            p->read_blocked[p->cant_read++] = getpid();
            block(getpid());
        } else {
            cant = readPipeBuff(p, buff, avail_read);
            buff += cant;
            n -= cant;
            dim += cant;

            if (p->cant_write != 0) {
                wakeupWrite(p);
            }
                
            release(&(p->lock));
        }
    }
    return dim;
}
