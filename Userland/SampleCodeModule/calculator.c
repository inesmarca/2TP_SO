#include <calculator.h>
#include <programHandler.h>
#include <programs.h>
#include <libC.h>
#include <sysLib.h>

char * eval( char * first,int first_dim,char * second,int second_dim,char operation);

long stringtoLong(char * number);

char * evaluatePosfija(char * input);

int findDim(const char * num);

int precedence(char token1, char token2);

int isToken(char token);

int infijaToPosfija(char * input, char * output);

char * longtoString(long number,int dim,char * output);

char * malloc2();

char * ajustardecimales(char * input);

static char input[WIDTH/8] = {0};
static int pos = 0;

#define MAX_NUM_IN_EXPRESION 20
#define MAX_NUMBER_LENGTH 20
#define MAX_INPUT_LENGTH 100
#define CANT_DECIMALES_INTER_OPERACIONES 6 
#define CANT_DECIMALES_OUTPUT 4 //siempre tiene que ser menos que cant_decimales_inter_operaciones

void initCalculator() {
    // prompt
    setCursor(0, SCREEN_HEIGHT - LETTER_HEIGHT);
    printf("Para el correcto funcionamiento del programa ingrese su expresion separada con espacios\n");
    printf("ej: ( 2 + 3 ) * 5 =\n");
    printf("las operaciones soportadas incluyen : + , - , * , / y el uso de parentesis\n");
    printf("para ejecutar la operacion presione la tecla = ");
    changeLetterColor(0xFF0000);
    printf("(recuerde dejar un espacio)\n");
    changeLetterColor(DEFAULT_LETTER_COLOR);
    printUser();
}

void calculator(char key) {
    if (key == '=') {
        putChar(key);
        putChar(' ');
        input[pos++] = key;
        char * result = runCalc(input);
        if (result != NULL) {
            printf("%s %s\n", key, result);
        } else {
            printf("%s\n", key);
        }                  
        input[0] = 0;
        pos = 0;
        printUser();
    } else if (key == DELETE) {
        if (pos != 0) {
            putChar(key);
            pos--;
            input[pos] = 0;
        }
    } else if (key == 'D') {  // borra la linea entera
        while (pos != 0) {
            putChar(DELETE);
            pos--;
        }
        input[pos] = 0;
    } else if (key != '\n') {
        putChar(key);
        input[pos++] = key;
        input[pos] = 0;
    }
}

char * runCalc(char * str) {
    char aux[MAX_INPUT_LENGTH];
    if (infijaToPosfija(str, aux)!=0)
    {
        return NULL;
    }
    
    char * result =evaluatePosfija(aux);

    if (result==NULL)
    {
        return NULL;
    }
    
    return ajustardecimales(result);
}


char * ajustardecimales(char * input){
    int index=0;
    while (input[index]!='.')
    {
        index++;
    }
    index+=CANT_DECIMALES_OUTPUT+1;
    if (input[index] - '0' >= 5)    //si tiene que hacer el redondeo
    {
      input[index] = 0;
      long aux = stringtoLong (input);
      if (aux >=0)
      {
          aux++;
      }
      else
      {
          aux--;
      }
      input = longtoString (aux, CANT_DECIMALES_OUTPUT, input);
     
    }
    input[index] = 0;
  //termina y deja CANT_DECIMALES _OUTPUT
  return input;
}


char * malloc2(){
    //esto es privado, no se deberia usar afuera del scope de la calculadora
    
    //  muy mala pseudo implementacion para poder conseguir memoria(necesito malloc. porque es tan dificil de implementar bien, me quiero matarrrrr)
    
    static char  freemem [MAX_NUM_IN_EXPRESION] [MAX_NUMBER_LENGTH];
    static int index=-1;
    index+=1;
    if (index>=MAX_NUM_IN_EXPRESION )
    {
        index=0;
    }
    for (int i = 0; i < MAX_NUMBER_LENGTH; i++)
    {
        freemem[index][i]=0;
    }
    //me aseguro que no venga basura porque puede molestar
    
    return freemem[index];
}





char * eval( char * first,int first_dim,char * second,int second_dim,char operation){
    
    long firstnum=stringtoLong(first);
    long secondnum=stringtoLong(second);
    long result=0;
    int result_dim=-1;//nunca puede ser un valor negativo, si sigue negativo al final hay un error
    
    
    while (first_dim<CANT_DECIMALES_INTER_OPERACIONES) //fijo  las dimensiones al minimo accurate decimales para poder operar bien
    {
        firstnum*=10;
        first_dim++;
    }
     while (second_dim<CANT_DECIMALES_INTER_OPERACIONES) //fijo  las dimensiones al minimo accurate decimales para poder operar bien
    {
        secondnum*=10;
        second_dim++;
    }
    switch (operation)
    {
    case '+':
        while (first_dim<second_dim) //igualo las dimensiones para poder operar bien
        {
        firstnum*=10;
        first_dim++;
        }
        while (second_dim<first_dim) //igualo las dimensiones para poder operar bien
        {
            secondnum*=10;
            second_dim++;
        }
            result+=firstnum;
            result+=secondnum;
            result_dim=first_dim;
            break;
     case '-':
        while (first_dim<second_dim) //igualo las dimensiones para poder operar bien
        {
        firstnum*=10;
        first_dim++;
        }
        while (second_dim<first_dim) //igualo las dimensiones para poder operar bien
        {
            secondnum*=10;
            second_dim++;
        }
        result=secondnum;
        result-=firstnum;
        result_dim=first_dim;
        break;

     case '*':
        result=firstnum;
        result*=secondnum;
        result_dim=first_dim+second_dim;
        while (result_dim>CANT_DECIMALES_INTER_OPERACIONES) //corrijo para que me queden solo Cant_decimal_inter_operaciones
        {
            result/=10;
            result_dim--;
        }
        break;
     case '/':
    if (firstnum==0)
    {
        printError("Cant divide by 0, invalid input");
        return NULL;
    }
    

     while (second_dim<2*CANT_DECIMALES_INTER_OPERACIONES) //fijo  las dimensiones al minimo accurate decimales para poder operar bien
    {
        secondnum*=10;
        second_dim++;
    }
        result=secondnum;
        result/=firstnum;
        result_dim=second_dim-first_dim;

        break;
    default:
        printError("Invalid symbol, invalid input");
        return NULL;

    }
   

 while (result_dim>CANT_DECIMALES_INTER_OPERACIONES) //corrijo para que me queden solo Cant_decimal_inter_operaciones
        {
            result/=10;
            result_dim--;
        }
    if(result==0){// si es 0 no se ve afectado por esto porque la dimension real cambio
        result_dim=0;
    }

    return longtoString(result,result_dim,malloc2()); //uso el first porque si se pasa de largo pisa el second. no se pueden instanciar arrays adentro del scope de la funcion porque van a fallar
}


char * longtoString(long number,int dim,char * output)
{ 
    //necesito un output limpio, no me pases vectores que tengan numeros antes porque no los detecta.
    //llamar a malloc 2 que asegura vectores de size correcto y limpios
    char aux [MAX_NUMBER_LENGTH];
    int index=0;
    int index2=0;
    int negative=0;

    if (number<0)
    {
        negative=1;
    }
    
    while (dim>=0||number!=0)
    {
         if(dim==0)
        {
          aux[index++]='.';
        }
        int auxiliar=number % 10;
        if(auxiliar<0){
            auxiliar*=-1; 
        }
        aux[index++] = auxiliar + '0';
        number /= 10;
        dim--;
    }
    if(dim==0){
          aux[index++]='.';
          aux[index++]='0';
    }

    if (negative)
    {
        aux[index++]='-';
    }
    
    while (index!=0)
    {
        output[index2++]=aux[--index];
    }

    index2=0;
    while (output[index2]!='.')
    {
        index2++;
    }
    index2++;
    for (int i = 0; i < CANT_DECIMALES_INTER_OPERACIONES; i++)
    {
        if (!(output[index2+i] >= '0' && output[index2 +i] <= '9'))
        {
            output[index2+i]='0';
        }
        
    }
    output[index2+CANT_DECIMALES_INTER_OPERACIONES]=0;
    return output;
        
}

//

long stringtoLong (char *number)
{
  char negative = 0;
  long int sum = 0;
  int index = 0;
  while (number[index] != 0)
    {
      if (number[index] == '-')
    {
      negative = 1;
    }
      if (number[index] >= '0' && number[index] <= '9')
    {
      sum *= 10;
      sum += (number[index] - '0');
    }
      index++;
    }
  if (negative)
    {
      sum *= -1;
    }
  return sum;

}


int findDim(const char * num){//retorna la cantidad de decimales que encuentra
    char decimal_found=0;
    int pos=0;
    int dim=0;
    while (num[pos]!=0)
    {
        
        if (decimal_found)
        {
            dim++;
        }
        if (num[pos]=='.')
        {
            decimal_found=1;
        }
        pos++;
    }
    return dim;

}


char * evaluatePosfija(char * input)
{
    int input_pos=0;//posicion en el array del input.
    int stack_pos=0;
    char * stack [MAX_NUM_IN_EXPRESION];//stack de strings o matriz de chars, depende de como lo pienses
    while (input[input_pos]!=0)
    {
        char current=input[input_pos++];
        if (isToken(current)&&input[input_pos]==' ')//se fija que sea token y chequea el caso de que sea un numero negativo
        {
            if(stack_pos<2){
                printError("Cant evaluate, stack not filled enough, invalid input");
                return NULL;
            }
            
            char * first=stack[--stack_pos];
            int first_dim=findDim(first);
            char * second=stack[--stack_pos];
            int second_dim=findDim(second);
            char * nulltest=eval(first,first_dim,second,second_dim,current);
            if (nulltest==NULL)
            {
                //testea que la evaluacion halla sido valida y no se cruzo co un operando que no reconoce
                return NULL;
            }
            
            stack[stack_pos++]=nulltest;
           
        }
        else if ((current>='0'&&current<='9')||(current=='-'&&input[input_pos]>='0'&&input[input_pos]<='9')) //number
        {
            char * auxiliar=malloc2();
            int index=0; 
            while (input[input_pos-1]!=' ')
            {
                auxiliar[index++]=input[input_pos++-1]; //copio el numero
            }
            auxiliar[index]=0;//pongo el 0 para terminate el string.
            stack[stack_pos++]=auxiliar;//meto el string formado al stack.
        }
    }
    if (stack_pos-1==0) //si se hizo bien, el stack solo tiene 1 elemento
    {
        char * answer=malloc2();
        answer=strcpy(answer,stack[0]);

        int answer_index=0;
        char has_point=0;               //resuelve el problema de que la expresion tenga un numero que nunca fue evaluado y le pone los decimales
        while (answer[answer_index++]!=0)
        {
            if (answer[answer_index]=='.')
            {
                has_point=1;
            }
            
        }
        if (!has_point)
        {
            answer[answer_index-1]='.';
            for (int i = 0; i <= CANT_DECIMALES_OUTPUT; i++)
            {
                answer[answer_index+i]='0';
            }
            answer[answer_index+CANT_DECIMALES_OUTPUT]=0;
            
        }
        
        
        
        return answer;
    }
    printError("Stack not empty, invalid input");
    return NULL;
   

}






int precedence(char token1, char token2) {
    switch (token1) {
    case '+':
        if (token2 == '+' || token2 == '-' || token2 == ')') 
            return 1;
        else
            return 0;
    case '-':
        if ( token2 == '+' || token2 == '-' || token2 == ')')
            return 1;
        else
            return 0;
    case '*':
        if (token2 == '+' || token2 == '-' || token2 == '*' || token2 == '/' || token2 == ')')
            return 1;
        else
            return 0;
    case '/':
        if ( token2 == '+' || token2 == '-' || token2 == '*' || token2 == '/' || token2 == ')')
            return 1;
        else
            return 0;
    case '(':
        return 0;

    
    default:
        printError("unrecognized character");
        break;
    
    }
    return -1;
}

int isToken(char token) {
    return (token == '+' || token == '-' || token == '*' || token == '/');
}


//toma la expresion infija y la trafsorma en posfija
int infijaToPosfija(char * input, char * output){
    int pos_output=0;
    int pos_input=0;
    int stack_pos=0;
    char stack [MAX_INPUT_LENGTH];
    while (input[pos_input]!= '=')
    {   

        char current=input[pos_input++];
        if (current=='(')
        {
             stack[stack_pos++]=current;
        }
        else if(current==')')
        {   
            if (stack_pos==0)
            {
                printError("Error in format conversion, invalid input");
                return -1;
            }
            
            

            current=stack[--stack_pos];// tiro el ) y pusheo hasta encontrar el bracket correspondiente
            while (current!='(')
            {
                output[pos_output++]=current;
                output[pos_output++]=' ';
                 if (stack_pos==0)
                {
                    printError("Error in format conversion, invalid input");
                    return -1;
                }
                current=stack[--stack_pos];
            }
            
        }
        
        
        else if (isToken(current)&&input[pos_input]==' ')//es un signo y no un numero negativo
        {
            while (stack_pos!=0&&precedence(stack[stack_pos-1],current))
            {
                char character=stack[--stack_pos];
                if (character!='('&&character!=')')
                {
                    output[pos_output++]=character;
                    output[pos_output++]=' ';
                }
                
                //printf("done2");
                
            }
            stack[stack_pos++]=current;
            
        }
        else if ((current>='0'&&current<='9')||(current=='-'&&input[pos_input]>='0'&&input[pos_input]<='9'))
            //si es un numero o si arranca con un - pero continua con un numero
            {
                int firstchar=1;
                int decimalpoints=0;
            while ((current>='0'&&current<='9')||(current=='.'&&decimalpoints==0)||(current=='-'&&firstchar))//mientras que sea numero o punto decimal o el menos de un negativo
            {
                if (current == '.')
                {
                    decimalpoints++;
                }
                
                firstchar=0;
                output[pos_output++]=current;
                current=input[pos_input++]; //sigue imprimiendo hasta llegar al espacio
                 
                //printf("done3");
            }
            output[pos_output++]=' '; //insertar el espacio para poder marcar por terminado
            
            }
        else if (current==' '){
            //armar el caso de space, no hace nada
        }
        else{
            //si no encontro algo que tiene sentido
            printError("Error in format conversion, invalid input");
            return 1;
        }
        
            
    }
    while (stack_pos!=0) //popear el resto del stack
    {
       char current=stack[--stack_pos];
       if (current!='('&&current!=')')
       {
        output[pos_output++]=current;
        output[pos_output++]=' ';
       }
        
    }
    output[pos_output++]=' ';
    output[pos_output-1]=0;
    return 0;
}
