#include <consoleManager.h>
#include <videoDriver.h>

#define LINE_START_POS SCREEN1_END_POS + 1
#define LINE_HEIGHT 4
#define B_SPACE 0x0E

// valores por default de los cursores
static int pos1X = 0;
static int pos1Y = SCREEN1_START_POS;
static int pos2X = 0;
static int pos2Y = SCREEN2_END_POS - LETTER_HEIGHT + 1;

// variable para saber la pantalla activa
static int currentScreen = 1;

// cambia la pantalla activa (utilizado por el syscall)
void changeScreen(int screen) { 
    if (screen == 1 || screen == 2) {
        currentScreen = screen; 
    }
}

// cambia la posicion del cursor de escrtitura en base a la pantalla activa
void changeCursor(int screen, int x, int y) {
    if (getCurrentScreen() == 1) {
        pos1X = x;
        pos1Y = y;
    } else {
        pos2X = x;
        pos2Y = y;
    }
}

// retorna la pantalla activa
int getCurrentScreen() { return currentScreen; }

// imprime un string desde la posicion del cursor en forma horizontal con los colores recividos
void print(const char * string, int letter_color, int background_color) {
    int * posX;
    int * posY;
    if (getCurrentScreen() == 1) {
        posX = &pos1X;
        posY = &pos1Y;
    } else {
        posX = &pos2X;
        posY = &pos2Y;
    }

    for (int i = 0; string[i] != 0; i++) {
        if (string[i] == '\n' || *posX == WIDTH) {
            newLine(background_color);
        } else if (string[i] == B_SPACE) {
            delete(background_color);
        } else {
            writeLetter(string[i], *posX, *posY, letter_color, background_color);
            *posX += LETTER_WIDTH;        
        }
    }
}

// borra el caracter en la poscion antes del cursor
void delete(int background_color) {
    int * posX;
    int * posY;
    if (getCurrentScreen() == 1) {
        posX = &pos1X;
        posY = &pos1Y;
    } else {
        posX = &pos2X;
        posY = &pos2Y;
    }
    if (*posX != 0) {
        *posX -= LETTER_WIDTH;
        setSegmentBlank(*posX, *posX + LETTER_WIDTH, *posY, *posY + LETTER_HEIGHT, background_color);
    }
}

// hace un scroll hacia arriba de la pantalla activa
void newLine(int background_color) {
    int * posX;
    int top_pos;
    int bottom_pos;
    if (getCurrentScreen() == 1) {
        posX = &pos1X;
        top_pos = SCREEN1_START_POS;
        bottom_pos = SCREEN1_END_POS;
    } else {
        posX = &pos2X;
        top_pos = SCREEN2_START_POS;
        bottom_pos = SCREEN2_END_POS;
    }
    *posX = 0;
    setSegmentBlank(0, WIDTH, top_pos, top_pos + LETTER_HEIGHT, background_color);
    for (int y = top_pos; y < bottom_pos - LETTER_HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int color = getPixelColor(x, y);
            writePixel(x, y, getPixelColor(x, y + LETTER_HEIGHT));
            writePixel(x, y + LETTER_HEIGHT, color);
        }
    }
}

// dibuja la linea de division
void middleLine() {
    for (int i = 0; i < LINE_HEIGHT; i++) {
        drawLine(LINE_START_POS + i, 0x62FFCC);
    }
}

// limpia la pantalla
void clear(int screen) {
    if (screen == 1) {
        for (int x = 0; x < WIDTH; x++) {
            for (int y = SCREEN1_START_POS; y < SCREEN1_END_POS; y++) {
                writePixel(x, y, BACKGROUND_COLOR);
            }
        }
    } else {
        for (int x = 0; x < WIDTH; x++) {
            for (int y = SCREEN2_START_POS; y < SCREEN2_END_POS; y++) {
                writePixel(x, y, BACKGROUND_COLOR);
            }
        }
    }
}