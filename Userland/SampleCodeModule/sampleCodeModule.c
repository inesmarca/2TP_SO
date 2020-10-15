/* sampleCodeModule.c */
#include <programHandler.h>
#include <sysLib.h>
#include <test_util.h>
#include <libC.h>

static int fd[2] = {STDIN, STDOUT};

int createBackground(const char * name, void * func, int priority, int argc, char * argv[]) {
	int aux[2] = {-1, STDOUT};
	return create(name, func, priority, aux, argc, argv);
}

int test() {
	while(1) {
		printf("Hola Mundo\n");
	}
}

int tests() {
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
		if (create("test_processes", test_processes, 2, fd, 0, 0) == -1)
			printf("Esta fallando la creacion");
		break;
	case '3':
		if (create("test_pri", test_prio, 2, fd, 0, 0) == -1)
			printf("Esta fallando la creacion");
		break;
	case '4':
		create("test_sync",test_sync, 2, fd, 0, 0);
		break;
	case '5':
		create("test_no_sync",test_no_sync, 2, fd, 0, 0);
		break;
	default:
		printf("Por favor, ingrese '1' o '2' \n");
		break;
	}

	return 0;
}

int main() {
	if (create("tests", tests, 0, fd, 0, 0) == -1)
		printf("Error Creating Process\n");
	return 0;
}