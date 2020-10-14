/* sampleCodeModule.c */
#include <programHandler.h>
#include <sysLib.h>
#include <test_util.h>
#include <libC.h>

void test(int argc, char ** argv) {
	for (int i = 0; i < argc; i++) {
		printf("%d ", atoi(argv[i]));
	}
	printf("\nTermine\n");
}

int main() {
	/*
	char * buff[50];
	buff[0] = malloc(20);
	buff[1] = malloc(20);
	buff[2] = malloc(20);

	itoa(10, buff[0], 10);
	itoa(10000000, buff[1], 10);
	itoa(-4, buff[2], 10);

	create("test", test, 0, 3, buff);
	create("test", test, 0, 3, buff);
	create("test", test, 0, 3, buff);
	create("test", test, 0, 3, buff);
	create("test", test, 0, 3, buff);
	*/

	
	printf("Ingrese 1 para MM, 2 para Scheduler, 3 para Prioridades, 4 sync con semaforos y 5 sin semaforos \n");
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
	case '4':
		create("test_sync",test_sync, 2, 0, 0);
		break;
	case '5':
		create("test_no_sync",test_no_sync, 2, 0, 0);
		break;
	default:
		printf("Por favor, ingrese '1' o '2' \n");
		break;
	}
	
	//create("test_processes", test_processes, 0, 0);
	//create("programHandler", programHandler, 0, 0);

	return 0;
}