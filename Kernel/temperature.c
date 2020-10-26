// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <stdint.h>

extern uint64_t getTempTargetInfo();
extern uint64_t getTempOffsetInfo();

// Hace la conversion de la temperatura y la retorna
int getTemp(){
    int maxtemp=getTempTargetInfo();//23:16 bits relevantes
    maxtemp=maxtemp & 0x00FF0000;
    maxtemp=maxtemp>>16;
    
    int tempoffset=getTempOffsetInfo();         //22-16 bits relevantes
    tempoffset=tempoffset &  0x007F0000;
    tempoffset=tempoffset>>16;

    return maxtemp-tempoffset;

}

