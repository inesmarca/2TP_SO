#include <programs.h>
#include <programHandler.h>
#include <libC.h>
#include <shell.h>
#include <libFun.h>
#include <sysLib.h>

#define CANT_FUNC 15

static void help();
static void initShell();
static void shellControler(char key);

static char functions[CANT_FUNC][20] = {"help","mem", "ps", "loop", "nice",         "cat",  "wc", "filter", "clear", "sem",  "philo",  "pipe",      "kill",     "block",     "unblock"};
static void (*func_ptr[CANT_FUNC])() = { help , mem,   ps,   loop,   nice_shell,     cat,    wc,   filter ,  clear,   sem,    philo,    pipeInfo,    kill_shell, block_shell, unblock_shell};
static char parameters[CANT_FUNC]    = { 0,     0,     0,    0,      2,              0,      0,    0,        0,       0,      0,        0,           1,          1,           1};
static char builtIn[CANT_FUNC]    =    { 1,     1,     1,    0,      1,              0,      0,    0,        1,       1,      0,        1,           1,          1,           1};
static char descripcion[CANT_FUNC][101] = {
    "enumeracion de las funciones disponibles del sistema", 
    "imprime el estado de la memoria", 
    "imprime la lista de todos los procesos activos con sus propiedades",
    "imprime su ID con un saludo cada una determinada cantidad de tiempo",
    "cambia la prioridad de un proceso dadu su ID y la nueva prioridad",
    "imprime el stdin tal como lo recibe",
    "cuenta la cantidad de lineas del input",
    "filtra las vocales del input e imprime el resultado",
    "limpia la pantalla",
    "lista los semaforos activos con su informacion correspondiente",
    "problema de los filosofos comensales",
    "imprime la lista de todos los pipes con sus propiedades",
    "Recibe el PID de un proceso y lo elimina",
    "Recibe el PID de un proceso y lo bloquea",
    "Recibe el PID de un proceso y lo desbloquea"
    };  


static char input[DIM_BUFFER] = {0};
static int pos = 0;
static char * user;



void shell(){
    initShell();
    while (1) {         // espera activa
        char k = getChar();
        shellControler(k);
    }
}

static void initShell() {
    setCursor(0, SCREEN_HEIGHT - LETTER_HEIGHT);
    printf("Enter your username: ");
    user = (char *)malloc(50);

    if (user == NULL)
        printError("Error setting username\n");

    scanf("%s", user);
    printf("\n");
    setCursor(0, SCREEN_HEIGHT - LETTER_HEIGHT);
    changeColor(0xFF00FF, DEFAULT_BACKGROUND_COLOR);
    printf("Bienvenido %s!\n", getUser());
    changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
    printf("Para comenzar, ingrese un comando\n");
    printf("Para una lista de los programas disponibles ingrese ");
    changeColor(0xE4D31B, DEFAULT_BACKGROUND_COLOR);
    printf("help\n");
    changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
    printUser();
}

static int isBackground() {
    int k=0;
    while (input[k]!=0){
        k++;
    }
    if (k>0 && input[k-1]=='&')
    {
        return 1;
    }
    return 0;
}

static int isPipe(){
    int k=0;
    while (input[k]!=0){
        k++;
    }
    while (k>2)
    {
        if (input[k]==' ' && input[k-1]=='|' && input[k-2]==' '){
        return 1;
        }
        k--;
    }
    return 0;
}

int lookForParameters(char ** argv, int argc, char * input) {
    // creacion de los mallocs para los parametros
    if (argc != 0) {
        if (argv == NULL) {
            printError("Error Creating argv\n");
            return -1;
        }
        
        for (int i = 0; i < argc; i++){
            argv[i] = malloc(255);

            if (argv[i] == NULL)
                printError("Error Creating Process 5\n");
        }      
    }


    int args = 0;
    int index=0;
    int k = 0;
    while (!(input[k] == 0 || input[k] == '|')) {
        if (input[k] == ' ') {
            argv[args][index] = 0;
            args++;
            index = 0;
        } else {
            argv[args][index++] = input[k];
        }
        k++;
    }

    if (input[k]== 0)
        argv[args][index] = 0;
    
    return k;
}

static void shellControler(char key) {
    if (key == '\n') {
        putChar('\n');
        if (pos != 0) {
            int j;
            int k=0;
            int background=isBackground();
            int pipes = isPipe();  // retorna la cantidad de pipes que hay
            int fd_pipe[2];
            char ** argv = NULL;
            int argc = parameters[j];


            if (pipes)
                pipe(fd_pipe);

            for (int pipecounter = 0;  pipecounter < pipes + 1; pipecounter++) {
                char * aux = malloc(DIM_BUFFER);
                memset(aux, 0, DIM_BUFFER);

                if (aux == NULL)
                    printError("Error Creating Process 1\n");
                
                for (int x = 0; input[k] != 0 && input[k] != ' '; x++) { // agaro la primera funcion
                    aux[x] = input[k];
                    k++;
                }
                
                for (j = 0; j < CANT_FUNC && !strcmp(aux, functions[j]) ; j++); // me fijo que la funcion sea valida

                if (j < CANT_FUNC) {
                    int fd_aux[MAX_PROCESS] = {STDIN, STDOUT, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
                    
                    char ** argv = NULL;
                    int argc = parameters[j];
                    
                    if (argc != 0) { // por lo tanto recibe parametros
                        argv = malloc(argc);
                        if (argv == NULL) {
                            printError("Error creating parameters\n");
                            return;
                        }

                        int aux;
                        if ((aux = lookForParameters(argv, argc, input)) == -1) { // tomo los parametros de la funcion
                            printError("Error Creating parameters\n");
                        }

                        k += aux;
                    }

                    if (builtIn[j] && pipes == 0) { // si la funcion es built in y no esta en un pipe
                        if (argc == 0) {
                            func_ptr[j]();
                        } else {
                            func_ptr[j](argc, argv);
                        }
                    } 
                    else if (background && pipes == 0) { // si la funcion es background y no esta en un pipe
                        fd_aux[0]=-1;
                        if (createBackground(aux, func_ptr[j], 1, fd_aux, argc, argv) == -1)
                            printError("Error Creating Process 2\n");
                    }
                    else if (pipes != 0) { // si hay un pipe
                        if (pipecounter) { // pipe del lado de y  ->  x | y 
                            fd_aux[STDIN] = fd_pipe[0];
                            if (createBackground(aux, func_ptr[j], 1, fd_aux, argc, argv) == -1)
                                printError("Error Creating Process 3\n");
                        }
                        else {// pipe del lado de x  ->  x | y 
                            fd_aux[1] = fd_pipe[1];
                            if (createForeground(aux, func_ptr[j], 1, fd_aux, argc, argv) == -1)
                                printError("Error Creating Process 3\n");
                        }
                    }
                    else {
                        if (createForeground(aux, func_ptr[j], 1, fd_aux, argc, argv) == -1)
                            printError("Error Creating Process 3\n");
                    }

                    if (argc != 0) {
                        // Liberacion de los parametros
                        for (int auxi = 0; auxi < argc; auxi++) {
                            printf("Im stuck here");
                            free(argv[auxi]);
                        }

                        free(argv);
                    }
                    
                    if (pipes && pipecounter != pipes) {
                        while (input[k] != '|') {
                            k++;
                        }
                        k += 2;
                    }
                    
                } else {
                    printError("Not a valid function\n");
                }
                input[0] = 0;
                free(aux);
            }
        }
        pos = 0;
        printUser();
    } else {
        if (key == DELETE) {
            if (pos != 0) {
                pos--;
                input[pos] = 0;
                putChar(key);
            }
        } else {
            putChar(key);
            input[pos++] = key;
            input[pos] = 0;
        }
    } 
}

static void help(){
    printf("La shell esta equipada con los siguientes Programas, para ejectutarlos ingrese el nombre del programa en la linea de comando \n");
    for (int i = 0; i < CANT_FUNC; i++) {
        changeColor(0x21DE7C, DEFAULT_BACKGROUND_COLOR);
        printf("%s ",functions[i]);
        changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
        printf("- %s\n", descripcion[i]);
    }
    printf("Para finalizar el programa presione ");
    changeColor(0xE4D31B, DEFAULT_BACKGROUND_COLOR);
    printf("ESC");
    changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
    putChar('\n');
    printf("Para informacion adicional, consultar documentacion.\n");
}

char * getUser() {
    return user;
}

void printUser() {
    changeColor(0xFF00FF, DEFAULT_BACKGROUND_COLOR);
    printf("%s$: ", getUser());
    changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
}