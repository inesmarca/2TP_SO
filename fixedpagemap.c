#include <stdio.h>
#include <string.h>

#define PAGESIZE 10
#define NUMBEROFPAGES 5

int pages_required_finder(int size);
char * malloc_mio(int size);
int index_finder(int pages_required);
void free(char * puntero);

char memory[NUMBEROFPAGES][PAGESIZE];
int occupied[NUMBEROFPAGES]={0};
int size_of_allocation [NUMBEROFPAGES]={0};

int pages_required_finder(int size){

    int pages_required=size/PAGESIZE; 
        if (pages_required%PAGESIZE!=0)
        {
            pages_required++;
        }
        return pages_required;

}

char * malloc_mio(int size){
    if (size<=0)                    //validar entradas
    {
        return NULL; 
    }

    int pages_required=pages_required_finder(size); //figure out cuantas paginas necesito

    int index=index_finder(pages_required); //encontrar index si existe y validar error
    if (index==-1)
    {
        return NULL;
    }

    
    size_of_allocation[index]=size;         //marcar el tamaño de la entrada

    for (int i = 0; i <= pages_required; i++) //marcar la salida como ocupada
    {
        occupied[index+i]=1;
    }


    return memory[index];
}

int index_finder(int pages_required){
    for (int i = 0; i < NUMBEROFPAGES && i+pages_required<=NUMBEROFPAGES; i++)
    {
        if (occupied[i]==0)
        {
            char valid_found=1;
            for (int j = 1; i+j<NUMBEROFPAGES  && valid_found && pages_required>1; j++)
            {
                if (occupied[i+j]!=0) 
                {
                    valid_found=0;
                }
                
                
            }
            if (valid_found)
            {
                return i;
            }
        }
        
    }
    return -1;

}

void free(char * puntero){
    //confio de que no me estan mandando basura 
    int aux =puntero- (char *)memory;                    //FUERTES DUDAS CON ESTO
    aux/=PAGESIZE;
    int pages_required=pages_required_finder(size_of_allocation[aux]);
    size_of_allocation[aux]=0;

    for (int i = 0; i <=pages_required; i++)
    {
        occupied[aux+i]=0;
    }
}

int main(int argc, char const *argv[])
{
    // printf("arrancando \n");
    // char * testing = malloc_mio(25);
    // strcpy(testing,"01234567891234567890123");
    // printf("%p, %s, %d, %d \n",testing,testing,occupied[0],size_of_allocation[0]);
    // if (malloc_mio(6000)==NULL)
    // {
    //     printf("failed succesully \n");
    // }
    // char *testing2=malloc_mio(30);
    // if(testing2==NULL){
    //     printf("failed succesully2 \n");
    // }
    // strcpy(testing2," DEBERIA VER ESTO2");
    // printf("%p, %s, %d, %d \n",testing2,testing2,occupied[1],size_of_allocation[1]);
    // if (testing!=NULL)
    // {
    //     printf("passed succesully \n");
    // }
    // free(testing);
    // printf("%p, %s, %d, %d \n",testing,testing,occupied[0],size_of_allocation[0]);
    // free(testing2);
    // printf("%p, %s, %d, %d \n",testing2,testing2,occupied[1],size_of_allocation[1]);

    
    return 0;
}


