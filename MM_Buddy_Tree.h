/************************************************************/
#define SIZE 15		//Cantidad de nodos que posee el Arbol
/*************************************************************/

/************************************************************************/

/************************************************************************/
typedef int point;	//REDEFINIRLO PARA EL MANEJO DE PUNTEROS A LA MEMORIA
/************************************************************************/

/****************************ASIGNAR MEMORIA**********************************************/	
/*
*	Se le indica un tamano de memoria contigua para que se retorne un puntero a dicho bloque
*	En caso de no encontrarlo retorna 0 (redefinirlo para cuando maneje punteros)
******************************************************************************************/
point mallok(int size);		 

/****************************LIBERAR MEMORIA**********************************************/
/*
*	Libera el puntero a la memoria indicado
*	Tener en cuenta que al 17/09/2020 no posee indicador de errores en caso de no encontrar el puntero deseeado.
******************************************************************************************/
void fri(point p);