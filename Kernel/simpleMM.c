// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <simpleMM.h>
#include <MM.h>
#define NUMBEROFPAGES ( MM_SIZE ) / ( PAGESIZE )

static int pages_required_finder(int size);
static int index_finder(int pages_required);

static void * MMemory = (void *)INITIAL_DIR;

static int occupied[NUMBEROFPAGES] = {0};
static int size_of_allocation [NUMBEROFPAGES]={0};

static char * getPos(int index) {
    return (char *)MMemory + index*(PAGESIZE);
}

static int pages_required_finder(int size){
    int pages_required=size/(PAGESIZE); 
    if (size%(PAGESIZE)!=0)
        pages_required++;

    return pages_required;
}

void * malloc_simple(int size){
    if (size<=0)                    //validar entradas
        return NULL; 

    int pages_required=pages_required_finder(size); //figure out cuantas paginas necesito

    int index=index_finder(pages_required); //encontrar index si existe y validar error
    if (index==-1)
        return NULL;

    size_of_allocation[index]=size;         //marcar el tamaño de la entrada

    for (int i = 0; i <= pages_required; i++) //marcar la salida como ocupada
        occupied[index+i]=1;

    return getPos(index);
}

static int index_finder(int pages_required){
    for (int i = 0; i < (NUMBEROFPAGES) && i+pages_required<=(NUMBEROFPAGES); i++) {
        if (occupied[i]==0) {
            char valid_found=1;
            for (int j = 1; i+j<(NUMBEROFPAGES)  && valid_found && pages_required>1 && j+1<pages_required; j++) {
                if (occupied[i+j]!=0) 
                    valid_found=0;
            }

            if (valid_found)
                return i;
        }
    }

    return -1;
}

void free_simple(char * puntero){
    //confio de que no me estan mandando basura 
    int aux = puntero- (char *)MMemory;                    //FUERTES DUDAS CON ESTO
    aux /= (PAGESIZE);
    int pages_required=pages_required_finder(size_of_allocation[aux]);
    size_of_allocation[aux]=0;

    for (int i = 0; i <=pages_required; i++)
        occupied[aux+i]=0;
}

int getTotalMem_Simple(){
    return MM_SIZE;
}

int getUsedMem_Simple(){
    int counter=0;
    for (int i = 0; i < (NUMBEROFPAGES); i++)
    {
        if(occupied[i]==1){
            counter++;
        }
    }
    return counter*(PAGESIZE);
}