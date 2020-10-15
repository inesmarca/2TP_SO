#include <consoleManager.h>
#include <videoDriver.h>

#define LINE_START_POS SCREEN1_END_POS + 1
#define LINE_HEIGHT 4
#define B_SPACE 0x0E

// valores por default de los cursores
static int posX = 0;
static int posY = HEIGHT - LETTER_HEIGHT + 1;

int letter_color = LETTER_COLOR;
int background_color = BACKGROUND_COLOR;

void changeColor(int letter, int background) {
    letter_color = letter;
    background_color = background;
}

// cambia la posicion del cursor de escrtitura en base a la pantalla activa
void changeCursor(int x, int y) {
    posX = x;
    posY = y;
}

// imprime un string desde la posicion del cursor en forma horizontal con los colores recividos
int print(const char * string, int cant) {
    int i;
    for (i = 0; string[i] != 0; i++) {
        if (string[i] == '\n' || posX == WIDTH) {
            newLine(background_color);
        } else if (string[i] == B_SPACE) {
            delete(background_color);
        } else {
            writeLetter(string[i], posX, posY, letter_color, background_color);
            posX += LETTER_WIDTH;        
        }
    }
    return i;
}

// borra el caracter en la poscion antes del cursor
void delete(int background_color) {
    if (posX != 0) {
        posX -= LETTER_WIDTH;
        setSegmentBlank(posX, posX + LETTER_WIDTH, posY, posY + LETTER_HEIGHT, background_color);
    }
}

// hace un scroll hacia arriba de la pantalla activa
void newLine(int background_color) {
    int top_pos = SCREEN_START_POS;
    int bottom_pos = SCREEN_END_POS;
    
    posX = 0;
    setSegmentBlank(0, WIDTH, top_pos, top_pos + LETTER_HEIGHT, background_color);
    for (int y = top_pos; y < bottom_pos - LETTER_HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int color = getPixelColor(x, y);
            writePixel(x, y, getPixelColor(x, y + LETTER_HEIGHT));
            writePixel(x, y + LETTER_HEIGHT, color);
        }
    }
}

// limpia la pantalla
void clear(int screen) {
    for (int x = 0; x < WIDTH; x++) {
        for (int y = SCREEN_START_POS; y < SCREEN_END_POS; y++) {
            writePixel(x, y, BACKGROUND_COLOR);
        }
    }
}