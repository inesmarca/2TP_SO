
#include <stdint.h>
#include <stdio.h>
#include <string.h>



#define MAX_BLOCKS 128
#define MAX_MEMORY 1024 * 1024 * 10 * 0.8 //Should be around 80% of memory managed by the MM
#define MM_SIZE ( 1024 * 1024 * 10 )
#define PAGESIZE  ( 1024 * 4 ) 




#define NUMBEROFPAGES ( MM_SIZE ) / ( PAGESIZE )




static uint32_t m_z = 362436069;
static uint32_t m_w = 521288629;

uint32_t GetUint(){
  m_z = 36969 * (m_z & 65535) + (m_z >> 16);
  m_w = 18000 * (m_w & 65535) + (m_w >> 16);
  return (m_z << 16) + m_w;
}

uint32_t GetUniform(uint32_t max){
  uint32_t u = GetUint();
  return (u + 1.0) * 2.328306435454494e-10 * max;
}

uint8_t memcheck(void *start, uint8_t value, uint32_t size){
  uint8_t *p = (uint8_t *) start;
  uint32_t i;

  for (i = 0; i < size; i++, p++)
    if (*p != value)
      return 0;

  return 1;
}



int pages_required_finder(int size);
int index_finder(int pages_required);

static void * MMemory[NUMBEROFPAGES][PAGESIZE];

int occupied[NUMBEROFPAGES] = {0};
int size_of_allocation [NUMBEROFPAGES]={0};

char * getPos(int index) {
    return (char *)MMemory + index*(PAGESIZE);
}

int pages_required_finder(int size){
    int pages_required=size/(PAGESIZE); 
    if (size%(PAGESIZE)!=0)
        pages_required++;

    return pages_required;
}
void weird_stop(void*aux){

    int x=0;
}


char * malloc_simple(int size){
    if (size<=0)                    //validar entradas
        return NULL; 

    int pages_required=pages_required_finder(size); //figure out cuantas paginas necesito

    int index=index_finder(pages_required); //encontrar index si existe y validar error
    if (index==-1)
        return NULL;

    size_of_allocation[index]=size;         //marcar el tamaño de la entrada

    for (int i = 0; i <= pages_required; i++) //marcar la salida como ocupada
        occupied[index+i]=1;
    void * test=getPos(index);
    if (test)
    {
        weird_stop(test);
    }
    
    return test;
}


int index_finder(int pages_required){
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



typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

void test_mm(){
  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  printf("running test\n");
  char stopper=1;
  while (stopper){
    rq = 0;
    total = 0;

    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = malloc_simple(mm_rqs[rq].size); // TODO: Port this call as required
//TODO: check if NULL
    printf("alloc %p %d \n", mm_rqs[rq].address, mm_rqs[rq].size);

      total += mm_rqs[rq].size;
      rq++;
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL) {
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required
        
        printf("write %p %d \n", mm_rqs[i].address, mm_rqs[i].size);
       
      }

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL){
        //*(char*)(mm_rqs[0].address)=5;
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
            printf("err %p %d \n", mm_rqs[i].address, mm_rqs[i].size);
                            // TODO: Port this call as required
            stopper =0;

        }
    }
    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        free_simple(mm_rqs[i].address);  // TODO: Port this call as required
    
    printf("loop Done");
  } 
}
int main(int argc, char const *argv[])
{
   test_mm();
    return 0;
}
