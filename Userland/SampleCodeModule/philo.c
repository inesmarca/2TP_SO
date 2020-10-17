/*
*                                   BIBLIOGRAFIA
*
*       https://www.geeksforgeeks.org/dining-philosopher-problem-using-semaphores/
*
*
*/
#include <sem.h>
#include <libC.h>
#include <sysLib.h>
#include <philo.h>
static int fd[2] = {STDIN, STDOUT};

int state[N];
sem_t * mutex;
sem_t * S[N];
int cant = 0;

static void test(int i){
    if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {    
        state[i] = EATING;
        //printf("Philospher %d takes fork %d and %d \n", i, LEFT, i);
        for (int j = 0; j < N; j++)
        {
            if (j == i) printf("E");
            else printf(" . ");
        }
        printf("\n");
        sem_post(S[i]);
    }
    
}

static void take_fork(int i){
    sem_wait(mutex);                                //FRENO SI ALGUIEN MAS INTENTA AGARRAR/DEJAR 
    state[i] = HUNGRY;
    //printf("El filosofo %d tiene hambre\n", i);
    test(i);                                        //VERIFICO SI PUEDO COMER
    sem_post(mutex);                                
    sem_wait(S[i]);                                 //SI NO COMIO ESPERA A QUE SUS VECINOS DEJEN LOS CUBIERTOS
}

static void put_fork(int i){
    sem_wait(mutex);                                //FRENO SI ALGUIEN MAS INTENTA AGARRAR/DEJAR
    state[i] = THINKING;
    //printf("Philospher %d putting fork %d and %d down\n", i, LEFT, i);
    test(RIGHT);                                    //VERIFICO SI LOS VECINOS COMEN YA QUE DEJE LOS CUBIERTOS
    test(LEFT);
    sem_post(mutex);
}

static void * philospher(char argc, char ** argv){
    int i = atoi(*argv);        //OBTENGO NUMERO DE FILOSOFO
    while (1)
    {
        take_fork(i);           //INTENTO AGARRAR TENEDORES
        put_fork(i);        
    }
    return NULL;
}
/*
int create_philo(){
    char * buff;
    buff = itoa(cant, buff, 10);
    printf("Numero de phil: %s\n", buff);
    S[cant] = sem_open(buff, 0, 0);
    if (S[cant] == NULL)
    {
        printf("ERROR AL CREAR SEMS");
        return -1;
    }
    char *buff0[5];
    *buff0 = buff;
    printf(*buff0);
    create(buff, philospher, 2, fd, 1, buff0);

    cant++;
    return 0;
}
*/
int philosphers() {
    printf("Philosphers problem: \n");
    printf("CREATING SEMS\n");
    mutex = sem_open("mutex", 0, 1);
    /*for (int i = 0; i < N; i++)
    {
      create_philo();
    }*/
    
    S[0] = sem_open("sem0", 0, 0);      //CREO LOS SEMS PARA CADA FILOSOFO
    S[1] = sem_open("sem1", 0, 0);
    S[2] = sem_open("sem2", 0, 0);
    S[3] = sem_open("sem3", 0, 0);
    S[4] = sem_open("sem4", 0, 0);
    printf("SEMS READY\n");

/*
    for (int i = 0; i < N; i++) {    
        char ** buff0 = malloc(1);
        buff0[i] = malloc(20);
        itoa(i, buff0[0], 10);
        char * num = itoa(i, num, 10);
        create(num, philospher, 2, fd, 1, buff0);
    }
    */
  
    char ** buff0 = malloc(1);                      //BUFFER PARA PASAR ARGV
	buff0[0] = malloc(20);
	itoa(0, buff0[0], 10);                          //LE PASO EL NUMERO DE FIL.
    create("phil0", philospher, 2, fd, 1, buff0);   //CREO PROCESO
  
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
