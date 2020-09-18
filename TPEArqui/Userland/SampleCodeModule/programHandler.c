#include <sysLib.h>
#include <programHandler.h>
#include <calculator.h>
#include <shell.h>
#include <programs.h>
#include <libC.h>
#include <libFun.h>

#define UP 1
#define DOWN 2

char user[30];
int exitFlag = 0;
static char buffer[DIM_BUFFER] = {0};

void exit() {
    exitFlag = 1;
}

int activeScreen;

void changeScreen(int screen) {
    activeScreen = screen;
    sysChangeScreen(screen);
}

char * getUser() {
    return user;
}

void initialize() {
    setCursor(0, SCREEN_HEIGHT - LETTER_HEIGHT);
    printf("Enter your username: ");
    scanf("%s", user);
}

void printUser() {
    changeLetterColor(0xFF00FF);
    printf("%s$: ", getUser());
    changeLetterColor(DEFAULT_LETTER_COLOR);
}

void programHandler() {
    changeScreen(DOWN);
    initialize();
    changeScreen(UP);
    // inicializa la calculadora
    initCalculator();
    changeScreen(DOWN);
    // inicializa la shell
    initShell();
    // imprime el cursor
    printBlock();
    while (!exitFlag) {
        readKeyBuff(buffer, WIDTH/8);
        for (int i = 0; buffer[i] != 0; i++) {
            if (buffer[i] == '\t') {
                if (activeScreen == UP) {
                    putChar(DELETE);
                    changeScreen(DOWN);
                    printBlock();
                } else {
                    putChar(DELETE);
                    changeScreen(UP);
                    printBlock();
                }
            } else if (buffer[i] == ESC) {
                exitFlag = 1;
            } else {
                putChar(DELETE);
                if (activeScreen == UP) {
                    calculator(buffer[i]);
                } else {
                    shell(buffer[i]);
                }
                printBlock();
            }
            buffer[i] = 0;
        }
    }
    changeScreen(UP);
    clear();
    changeScreen(DOWN);
    clear();
    changeLetterColor(0x0000FF);
    setCursor(WIDTH/2 - 15*LETTER_WIDTH, SCREEN_HEIGHT/2 - 3*LETTER_HEIGHT);
    printf("Has finalizado el programa\n");
    changeLetterColor(0xE4D31B);
    setCursor(WIDTH/2 - 25*LETTER_WIDTH, SCREEN_HEIGHT/2 - 2*LETTER_HEIGHT);
    printf("Para volver a correrlo reiniciar el programa\n");
    changeLetterColor(0x0000FF);
    setCursor(WIDTH/2 - 15*LETTER_WIDTH, SCREEN_HEIGHT/2 - LETTER_HEIGHT);
    printf("Nos vemos luego %s", user);
}