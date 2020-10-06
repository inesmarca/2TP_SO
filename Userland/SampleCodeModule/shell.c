#include <programs.h>
#include <programHandler.h>
#include <libC.h>
#include <shell.h>
#include <libFun.h>
#include <sysLib.h>
#define CANT_FUNC 9

void help();

char functions[CANT_FUNC][20] = {"help","printTime", "printTemperature","printCPUInfo", "inforeg", "triggerException0", "triggerException6","printmem", "clear"};
void (*func_ptr[CANT_FUNC])() = { help , printTime,   printTemperature,  printCPUInfo,   inforeg,   triggerException0,   triggerException6,  printmem ,  clear};
char parameters[CANT_FUNC]    = { 0,     0,           0,                 0,              0,         0,                   0,                  1,          0       };
char descripcion[CANT_FUNC][101] = {
    "enumeracion de las funciones disponibles del sistema", 
    "imprime en pantalla la hora actual", 
    "imprime en pantalla la temperatura de la computadora",
    "imprime en pantalla la inforamcion sobre el CPU",
    "imprime en pantalla los registros guardados al apretar CTRL S",
    "demostracion de la excepcion division por 0",
    "demostracion de la excepcion invalid opcode",
    "recibe una direccion de memoria y hace un volcado de memoria de 32 bytes a partir de dicho parametro",
    "limpia la pantalla"
    };  


static char input[DIM_BUFFER] = {0};
static int pos = 0;


void initShell() {
    setCursor(0, SCREEN_HEIGHT - LETTER_HEIGHT);
    changeLetterColor(0xFF00FF);
    printf("Bienvenido %s!\n",getUser());
    changeLetterColor(DEFAULT_LETTER_COLOR);
    printf("Para comenzar, ingrese un comando o presione la tecla ");
    changeLetterColor(0xE4D31B);
    printf("TAB");
    changeLetterColor(DEFAULT_LETTER_COLOR);
    printf(" para cambiar de programa\n");
    printf("Para una lista de los programas disponibles ingrese ");
    changeLetterColor(0xE4D31B);
    printf("help\n");
    changeLetterColor(DEFAULT_LETTER_COLOR);
    printUser();
}

void shell(char key) {
    if (key == '\n') {
        putChar('\n');
        if (pos != 0) {
            int j;
            int k=0;
            
            char aux[DIM_BUFFER];
            while (input[k] != ' ' && input[k] != 0)
            {
                aux[k]=input[k];
                k++;
            }
            aux[k]=0;
            
            for (j = 0; j < CANT_FUNC && !strcmp(aux, functions[j]) ; j++) {}
            if (j < CANT_FUNC) {
                if (parameters[j==0])
                {
                     func_ptr[j]();
                }
                int d=0;
                k++;
                while (input[k]!=0) //copio solo los parametros
                {
                    aux[d++]=input[k++];
                }
                aux[d]=0;
                func_ptr[j](aux);
               
            } else {
                printError("Not a valid function\n");
            }
            input[0] = 0;
            pos = 0;
        }
        printUser();
    } else if (key == DELETE) {
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

void help(){
    printf("La shell esta equipada con los siguientes Programas, para ejectutarlos ingrese el nombre del programa en la linea de comando \n");
    for (int i = 0; i < CANT_FUNC; i++) {
        changeLetterColor(0x21DE7C);
        printf("%s ",functions[i]);
        changeLetterColor(DEFAULT_LETTER_COLOR);
        printf("- %s\n", descripcion[i]);
    }
    printf("Para finalizar el programa presione ");
    changeLetterColor(0xE4D31B);
    printf("ESC");
    changeLetterColor(DEFAULT_LETTER_COLOR);
    putChar('\n');
    printf("Para informacion adicional, consultar documentacion.\n");
}