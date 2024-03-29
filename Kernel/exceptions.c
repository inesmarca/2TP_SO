// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <consoleManager.h>
#include <stdint.h>
#include <lib.h>
#include <exception.h>
#include <time.h>
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_EXCEPTION_ID 6

static void zero_division();
static void invalid_opcode();
void _hlt();
void _sti();

// valores de retorno de la excepcion
uint64_t * ipReturn;
uint64_t * rspReturn;

static char * regs[] = {
	"R15:   ", "R14:   ", "R13:   ", "R12:   ", "R11:   ", 
	"R10:   ", "R9:    ", "R8:    ", "RSI:   ", "RDI:   ", 
	"RBP:   ", "RDX:   ", "RCX:   ", "RBX:   ", "RAX:   ", 
	"RIP:   ", "CS:    ", "FLAGS: ", "RSP:   "
};

// imprime los registros y despliega un timer para el reinicio de la pantalla
void printRegs(uint64_t * stackFrame) {
	char buffer[9];
	for (int i = 0; i < 19; i++) {
		print(regs[i], 19);
		uintToBase(stackFrame[i], buffer, 16);
		baseToHexa(buffer);
		print(buffer, 9);
		if (i % 2 == 0) {
			newLine(BACKGROUND_COLOR);
		} else {
			print("           ", 12);
		}
	}
	// habilita de vuelta las interrupciones
	_sti();
	print("La pantalla se reiniciara en ", 30);
	char buff[3] = {0};
	// espera a una interrupcion
	_hlt();
	int init_time = seconds_elapsed();
	int aux = 10;
	int i = 10;
	uintToBase(i, buff, 10);
	changeColor(0xFB781F, BACKGROUND_COLOR);
	print(buff, 9);
	while (i > 0) {
		_hlt();
		aux = 10 - (seconds_elapsed() - init_time);
		if (i != aux) {
			delete(BACKGROUND_COLOR);
			if (i == 10) {
				delete(BACKGROUND_COLOR);
			}
			i = aux;
			uintToBase(i, buff, 10);
			changeColor(0xFB781F, BACKGROUND_COLOR);
			print(buff, 3);
		}
	}
	newLine(BACKGROUND_COLOR);
}

// setea los registros de retorno
void setAddresses(uint64_t * ip, uint64_t * rsp) {
	ipReturn = ip;
	rspReturn = rsp;
}

// resetea la pantalla
void resetScreen(uint64_t * stackFrame) {
	stackFrame[15] = (uint64_t)ipReturn;
	stackFrame[18] = (uint64_t)rspReturn;
	clear(1);
	clear(2);
}

void exceptionDispatcher(int exception, uint64_t * stackFrame) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division(stackFrame);
	else if (exception == INVALID_OPCODE_EXCEPTION_ID)
		invalid_opcode(stackFrame);
}

// handler de la excepcion 0
static void zero_division(uint64_t * stackFrame) {
	changeColor(0xFF0000, BACKGROUND_COLOR);
	print("Exception 0: division by 0\n", 28);
	printRegs(stackFrame);
	resetScreen(stackFrame);
}

// handler de la excepcion 6
static void invalid_opcode(uint64_t * stackFrame) {
	changeColor(0xFF0000, BACKGROUND_COLOR);
	print("Exception 6: invalid opcode\n", 29);
	printRegs(stackFrame);
	resetScreen(stackFrame);
}