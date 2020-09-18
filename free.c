char occupied_left;        //Indica si su hijo izquierdo esta ocupado
char occupied_right;    //Indica si su hijo derecho esta ocupado
char data;                //Indica que guarda la informacion

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
    if (p % 2 == 0)
	    friRec(p, &tree[0]);
}

typedef struct node {
	struct node * left;		//Puntero al padre y sus hijos.
	struct node * right;
	struct node * father;	
	point value;			//Valor del puntero al que apunta
	char occupied_left;        //Indica si su hijo izquierdo esta ocupado
    char occupied_right;			//Indica si sus hijos estan ocupados
	char data;				//Indica que guarda la informacion
	int size;				//Indica el tamano libre que representa
} node;