#ifndef __CONSOLE_MANAGER_H_
#define __CONSOLE_MANAGER_H_

#define SCREEN1_START_POS 0
#define SCREEN1_END_POS 382
#define SCREEN2_START_POS 385
#define SCREEN2_END_POS HEIGHT
#define SCREEN_HEIGHT 382

#define SCREEN_START_POS 0
#define SCREEN_END_POS HEIGHT

#define BACKGROUND_COLOR 0x000000
#define LETTER_COLOR 0xFFFFFF

int print(const char * string, int cant);

void delete(int background_color);

void middleLine();

void newLine(int background_color);

void clear();

void changeCursor(int x, int y);

void changeColor(int letter, int background);

#endif