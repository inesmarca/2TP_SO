// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <test_util.h>
#include <libC.h>

#define MAX_BLOCKS 200
#define MAX_MEMORY 1024 * 1024 * 128 * 0.8 //Should be around 80% of memory managed by the MM

static int killed = 0;
static int fd[2] = {STDIN, STDOUT};

typedef struct MM_rq{
  void *address;
  uint32_t size;
}mm_rq;

static  void test(int argc, char ** argv){
  while (1)
  {  
    mm_rq mm_rqs[MAX_BLOCKS];
    uint8_t rq;
    uint32_t total;
    rq = 0; 
    total = 0;
    printf("TESTEANDO\n");
    // Request as many blocks as we can
    while(rq < MAX_BLOCKS && total < MAX_MEMORY){
      mm_rqs[rq].size = GetUniform(MAX_MEMORY - total - 1) + 1;
      mm_rqs[rq].address = malloc(mm_rqs[rq].size); // TODO: Port this call as required
  //TODO: check if NULL
      total += mm_rqs[rq].size;
      rq++;
    }
    printf("TESTEANDO\n");

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL) {
        memset(mm_rqs[i].address, i, mm_rqs[i].size); // TODO: Port this call as required
      }

    printf("TESTEANDO\n");
    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        if(!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          printf("ERROR!\n"); // TODO: Port this call as required
        }

    printf("TESTEANDO\n");
    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address != NULL)
        free(mm_rqs[i].address);  // TODO: Port this call as required
    printf("FUNCIONANDO\n");
    if (killed)
    {
      kill(getpid(), 0);
    }
    
  }

}


void test_mm(){
  printf("INICIANDO\n");

  while (1) {
    create("testmm", test, 2, fd, 0, 0, 0);
    int k = getChar();
    if (k == 'q')
    {
      killed = 1;
      return;
    }
    
  } 
}
