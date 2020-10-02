/* sampleCodeModule.c */
#include <programHandler.h>
#include <sysLib.h>
#include <test_util.h>
#include <libC.h>

int main() {
	printf("Ingreso 1 para testear el MM o 2 para testear el scheduler:");
	char k = getChar();
	printf("\n");
	switch (k) {
	case '1':
		printf("Testeo de Memory Manager \nCierre QEMU para finalizar\n");
		test_mm();	
		break;
	case '2':
		printf("Testeo de Procesos \nCierre QEMU para finalizar\n");
		create("test_processes", test_processes, 0, 0);
		break;
	default:
		printf("Por favor, ingrese '1' o '2' \n");
		break;
	}

	//create("programHandler", programHandler, 0, 0);
	return 0;
}