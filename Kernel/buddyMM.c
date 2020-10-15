#include <buddyMM.h>
#include <MM.h>

#define LEFT 2
#define RIGHT 1
#define ROOT -1
		
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

static node tree[SIZE] = {{0}};
static void * MMemory = (void *)INITIAL_DIR;
static int index = 0;
static int initialized=0;

static node * InitializeTree();
static node * newTreeRec(int size, node * father, int right_or_left, int cant_levels);

static node * InitializeTree(){
    return  newTreeRec(MM_SIZE, NULL, ROOT, MAX_CANT_LEVELS);
}

static node * newTreeRec(int size, node * father, int right_or_left, int cant_levels){
    if (cant_levels == 0) {
        return NULL;
	}
	node * Node = &tree[index++];
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
	index++;
	Node->right = newTreeRec(size/2, Node, RIGHT, cant_levels - 1);
	
	return Node;
}

static point mallokRec(int size, node * n){
	if (size > n -> size || n->data==1) {	//Si no entra en el nodo regresa
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

int getTotalMem_Buddy(){
    return MM_SIZE;
}

int getUsedMem_Buddy(){
	int suma=0;
	for (int i = 0; i < SIZE; i++)
	{
		if(tree[i].data==1){
			suma+=tree[i].size;
		}
	}
	return suma;
}
