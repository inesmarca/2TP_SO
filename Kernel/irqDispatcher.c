// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <keyboard.h>
#include <stdint.h>
#include <time.h>
#include <scheduler.h>
#include <consoleManager.h>
#include <defs.h>

static uint64_t * int_20(uint64_t * rsp);
static void int_21();

uint64_t * irqDispatcher(uint64_t irq, uint64_t * rsp) {
	switch (irq) {
		case 0:
			return int_20(rsp);
			break;
		case 1:
			int_21(rsp);
			break;
	}
	return rsp;
}

uint64_t * int_20(uint64_t * rsp) {
	timer_handler();
	return swap(rsp);
}

void int_21(uint64_t * rsp) {
	keyboard_handler(rsp);
}
