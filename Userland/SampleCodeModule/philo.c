#include <sem.h>
#include <libC.h>
#include <sysLib.h>
#include <philo.h>
static int fd[2] = {STDIN, STDOUT};

int state[N];
sem_t * mutex;
sem_t * S[N];

static void test(int i){
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[i] = EATING;
        kill(getpid(), 1);
        //sleep(2);
        printf("Philospher %d takes fork %d and %d \n", i, LEFT, i);
        printf("Philosfer %d is eating \n", i);
        sem_post(S[i]);
    }
    
}

static void take_fork(int i){
    sem_wait(mutex);
    printf("forking");
    state[i] = HUNGRY;
    printf("Philospher %d is Hungry \n", i);
    test(i);
    sem_post(mutex);
    sem_wait(S[i]);
}

static void put_fork(int i){
    sem_wait(mutex);
    state[i] = THINKING;
    printf("Philospher %d putting fork %d and %d down", i, LEFT, i);
    printf("Philospher %d is thinking", i);
    test(RIGHT);
    test(LEFT);
    sem_post(mutex);
}

static void * philospher(char argc, char ** argv){
    printf("im platon\n");
   // printf("%s", argv[0]);
    
    int i = atoi(argv[0]);
    while (1)
    {
        for (int i = 0; i < 5; i++)
        {
            printf("im sleep");
        }
        
        take_fork(i);

        for (int i = 0; i < 5; i++)
        {
            printf("im sleep");
        }
        put_fork(i);
    }
    
}

int philosphers()
{
    printf("Philosphers problem: \n");
    printf("CREATING SEMS\n");
    mutex = sem_open("mutex", 0, 1);
    S[0] = sem_open("sem0", 0, 0);
    S[1] = sem_open("sem1", 0, 0);
    S[2] = sem_open("sem2", 0, 0);
    S[3] = sem_open("sem3", 0, 0);
    S[4] = sem_open("sem4", 0, 0);
    printf("SEMS READY\n");

    char ** buff0 = malloc(1);
	buff0[0] = malloc(20);
	itoa(0, buff0[0], 10);
	create("phil0", philospher, 2, fd, 1, buff0);
    
    char ** buff1 = malloc(1);
	buff1[0] = malloc(20);
	itoa(1, buff1[0], 10);
    create("phil1", philospher, 2, fd, 1, buff1);

    
    char ** buff2 = malloc(1);
	buff2[0] = malloc(20);
	itoa(2, buff2[0], 10);
    create("phil2", philospher, 2, fd, 1, buff2);
    
    char ** buff3 = malloc(1);
	buff3[0] = malloc(20);
	itoa(3, buff3[0], 10);
    create("phil3", philospher, 2, fd, 1, buff3);
    
    char ** buff4 = malloc(1);
	buff4[0] = malloc(20);
	itoa(4, buff4[0], 10);
    create("phil4", philospher, 2, fd, 1, buff4);
    
    printf("Philosphers are thinking\n");
    
    return 0;
}
