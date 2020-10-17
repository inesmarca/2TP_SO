#include <programs.h>
#include <programHandler.h>
#include <libC.h>
#include <shell.h>
#include <libFun.h>
#include <sysLib.h>

#define CANT_FUNC 12

static void help();
static void initShell();
static void shellControler(char key);



static char functions[CANT_FUNC][20] = {"help","mem", "ps", "loop", "nice", "cat",  "wc", "filter", "clear", "sem",  "philo",  "pipe"};
static void (*func_ptr[CANT_FUNC])() = { help , mem,   ps,  loop,   nice,    cat,    wc,   filter ,  clear,   sem,     philo,  pipeInfo};
static char parameters[CANT_FUNC]    = { 0,     0,     0,   0,      2,       0,      0,    0,          0,      0,      0,      0};
static char builtIn[CANT_FUNC]    =    { 1,     1,     1,   0,      1,       0,      0,    0,          1,      1,      0,      1};
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
    "imprime la lista de todos los pipes con sus propiedades"
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

static void shellControler(char key) {
    if (key == '\n') {
        putChar('\n');
        if (pos != 0) {
            int j;
            int k=0;
            int background=isBackground();
            int pipes=isPipe();
            int fd[2] = {STDIN, STDOUT};
            int fdcopy[2];

            if (pipes)
                pipe(fd);

            fdcopy[0]=fd[0];
            fdcopy[1]=fd[1];

            for (int pipecounter = 0;  pipecounter < pipes + 1; pipecounter++) {
                if (pipes==1) {
                    if (pipecounter==0) {
                        fd[0] = STDIN;
                        fd[1] = fdcopy[1];                       
                    } else {
                        fd[0] = fdcopy[0];
                        fd[1] = STDOUT;
                    }
                }
                
                char * aux = malloc(DIM_BUFFER);

                if (aux == NULL)
                    printError("Error Creating Process 1\n");
                
                while (input[k] != ' ' && input[k] != 0) {
                    aux[k] = input[k];
                    k++;
                }

                aux[k]=0;
                
                for (j = 0; j < CANT_FUNC && !strcmp(aux, functions[j]) ; j++);

                if (j < CANT_FUNC) {
                    if (parameters[j] == 0) {
                        if (background) {
                            if (createBackground(aux, func_ptr[j], 1, 0, 0) == -1)
                                printError("Error Creating Process 2\n");
                        } else if (!builtIn[j] || pipes != 0) {
                            if (create(aux, func_ptr[j], 1, fd, 0, 0) == -1)
                                printError("Error Creating Process 3\n");
                        } else {
                            func_ptr[j]();
                        }   
                    } else {
                        k++;
                        char ** argv = malloc(parameters[j]);

                        if (parameters[j]!=0) {
                            if (argv == NULL)
                                printError("Error Creating argv\n");
                            for (int args= 0; args < parameters[j]; args++){
                                argv[args] = malloc(255);

                                if (argv[args] == NULL)
                                    printError("Error Creating Process 5\n");
                            }      
                        }
                    
                        while (input[k]!=0) {
                            int args=0;
                            int index=0;

                            if (input[k]==' ') {
                                argv[args][index]=0;
                                args++;
                                index=0;
                            } else {
                                argv[args][index++]=input[k];
                            }

                            k++;
                        }

                        if (background) {
                            if (createBackground(aux, func_ptr[j], 1, parameters[j], argv) == -1)
                                printError("Error Creating Process 6\n");
                        } else if (!builtIn[j] || pipes != 0) {
                            if (create(aux, func_ptr[j], 1, fd, parameters[j], argv) == -1)
                                printError("Error Creating Process 7\n");
                        } else {
                            func_ptr[j](parameters[j], argv);
                        }

                        for (int auxi = 0; auxi < parameters[j]; auxi++) {
                            free(argv[auxi]);
                        }

                        free(argv);
                    }
                    
                } else {
                    printError("Not a valid function\n");
                }

                input[0] = 0;
                pos = 0;
                free(aux);
            }
        }
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