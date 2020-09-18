#include "MM_Buddy_Tree.h"
		
typedef struct node
{
	struct node * left;		//Puntero al padre y sus hijos.
	struct node * right;
	struct node * father;	
	point value;			//Valor del puntero al que apunta
	char occupied;			//Indica si sus hijos estan ocupados
	char data;				//Indica que guarda la informacion
	int size;				//Indica el tamano libre que representa
}node;

node tree[sizeof(node) * SIZE];
/*
node * newTree(int size){
	node tree[sizeof(struct node) * size];
	return tree;
}
*/
point mallokRec(int size, node * n){
	if (size > n -> size) {
		return 0;
	}
	if (size <= (n -> size / 2)) {
		point p = mallokRec(size, n -> left);
		return (p != 0? p : mallokRec(size, n -> right));
	}
	if (! n -> occupied)
	{
		n -> occupied = 1;
		n -> data = 1;
		n -> father -> occupied = 1;
		return n -> value;  
	}
	return 0;
}

point mallok(int size){
	return mallokRec(size, &tree[0]);
}

void friRec(point p, node * n){
	if (p < n -> value) {			//ningun puntero del arbol es mas chico que el de la raiz
		return;
	}

	if (p >= (n-> value + n -> size / 2)){
		friRec(p, n -> right);
	}
	if (p == n -> value) {
		if (n -> data)			//si es el puntero me fijo de liberar todo el tamano del mismo
		{
			n -> occupied = 0;
			n -> data = 0;
			return;
		}
	}
	friRec(p, n -> left);
}

void fri(point p){
	friRec(p, &tree[0]);
}





