#include "MM_Buddy_Tree.h"
#define LEFT 2
#define RIGHT 1
#define ROOT -1
#include <stdio.h>
		
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

/*
node * newTree(int size){
	node tree[sizeof(struct node) * size];
	return tree;
}
*/

node tree[sizeof(node) * SIZE];
char memory[MAX_MEMORY];
static int index = 0;

static node * InitializeTree();
static node * newTreeRec(int size, node * father, int right_or_left);

static node * InitializeTree(){
    return  newTreeRec(MAX_MEMORY, NULL, ROOT);
}

static node * newTreeRec(int size, node * father, int right_or_left){
    //int right_or_left , 0 for left, 1 for right
    if (size == 0 ) {
        return NULL;
	}
	node * Node = &tree[index++];
	Node->father = father;
	Node->occupied_left = 0;
	Node->occupied_right = 0;
	Node->data = 0;
	Node->size = size;

	if (right_or_left == ROOT)
		Node->value = memory;
	else {
		Node->value = father->value;
		if (right_or_left == RIGHT)
			Node->value += size;
	}
	Node->left = newTreeRec(size/2, Node, LEFT);
	index++;
	Node->right = newTreeRec(size/2, Node, RIGHT);
	
	return Node;
}

static point mallokRec(int size, node * n){
	if (size > n -> size || (n -> data == 0 && size > n->size)) {	//Si no entra en el nodo regresa
		return 0;
	}
	if (size <= (n -> size / 2)) {				//Reviso si entro al siguiente nivel
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
	if (! (n -> occupied_left || n -> occupied_right))		//Si uno de sus hijos esta ocupado 
	{														//no lo puedo guardar ahi
		//n -> occupied_left = 1;
		//n -> occupied_right = 1;
		n -> data = 1;
		return n -> value;  
	}
	return 0;
}

point mallok(int size){
	return mallokRec(size, tree);
}

static int friRec(point p, node * n){
    int son_changed;
    if (p < n -> value || (n->occupied_left == 0 && n->occupied_right == 0 && n->data == 0)) {
        return 0;
    }

    if (p >= (n -> value + n -> size / 2)) {
        son_changed = friRec(p, n->right);
        if (son_changed) 
                n->occupied_right = 0;
                if (n->occupied_left == 0) 
                    return 1;
    } else if (p == n->value) {
        if (n->data == 1) {
            n->data = 0;
            return 1;
        } else {
            son_changed = friRec(p, n->left);
            if (son_changed) 
                n->occupied_left = 0;
                if (n->occupied_right == 0) 
                    return 1;
        }
    } else {
        son_changed = friRec(p, n->left);
        if (son_changed) 
            n->occupied_left = 0;
            if (n->occupied_right == 0) 
                return 1;
    }
    return 0;
}

void fri(point p){
	friRec(p, &tree[0]);
}

void printNode(node * Node) {
	putchar('\n');
	printf("Node: %p\n", Node->value);
	printf("Left: %p\n", Node->left->value);
	printf("Right: %p\n", Node->right->value);
	if (Node->father != NULL) 
		printf("Father: %p\n", Node->father->value);
	printf("Occupied Left: %d\n", Node->occupied_left);
	printf("Occupied Right: %d\n", Node->occupied_right);
	printf("Data: %d\n", Node->data);
	printf("Size: %d\n", Node->size);
	putchar('\n');
}