#include <libC.h>
#include <libFun.h>
#include <sysLib.h>

#include <philo.h>

#define MAX_DIGITS 20
#define MAX_READ 255

extern int cpuModel();
extern char * cpuVendor();
extern void getRegisters(uint64_t * buff) ;
extern int getTemperature();
extern void getTime(int * buff);

int isVocal(char c) {
    return c == 'a' || c == 'A' || c == 'e' || c == 'E' || c == 'i' || c == 'I' || c == 'o' || c == 'O' || c == 'u' || c == 'U';
}

// sem
void sem(){                                 //FALTA TESTEAR
    printf("Listado de semaforos:\n");
	infoSem * buff[MAX_SEMS];
	int active_sems = getListSem(buff);
    printf("Semaforos activos: %d\n", active_sems);
	for (int i = 0; i < active_sems; i++){
        printf("holaa");
        printf("sem name: %s\n", buff[i] -> name);
        printf("sem value: %d\n", buff[i] -> value);

        for (int j = 0; j < buff[i] -> cant_blocked; j++)
            printf("PID del proceso bloqueado: %d\n", buff[i]->blocked_pids[j]);
	}
}

// pipe
void pipeInfo() {
	infoPipe * buff[MAX_SEMS];
    int cant = getPipeList(buff);

    printf("%d pipes opened\n", cant);

    for (int i = 0; i < cant; i++)
        printf("ID: %d, NWRITE: %d, NREAD: %d, BLOCKED PID: %d %d\n", buff[i]->id, buff[i]->nwrite, buff[i]->nread, buff[i]->pids_blocked[0], buff[i]->pids_blocked[1]);

    return;
}

// wc
void wc() {
    char c;
    int cant = 0;
    while((c = getChar()) != 0)
        putChar(c);
    
    printf("%d ", cant);
}

// ps
void ps() {
	infoPCB * buff[MAX_PROCESS];
    int cant = getListPCB(buff);

    printf("%d process opened\n", cant);

    int foreground;

    for (int i = 0; i < cant; i++) {
        foreground = 1;
        
        if (buff[i]->fd[STDIN] != -1) 
            foreground = 0;

        printf("PID: %d, NAME: %s, PRIORITY: %d, ACTIVE: %d STACK POINTER: %s, BASE POINTER: %s, FOREGROUND: %d\n", buff[i]->pid, buff[i]->name, buff[i]->priority, buff[i]->state - 1, buff[i]->stackPointer, buff[i]->basePointer, foreground);
    }
}

// filter
void filter() {
    char * buff = malloc(255);
    int dim = scanf("%s", buff);
    char * aux = malloc(dim);
    int i, j = 0;

    for(i = 0; buff[i] != 0; i++) {
        if(!isVocal(buff[i])) {
            aux[j] = buff[i];
            j++;
        }
    }
    aux[j] = 0;
    strcpy(buff, aux);
    free(aux);
    printf(buff);
    free(buff);
}

// cat
void cat() {
	char * buff = malloc(255);
	int cant = read(STDIN, buff, 255);
    int pos = cant - 1;
	printf("%s", buff);
    while (cant == 255 && buff[pos] != 0) {
        cant = read(STDIN, buff, 255);
	    printf("%s", buff);
        pos += cant - 1;
    }
    free(buff);
}

// mem
void mem() {
    int buff[2];
    memState(buff);
    printf("Total Space: %d\nSpace Used: %d\n", buff[0], buff[1]);
}
//kill
void kill_shell(int argc,char *argv[]){
    if (argc!=1)
    {
        printError("invalid ammount of arguments");
    }
    int pid=atoi(argv[0]);
    kill(pid,0);
    
}
//block
void block_shell(int argc,char *argv[]){
    if (argc!=1)
    {
        printError("invalid ammount of arguments");
    }
    int pid=atoi(argv[0]);
    kill(pid,1);
}
//unblock
void unblock_shell(int argc,char *argv[]){
    if (argc!=1)
    {
        printError("invalid ammount of arguments");
    }
    int pid=atoi(argv[0]);
    kill(pid,2);
}
// loop
void loop() {
    while(1) {
        printf("%d ", getpid());
        for (int i = 1000000000; i > 0; i--);
    }
}

// philosophers
void philo(){
    philosphers();
}
//cat para la shell(pointer type)
void nice_shell(int argc,char * argv[]){
    if (argc!=2)
    {
        printError("invalid ammount of arguments");
    }
    int pid=atoi(argv[0]);
    int priority=atoi(argv[1]);
    nice(pid,priority);
}
// create process on background
int createBackground(const char * name, void * func, int priority, int argc, char * argv[]) {
	int aux[2] = {-1, STDOUT};
	return create(name, func, priority, aux, argc, argv);
}



void printTime(){
    int buff[3];
    getTime(buff);
    printf("%d:%d:%d\n", buff[0], buff[1], buff[2]);
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
        changeColor(0xFEAB8A, DEFAULT_BACKGROUND_COLOR);
        printf("%d ", i);
        if (i < 10) {
            putChar(' ');
        }
        changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
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
    changeColor(0xFF0000, DEFAULT_BACKGROUND_COLOR);
    printf(str);
    changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
}

void printBlock() {
    changeColor(DEFAULT_LETTER_COLOR, 0x808080);
    putChar(' ');
    changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
}
