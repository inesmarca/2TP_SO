#include <libC.h>
#include <libFun.h>
#define MAX_DIGITS 20

extern int cpuModel();
extern char * cpuVendor();
extern void getRegisters(uint64_t * buff) ;
extern int getTemperature();
extern void getTime(int * buff);

// Trigger Exception 0
void triggerException0() {
    int x = 5 / 0;
}

void printTime(){
    int buff[3];
    getTime(buff);
    printf("%d:%d:%d/n", buff[0], buff[1], buff[2]);
}


// Print Temperature
void printTemperature() {
	printf("%d\n", getTemperature());
}

// Printmem
void hexaTo2(char * buff, int dim) {
	char auxStr[5] = "0x00";
	for (int j = 3; j >= 0 && dim != 0; j--) {
		auxStr[j] = buff[dim - 1];
		dim--;
	} 
	for (int k = 0; k < 4; k++) {
		buff[k] = auxStr[k];
	}
}

void printmem(char * parameters) {

    long direc = -1;

    int error = sscanf(parameters,"%ld", &direc);
    
    if (error != 1 || direc < 0) {
        printError("Error en parseo de parametros \n");
        return;
    }
   
    
    unsigned char * mem=(unsigned char *)direc;

    printf("Direccion inicial : %ld",direc);
    
    printf("\n");
    printf("siguientes 32 bytes en direcciones relativas");
    char buff[5] = {0};
    for (int i = 0; i < 32; i++) {
        int dim = uintToBase(mem[i], buff, 16);
        hexaTo2(buff, dim); 
        if (i % 8 == 0) {
            putChar('\n');
        }
        changeLetterColor(0xFEAB8A);
        printf("%d ", i);
        if (i < 10) {
            putChar(' ');
        }
        changeLetterColor(DEFAULT_LETTER_COLOR);
        printf(": %s   ", buff);
    }
    printf("\n");
}

// Inforeg
static char * regs[] = {
	"R15:   ", "R14:   ", "R13:   ", "R12:   ", "R11:   ", 
	"R10:   ", "R9:    ", "R8:    ", "RSI:   ", "RDI:   ", 
	"RBP:   ", "RDX:   ", "RCX:   ", "RBX:   ", "RAX:   ", 
	"RIP:   ", "CS:    ", "FLAGS: ", "RSP:   "
	};

void inforeg() {
    uint64_t memory[19];
    char buff[9] = {0};
    getRegisters(memory);
    for (int i = 0; i < 19; i++) {
        uintToBase(memory[i], buff, 16);
        baseToHexa(buff);
        printf("%s %s", regs[i], buff);
        if (i % 2 == 0) {
            putChar('\n');
        } else {
            printf("            ");
        }
    }
}

// CPU Info
int CPUFamily(int chipvalue);
int CPUModel(int chipvalue);

//https://software.intel.com/content/www/us/en/develop/download/intel-64-and-ia-32-architectures-sdm-combined-volumes-1-2a-2b-2c-2d-3a-3b-3c-3d-and-4.html
//la doc para descifrar que significan los numeros, el codigo es mio.

int CPUModel(int chipvalue){

    //The actual processor model is derived from the Model, Extended Model ID and Family ID fields. 
    //If the Family ID field is either 6 or 15, the model is equal to the sum of the Extended Model ID field shifted left by 4 bits and the Model field.
     //Otherwise, the model is equal to the value of the Model field.
   

    //consigo el numero compuesto 

    int FamilyID=chipvalue  & 0x00000F00;       //mask y corrimiento para conseguir los valores separados en distintas variables
    FamilyID=FamilyID>>8;

    int ModelID= chipvalue  & 0x000000F0;
    ModelID=ModelID>>4;

    int ExtendedModelID=chipvalue & 0x000F0000;
    ExtendedModelID=ExtendedModelID>>16;

    if (FamilyID==6||FamilyID==15)
    {
        ExtendedModelID=ExtendedModelID<<4;
        return ExtendedModelID + ModelID;
    }
    return ModelID; 


}

int CPUFamily(int chipvalue) {

//The actual processor family is derived from the Family ID and Extended Family ID fields.
    // If the Family ID field is equal to 15, the family is equal to the sum of the Extended Family ID and the Family ID fields.
    // Otherwise, the family is equal to value of the Family ID field.

    //consigo el numero compuesto 

    int FamilyID=chipvalue  & 0x00000F00;       //mask y corrimiento para conseguir los valores separados en distintas variables
    FamilyID=FamilyID>>8;

    int ExtendedFamilyID=chipvalue & 0x0FF00000;
    ExtendedFamilyID=ExtendedFamilyID>>20;

    if (FamilyID==15){
        return ExtendedFamilyID + FamilyID;
    }
    return FamilyID;

}

void printCPUInfo() {
    int cpuval=cpuModel();
    int Family=CPUFamily(cpuval);
    int Model=CPUModel(cpuval);

    printf("CPU Vendor is: %s ,the procesor family ID is %d and the model ID is %d\n",cpuVendor(),Family,Model);
}


void printError(char * str) {
	changeLetterColor(0xFF0000);
    printf(str);
    changeLetterColor(DEFAULT_LETTER_COLOR);
}

void printBlock() {
    changeBackgroundColor(0x808080);
    putChar(' ');
    changeBackgroundColor(DEFAULT_BACKGROUND_COLOR);
}
