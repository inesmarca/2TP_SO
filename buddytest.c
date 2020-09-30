
#include <stdint.h>
#include <stdio.h>
#include <string.h>



#define LEFT 2
#define RIGHT 1
#define ROOT -1


#define MAX_BLOCKS 128
#define MAX_MEMORY 1024 * 1024 * 10 * 0.8 //Should be around 80% of memory managed by the MM
#define MM_SIZE ( 1024 * 1024 * 10 )
#define PAGESIZE  ( 1024 * 4 ) 




#define NUMBEROFPAGES ( MM_SIZE ) / ( PAGESIZE )

/************************************************************/
#define MAX_CANT_LEVELS 12		// Cantidad de niveles maximo
#define pwrtwo(x) (1 << (x))
#define SIZE pwrtwo(MAX_CANT_LEVELS + 1) - 1
/*************************************************************/

/************************************************************************/

/************************************************************************/
typedef void * point;	//REDEFINIRLO PARA EL MANEJO DE PUNTEROS A LA MEMORIA
/************************************************************************/

/****************************ASIGNAR MEMORIA**********************************************/	
/*
*	Se le indica un tamano de memoria contigua para que se retorne un puntero a dicho bloque
*	En caso de no encontrarlo retorna 0 (redefinirlo para cuando maneje punteros)
******************************************************************************************/
point malloc_buddy(int size);		 

/****************************LIBERAR MEMORIA**********************************************/
/*
*	Libera el puntero a la memoria indicado
*	Tener en cuenta que al 17/09/2020 no posee indicador de errores en caso de no encontrar el puntero deseeado.
******************************************************************************************/
void free_buddy(point p);




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
      mm_rqs[rq].address = malloc_buddy(mm_rqs[rq].size); // TODO: Port this call as required
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
        free_buddy(mm_rqs[i].address);  // TODO: Port this call as required
    
    printf("loop Done");
  } 
}
int main(int argc, char const *argv[])
{
   test_mm();
    return 0;
}



		
typedef struct node
{
	struct node * left;		//Puntero al padre y sus hijos.
	struct node * right;
	struct node * father;	
	point value;			//Valor del puntero al que apunta
	char occupied_left;		//Indica si su hijo izquierdo esta ocupado
	char occupied_right;	//Indica si su hijo derecho esta ocupado
	char data;				//Indica que guarda la informacion
	int size;				//Indica el tamano libre que representa
} node;

node tree[SIZE] = {{0}};
static void * MMemory[NUMBEROFPAGES][PAGESIZE];
static int index2 = 0;
int initialized=0;

static node * InitializeTree();
static node * newTreeRec(int size, node * father, int right_or_left, int cant_levels);

static node * InitializeTree(){
    return  newTreeRec(MM_SIZE, NULL, ROOT, MAX_CANT_LEVELS);
}

static node * newTreeRec(int size, node * father, int right_or_left, int cant_levels){
    if (cant_levels == 0) {
        return NULL;
	}
	node * Node = &tree[index2++];
	Node->father = father;
	Node->occupied_left = 0;
	Node->occupied_right = 0;
	Node->data = 0;
	Node->size = size;

	if (right_or_left == ROOT)
		Node->value = MMemory;
	else {
		Node->value = father->value;
		if (right_or_left == RIGHT)
			Node->value += size;
	}
	Node->left = newTreeRec(size/2, Node, LEFT, cant_levels - 1);
	index2++;
	Node->right = newTreeRec(size/2, Node, RIGHT, cant_levels - 1);
	
	return Node;
}

static point mallokRec(int size, node * n){
	if (size > n -> size || n -> data == 1 ) {	//Si no entra en el nodo regresa
		return 0;
	}
	if (size <= (n -> size / 2)) {				//Reviso si entro al siguiente nivel
		if (n->left == NULL || n->right == NULL) {
			n -> data = 1;
			return n -> value; 
		}
		
		point p;
		if (n -> left -> data == 0) {			//////////////////////////////////////////
			p = mallokRec(size, n -> left);		/**/
			if (p != 0) {						/**/
				n -> occupied_left = 1;			/**/
				return p;						/**/
			}									/*Me fijo a que hijo entrar*/
		}										/*si es que alguno de los 2 esta ocupado*/
		if (n -> right -> data == 0) {			/*Si lo aloje indico que hijo esta ocupado*/
			p = mallokRec(size, n -> right);	/**/
			if (p != 0) {						/**/
				n -> occupied_right = 1;		/**/
				return p;						/**/
			}									/**/
		}										//////////////////////////////////////////
		return 0;
	}
	if (!(n -> occupied_left || n -> occupied_right) || n->left == NULL)		//Si uno de sus hijos esta ocupado 
	{														//no lo puedo guardar ahi
		//n -> occupied_left = 1;
		//n -> occupied_right = 1;
		n -> data = 1;
		return n -> value;  
	}
	return 0;
}

point malloc_buddy(int size){
	if (initialized == 0){
		InitializeTree();
		initialized++;
	}
	
	return mallokRec(size, tree);
}

static int friRec(point p, node * n){
    int son_changed;
	
    if (n == NULL || p < n -> value || (n->occupied_left == 0 && n->occupied_right == 0 && n->data == 0))
        return 0;

    if (p >= (n -> value + n -> size / 2)) {
        son_changed = friRec(p, n->right);
        if (son_changed) {
            n->occupied_right = 0;
            if (n->occupied_left == 0) 
                return 1;
		}
    } else if (p == n->value) {
        if (n->data == 1) {
            n->data = 0;
            return 1;
        } else {
            son_changed = friRec(p, n->left);
            if (son_changed) {
                n->occupied_left = 0;
                if (n->occupied_right == 0) 
                    return 1;
			}
        }
    } else {
        son_changed = friRec(p, n->left);
        if (son_changed) {
            n->occupied_left = 0;
            if (n->occupied_right == 0) 
                return 1;
		}
    }

    return 0;
}

void free_buddy(point p){
	friRec(p, tree);
}