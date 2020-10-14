#include <keyboard.h>
#include <consoleManager.h>
#include <videoDriver.h>
#include <temperature.h>
#include <stdint.h>
#include <registers.h>
#include <buddyMM.h>
#include <scheduler.h>
#include <simpleMM.h>
#include <lib.h>
#include <semaphore.h>

extern int getRTC(int x);
void _hlt();

// Funcion de syscall que retorna el buffer de keys leidas
void readKey(char * buf, int cant) {
    char * input = getBuffer();
    int i;
    for (i = 0; input[i] != 0 && i < cant; i++) {
        (buf)[i] = input[i];
    }
    deleteBuff();
}

// escribe en pantalla dicho string con los colores especificados en la pantalla activa
void writeString(const char * string, unsigned int letter_color, unsigned int background_color) {
    print(string, letter_color, background_color);
}

// retorna un numero hexadecimal del color de dicho pixel
int getPixelData(int x, int y) {
    int screen = getCurrentScreen();
    if ((y >= 0 && y < SCREEN_HEIGHT) && x >= 0 && x < WIDTH) {
        int res;
        if (screen == 1) res = getPixelColor(x, y);
        else res = getPixelColor(x, y + SCREEN2_START_POS);
        return res;
    }
    return 0;
}

// dibuja en pantalla dicho pixel con el color recivido en base a las coordenads relativas de la pantalla activa
void printPixel(int x, int y, int color) {
    int screen = getCurrentScreen();
    if ( y >= 0 && y < SCREEN_HEIGHT && x >= 0 && x < WIDTH) {
        if (screen == 1) writePixel(x, y, color);
        else writePixel(x, y + SCREEN2_START_POS, color);
    }
}

// retorna el valor de la temperatura
int getTemperature(){
    return getTemp();
}


// retorna el vector de registros que fueron gurdados al apretar CTRL S
void getRegVec(uint64_t * buff) {
    uint64_t * memory = getVec();
    for (int i = 0; i != 19; i++) {
        buff[i] = memory[i];
    }
}

// limpia la pantalla activa
void sysClear() {
    clear(getCurrentScreen());
}

// cambia de posicion el cursor de escritura dependiendo de la pantalla activa
void setCursor(int x, int y) {
    int screen = getCurrentScreen();
    if ((y >= LETTER_HEIGHT && y < SCREEN_HEIGHT) && x >= 0 && x < WIDTH - LETTER_WIDTH) {
        if (screen == 1) {
            changeCursor(screen, x, y + SCREEN1_START_POS);
        } else {
            changeCursor(screen, x, y + SCREEN2_START_POS);
        }
    }
}

// Get Time
int fix_format_hours(int time) {
    int aux = time/16;
    aux *= 10;
    aux = aux + (time % 16) - 3;
    if (aux < 0) {
        aux += 24;
    }
    return aux % 24;
}

int fix_format(int time) {
    int aux = time / 16;
    aux *= 10;
    aux = aux + (time % 16);
    return aux;
} 

int intToString(int value, char * buffer) {
	char *p = buffer;
	char *p1, *p2;
	int digits = 0;

	//Calculate characters for each digit
	do {
		int remainder = value % 10;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= 10);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2) {
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void getTime(int * buff) {
    buff[0] = fix_format_hours(getRTC(4));
    buff[1] = fix_format(getRTC(2));
    buff[2] = fix_format(getRTC(0));
}

// Change State
int changeState(int pid, int state) {
    int res;
    switch (state) {
        case 0:
            res = kill(pid);
            break;
        case 1: 
            res = block(pid);
            break;
        case 2:
            res = unblock(pid);
            break;
        default:
            res = -1;
            break;
    }
    return res;
}

// Malloc
void * malloc(int size) {
    #ifdef MM_BUDDY
        return malloc_buddy(size);
    #else  
        return malloc_simple(size);
    #endif
}

// Free
void free(void * dir) {
    #ifdef MM_BUDDY
        free_buddy(dir);
    #else  
        free_simple(dir);
    #endif
}

//Total Mem Available
int getTotalMem(){
    #ifdef MM_BUDDY
        return getTotalMem_Buddy();
    #else  
        return getTotalMem_Simple();
    #endif
}
//Mem currently in Use
int getUsedMem(){
    #ifdef MM_BUDDY
        return getUsedMem_Buddy();
    #else  
        return getUsedMem_Simple();
    #endif
}
char buff[50] = {0};
// Syscall Handler
uint64_t sysHandler(uint64_t reg1, uint64_t reg2, uint64_t reg3, uint64_t reg4, uint64_t reg5, int sys) {
    uint64_t res;
    switch (sys) {
        case 0:
            readKey((char *)reg1, (int)reg2);
            break;
        case 1:
            writeString((const char *)reg1, (unsigned int)reg2, (unsigned int)reg3);
            break;
        case 2:
            res = getPixelData((int)reg1, (int)reg2);
            break;
        case 3: 
            printPixel((int)reg1, (int)reg2, (int)reg3);
            break;
        case 4:
            sysClear();
            break;
        case 5:
            changeScreen((int)reg1);
            break;
        case 6:
            res = getTemperature();
            break;
        case 7:
            getRegVec((uint64_t *)reg1);
            break;
        case 8:
            setCursor((int)reg1, (int)reg2);
            break;
        case 9:
            getTime((int *)reg1);
            break;
        case 10:
            res = malloc((int)reg1);
            break;
        case 11:
            free((point)reg1);
            break;
        case 12:
            res = changeState((int)reg1, (int)reg2);
            break;
        case 13:
            //print(((char **)reg5)[0], LETTER_COLOR, BACKGROUND_COLOR);
            res = createProcess((char *)reg1, (void *)reg2, (int)reg3, (int)reg4, (char **)reg5);
            break;
        case 14:
            res = getpid();
            break;
        case 15:
            res = nice((int)reg1, (int)reg2);
            break;
        case 16: 
            yield();
            break;
        case 17:
            res = sem_open((char *)reg1, (char)reg2, (int)reg3);
            break;
        case 18:
            res = sem_wait((sem_t *)reg1);
            break;
        case 19:
            res = sem_post((sem_t *)reg1);
            break;
        case 20:
            res = sem_close((sem_t *)reg1);
            break;
        default:
            break;
    }
    return res;
}