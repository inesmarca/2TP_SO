#include <keyboard.h>
#include <consoleManager.h>
#include <videoDriver.h>
#include <temperature.h>
#include <stdint.h>
#include <registers.h>

extern int getRTC(int x);

// Funcion de syscall que retorna el buffer de keys leidas
void readKey(uint64_t buf, uint64_t cant) {
    char * input = getBuffer();
    int i;
    for (i = 0; input[i] != 0 && i < (int)cant; i++) {
        ((char *)buf)[i] = input[i];
    }
    deleteBuff();
}

// escribe en pantalla dicho string con los colores especificados en la pantalla activa
void writeString(uint64_t string, uint64_t letter_color, uint64_t background_color) {
    print((const char *)string, (unsigned int)letter_color, (unsigned int)background_color);
}

// retorna un numero hexadecimal del color de dicho pixel
int getPixelData(uint64_t reg1, uint64_t reg2) {
    int screen = getCurrentScreen();
    int x = (int)reg1;
    int y = (int)reg2;
    if ((y >= 0 && y < SCREEN_HEIGHT) && x >= 0 && x < WIDTH) {
        int res;
        if (screen == 1) res = getPixelColor(x, y);
        else res = getPixelColor(x, y + SCREEN2_START_POS);
        return res;
    }
}

// dibuja en pantalla dicho pixel con el color recivido en base a las coordenads relativas de la pantalla activa
void printPixel(uint64_t reg1, uint64_t reg2, uint64_t reg3) {
    int screen = getCurrentScreen();
    int x = (int)reg1;
    int y = (int)reg2;
    int color = (int)reg3;
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
void getRegVec(uint64_t buff) {
    uint64_t * memory = getVec();
    for (int i = 0; i != 19; i++) {
        ((uint64_t *)buff)[i] = memory[i];
    }
}

// limpia la pantalla activa
void sysClear() {
    clear(getCurrentScreen());
}

// cambia de posicion el cursor de escritura dependiendo de la pantalla activa
void setCursor(uint64_t reg1, uint64_t reg2) {
    int screen = getCurrentScreen();
    int x = (int)reg1;
    int y = (int)reg2;
    if ((y >= LETTER_HEIGHT && y < SCREEN_HEIGHT) && x >= 0 && x < WIDTH - LETTER_WIDTH) {
        if (screen == 1) {
            changeCursor(screen, x, y + SCREEN1_START_POS);
        } else {
            changeCursor(screen, x, y + SCREEN2_START_POS);
        }
    }
}

// Print Time
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

void getTime(uint64_t reg1) {
    int * buff = (int *)reg1;
    buff[0] = fix_format_hours(getRTC(4));
    buff[1] = fix_format(getRTC(2));
    buff[2] = fix_format(getRTC(0));
}

void sysHandler(uint64_t reg1, uint64_t reg2, uint64_t reg3, int sys) {
    switch (sys) {
        case 0:
            readKey(reg1, reg2);
            break;
        case 1:
            writeString(reg1, reg2, reg3);
            break;
        case 2:
            getPixelData(reg1, reg2);
            break;
        case 3: 
            printPixel(reg1, reg2, reg3);
            break;
        case 4:
            sysClear();
            break;
        case 5:
            changeScreen(reg1);
            break;
        case 6:
            getTemperature();
            break;
        case 7:
            getRegVec(reg1);
            break;
        case 8:
            setCursor(reg1, reg2);
            break;
        case 9:
            getTime(reg1);
            break;
        default:
            break;
    }
}