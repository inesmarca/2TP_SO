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

// test para pipes
void readTest() {
	while(1) {
		char buff[50];
		read(STDIN, buff, 50);
		printf("\n String mandado por el otro proceso: %s\n", buff);
	}
}

void writeTest() {
	char buff[50] = "hola mundo";
	while(1) {
		write(STDOUT, buff, 50);
	}
}

void pipeTest() {
	int aux[2];
	if (pipe(aux) == -1)
		printf("Error creacion");

	int p1[2];
	p1[0] = STDIN;
	p1[1] = aux[1];

	int p2[2];
	p2[0] = aux[0];
	p2[1] = STDOUT;

	create("readTest", readTest, 0, p2, 0, 0);
	create("writeTest", writeTest, 0, p1, 0, 0);
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
	if (create("tests", pipeTest, 0, fd, 0, 0) == -1)
		printf("Error Creating Process\n");
	return 0;
}