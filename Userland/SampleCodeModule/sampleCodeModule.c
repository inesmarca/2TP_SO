/* sampleCodeModule.c */
#include <programHandler.h>
#include <sysLib.h>
#include <test_util.h>
#include <libC.h>

int main() {
	//char k = getChar();
	switch ('2')//cambiar esto a k
	{
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
	while (1)
	{
		/* code */
	}
	

	
	//create("test_processes", test_processes, 0, 0);
	//create("programHandler", programHandler, 0, 0);
	return 0;
}