#include "MM_Buddy_Tree.h"
		
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
}node;

node tree[sizeof(node) * SIZE];
/*
node * newTree(int size){
	node tree[sizeof(struct node) * size];
	return tree;
}
*/


point mallokRec(int size, node * n){
	if (size > n -> size || n -> data == 0) {	//Si no entra en el nodo regresa
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
	return mallokRec(size, &tree[0]);
}

int friRec(point p, node * n){
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





