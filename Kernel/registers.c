#include <stdint.h>
#include <lib.h>
#include <consoleManager.h>


/*
	"R15:   ", "R14:   ", "R13:   ", "R12:   ", "R11:   ", 
	"R10:   ", "R9:    ", "R8:    ", "RSI:   ", "RDI:   ", 
	"RBP:   ", "RDX:   ", "RCX:   ", "RBX:   ", "RAX:   ", 
	"RIP:   ", "CS:    ", "FLAGS: ", "RSP:   "
*/

uint64_t memory[19] = {0};

// La posicion inicial del stack frame apunta al registro R15 y sigue de acuerdo al comentario anterior
void saveReg(uint64_t * stackFrame) {
    for (int i = 0; i < 19; i++) {
        memory[i] = stackFrame[i];
    }
}

// es usado por el sysCall para retornar el vector con la memoria guardada
uint64_t * getVec() {
    return memory;
}