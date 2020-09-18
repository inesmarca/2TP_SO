#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <videoDriver.h>
#include <consoleManager.h>
#include <idtLoader.h>
#include <exception.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

extern uint64_t * getRSP();

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{

	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

int main()
{	
	load_idt();
	middleLine();
	// por default se indica esta como primera
	changeScreen(2);
	// se envian los valores iniciales del RSP y de la primera 
	// instruccion para el reinicio luego de una excepcion
	setAddresses((uint64_t *)sampleCodeModuleAddress, getRSP());
	
	((EntryPoint)sampleCodeModuleAddress)();
	return 0;
}
