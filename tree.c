#include "MM_Buddy_Tree.h"
#define LEFT 0
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
}node;

node tree_nodes[sizeof(node) * SIZE];
char memory[MAX_MEMORY];

node * InitializeTree();
node * newTreeRec(int sizetree,int size,int index,node * father,int right_or_left);

node * InitializeTree(){
	static int index=0;
    return  newTreeRec(SIZE,MAX_MEMORY,index,NULL,ROOT);
}

node * newTreeRec(int sizetree,int size,int index,node * father,int right_or_left){
    //int right_or_left , 0 for left, 1 for right
    if (sizetree/2 ==0 ) {
        return NULL;
    }
    else
    {
        node * Node=&tree_nodes[index++];
        Node->left=newTreeRec(sizetree/2,size/2,index,Node,LEFT);
        Node->right=newTreeRec(sizetree/2,size/2,index,Node,RIGHT);
        Node->father=father;
        switch (right_or_left)
        {
        case LEFT:
            Node->value=father->value;
            break;

        case RIGHT:
            Node->value=(father->value)+size;
            break;
        case ROOT:
            Node->value=memory;
            break;
        }

        Node->occupied_left=0;
        Node->occupied_right=0;
        Node->data=0;
        Node->data=size;
        return Node;
    }
    
}
