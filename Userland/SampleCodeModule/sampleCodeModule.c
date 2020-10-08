/* sampleCodeModule.c */
#include <programHandler.h>
#include <sysLib.h>
#include <test_util.h>
#include <libC.h>

int main() {
	printf("Ingrese 1 para MM, 2 para Scheduler, 3 para Prioridades");
	char k = getChar();
	switch (k)
	{
	case '1':
		printf("Testeo de Memory Manager \nCierre QEMU para finalizar\n");
		test_mm();	
		break;
	case '2':
		printf("Testeo de Procesos \nCierre QEMU para finalizar\n");
		if (create("test_processes", test_processes, 2, 0, 0) == -1)
			printf("Esta fallando la creacion");
		break;
	case '3':
		if (create("test_pri", test_prio, 2, 0, 0) == -1)
			printf("Esta fallando la creacion");
		break;
	default:
		printf("Por favor, ingrese '1' o '2' \n");
		break;
	}
	
	//create("test_processes", test_processes, 0, 0);
	//create("programHandler", programHandler, 0, 0);
	return 0;
}