#include <libC.h>
#include <stdarg.h>
#include <math.h>
#include <sysLib.h>

#define MAX_DIGITOS_EN_UN_NUMERO 20
#define DECIMALPLACES 4
#define MAX_PRINTABLE_CHARACTERS 1024
#define MAX_READABLE_CHARACTERS 1024
#define NULL (void*)0
#define DELETE 0x0E

long stringtoLong_libc (char * string);
int longToString_libc(long value, char * buffer);
int stringtoInt (char * string);
int intToString(int value, char * buffer);

// http://www.color-hex.com
// pagina para buscar los colores hexa correspondientes
int letter_color = DEFAULT_LETTER_COLOR;
int background_color = DEFAULT_BACKGROUND_COLOR;

void changeLetterColor(int code) {
	letter_color = code;
} 

void changeBackgroundColor(int code) {
	background_color = code;
}

//https://www.techiedelight.com/implement-strcpy-function-c/ era muy sencilla asi que la tomamos de internet.
char * strcpy(char * destination, const char * source){
	char * ptr=destination;
	while (*source != 0)
	{
		*destination=*source;
		destination++;
		source++;
	}
	*destination=0;
	return ptr;
	
}

int strcmp(char * s1, char * s2) {
    int cmp = 1;
    int i;
    for ( i = 0; s1[i] != 0 && s2[i] != 0 && cmp; i++) {
        if (s1[i] != s2[i]) {
            cmp = 0;
        }
    }

    if ((s1[i] == 0 && s2[i] != 0) || (s1[i] != 0 && s2[i] == 0) ) {
        cmp = 0;
    }
    return cmp;
}

void putChar(char str) {
	char buff[2] = {0};
	buff[0] = str;
	writeScreen(buff, letter_color, background_color);
}

char getChar() {
	char res = 0;
    while (res == 0) {
	    readKeyBuff(&res, 1);
    }
	return res;
}

void printf(const char * format,...){
	//tiene que tener el formato correcto para funcionar bien. si hay % faltantes o sobrantes no va afuncionar.
	//tiene soporte para %c %d %s
	//wont print more all if the resulting string is greater than 1024 characters
	char output [MAX_PRINTABLE_CHARACTERS];
  
	va_list valist;
    va_start( valist, format );
	int output_pos=0;
	for (int i = 0; format[i] != 0; i++)
	{
		if (format[i]=='%')
		{
			i++;
			switch (format[i])
			{
			case 'c':
				output[output_pos++]=(va_arg( valist, int ));
				break;
			
			

			case 'd':
                ;//algo que ver con c y switch que son raros
				char aux [MAX_DIGITOS_EN_UN_NUMERO];
				int counter=intToString(va_arg( valist, int ),aux);//me transforma el int a un string
				if (counter>MAX_DIGITOS_EN_UN_NUMERO)
				{
					/* should throw exception*/
					//no acepta int's con mas de MAX_DIGITOS_EN_UN_NUMERO digitos.
				}
				for (int j = 0; j < counter; j++)
				{
					output[output_pos++]=aux[j];//copio el in hecho string a mi output
				}
				break;
			
			case 'l':
			i++;//deshaecrme de la d
			char aux2 [MAX_DIGITOS_EN_UN_NUMERO];
			int counter2=longToString_libc(va_arg( valist, long ),aux2);//me transforma el int a un string
				if (counter2>MAX_DIGITOS_EN_UN_NUMERO)
				{
					/* should throw exception*/
					//no acepta int's con mas de MAX_DIGITOS_EN_UN_NUMERO digitos.
				}
				for (int j = 0; j < counter2; j++)
				{
					output[output_pos++]=aux2[j];//copio el in hecho string a mi output
				}
				break;
			

			
			case 's': //copio el string
            ;//algo que ver con c y switch que son raros
				char * string2=(char *)va_arg( valist, char* );
               
				int j=0;
            
                
                
				while (string2[j] != 0)//busco que termine el string a copiar
				{
                    
					output[output_pos++]=string2[j++];
                   
				}
                
				
				break;
			
			default:
			/* should throw exception*/
			
	
    		
			changeLetterColor(0xFF0000);
			printf("Expresion %s is unsupported \n",format[i]);
			changeLetterColor(DEFAULT_LETTER_COLOR);
			//expression not suported
				break;
			}
		}
        else //just copy the string
		{
			output[output_pos++]=format[i];
		}
		
		
	}
	va_end(valist);
    output[output_pos]=0;
	writeScreen(output, letter_color, background_color);
}

int scanf(const char *format, ...){
    //tiene que tener el formato correcto para funcionar bien. si hay % faltantes o sobrantes no va afuncionar.
	//tiene soporte para %c %d %s
	//lee como maximo MAX_READABLE_CHARACTERS (1024 characters)
	char input [MAX_READABLE_CHARACTERS];
	
	
	
	int input_pos=0;
	
	char in = getChar();
	
	while (in != '\n') {
		if (in == DELETE) {
			if (input_pos != 0) {
				input_pos--;
				input[input_pos] = 0;
				putChar(in);
			}
		} else {
			input[input_pos++] = in;
			putChar(in);
		}
		in = getChar();
	}
	putChar('\n');
	
	input[input_pos]=0;
	input_pos=0;
	
	va_list valist;
    va_start( valist, format );
    int number_of_vars=0;

	
	for (int i = 0; format[i] != 0; i++)
	{	
		
		if (format[i]=='%')
		{
			i++;
            number_of_vars++;
			switch (format[i])
			{
			case 'c':
			;//asignaciones en switch no pueden ser primera operacion o algo asi legacy c
			*(char *)va_arg( valist, char* ) = input[input_pos++];
				break;

			case 'l':
			i++;//deshacerse de la d
			;//algo que ver con c y switch que son raros
			char aux [MAX_DIGITOS_EN_UN_NUMERO];
			int aux_counter=0;
			while (input[input_pos]!=0 && input[input_pos]!=' ')
			{
			aux[aux_counter++]=input[input_pos++];
			}
			aux[aux_counter++]=0;

			*(long *)va_arg( valist, long* )=stringtoLong_libc(aux);
			
			break;
			
			case 'd':
			
                ;//algo que ver con c y switch que son raros
				char aux2 [MAX_DIGITOS_EN_UN_NUMERO];
				int aux_counter2=0;
				while (input[input_pos]!=0 && input[input_pos]!=' ')
				{
				aux2[aux_counter2++]=input[input_pos++];
				}
				aux2[aux_counter2++]=0;

				*(int *)va_arg( valist, int* )=stringtoInt(aux2);
				
				break;
			case 's': 
            ;//algo que ver con c y switch que son raros

			char * output=(char *)va_arg( valist, char* );
			int output_pos=0;
			while (input[input_pos]!=' '&&input[input_pos]!=0)
			{
				output[output_pos++]=input[input_pos++];
			}
			output[output_pos]=0;
			
				break;
			
			default:
			/* should throw exception*/
			changeLetterColor(0xFF0000);
			printf("Expresion %s is unsupported \n",format[i]);
			changeLetterColor(DEFAULT_LETTER_COLOR);
			//expression not suported
			return -1;
				break;
			}
		}
	}
	va_end(valist);
    return number_of_vars;
}

int sscanf(const char *str,const char *format, ...){
    //tiene que tener el formato correcto para funcionar bien. si hay % faltantes o sobrantes no va afuncionar.
	//tiene soporte para %c %d %s
	if (*str == 0) {
		return 0;
	}

	int string_pos=0;
	va_list valist;
    va_start( valist, format );
    int number_of_vars=0;

	
	for (int i = 0; format[i] != 0; i++)
	{	
		
		if (format[i]=='%')
		{
			i++;
            number_of_vars++;
			switch (format[i])
			{
			case 'c':
			;//asignaciones en switch no pueden ser primera operacion o algo asi legacy c
			*(char *)va_arg( valist, char* ) = str[string_pos++];
				break;
			
			case 'd':
			
                ;//algo que ver con c y switch que son raros
				char aux [MAX_DIGITOS_EN_UN_NUMERO];
				int aux_counter=0;
				while (str[string_pos]!=0 && str[string_pos]!=' ')
				{
				aux[aux_counter++]=str[string_pos++];
				}
				aux[aux_counter++]=0;

				*(int *)va_arg( valist, int* )=stringtoInt(aux);
				
				break;
			
			case 'l':
			i++;//me deshago de la d.
			;//algo que ver con c y switch que son raros
			char aux2 [MAX_DIGITOS_EN_UN_NUMERO];
			int aux_counter2=0;
			while (str[string_pos]!=0 && str[string_pos]!=' ')
			{
			aux2[aux_counter2++]=str[string_pos++];
			}
			aux2[aux_counter2++]=0;
			*(long *)va_arg( valist, long* )=stringtoLong_libc(aux2);
			
				break;

			case 's': 
            ;//algo que ver con c y switch que son raros

			char * output=(char *)va_arg( valist, char* );
			int output_pos=0;
			while (str[string_pos]!=' '&&str[string_pos]!=0)
			{
				output[output_pos++]=str[string_pos++];
			}
			output[output_pos]=0;
			
				break;
			
			default:
			/* should throw exception*/
			changeLetterColor(0xFF0000);
			printf("Expresion %s is unsupported \n",format[i]);
			changeLetterColor(DEFAULT_LETTER_COLOR);
			//expression not suported
			return -1;
				break;
			}
		}
	}
	va_end(valist);
    return number_of_vars;
}

int intToString(int value, char * buffer)
{
	char *p = buffer;
	char *p1, *p2;
	int digits = 0;

	//Calculate characters for each digit
	do
	{
		int remainder = value % 10;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= 10);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

int stringtoInt (char * string){
	//pasale strings bien, no valida. si los pasas mal, hay comportamiento inesperado
	int answer=0;
	int index=0;
	int negative=0;
	if (string[0]=='-')
	{
		negative=1;
		index++;
	}
	
	while (string[index]!=0)
	{
		if (!(string[index]>='0'&&string[index]<='9'))
		{	
			printError("invalid string for stringtoint conversion \n");
			return 0;
		}
		if (index>0)
		{
			answer*=10;
		}
		answer+=string[index++]-'0';
		
		
	}
	if (negative)
	{
		answer*=-1;
	}
	return answer;
}

long stringtoLong_libc (char * string){
	//pasale strings bien, no valida. si los pasas mal, hay comportamiento inesperado
	long answer=0;
	int index=0;
	int negative=0;
	if (string[0]=='-')
	{
		negative=1;
		index++;
	}
	
	while (string[index]!=0)
	{
		if (!(string[index]>='0'&&string[index]<='9'))
		{
			printError("invalid string for stringtoLong conversion \n");
			return 0;
		}
		answer*=10;
		answer+=string[index++]-'0';
		
		
	}
	if (negative)
	{
		answer*=-1;
	}
	return answer;
}
int longToString_libc(long value, char * buffer)
{
	char *p = buffer;
	char *p1, *p2;
	int digits = 0;

	//Calculate characters for each digit
	do
	{
		int remainder = value % 10;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= 10);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
	buffer[digits+1]=0;
	return digits;
}


	


uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

void baseToHexa(char * buff) {
	char auxStr[9] = "0x000000";
	int dim = 0;
	for (int i = 0; buff[i] != 0; i++) {
		dim++;
	}
	
	for (int j = 7; j >= 0 && dim != 0; j--) {
		auxStr[j] = buff[dim - 1];
		dim--;
	} 
	for (int k = 0; k < 8; k++) {
		buff[k] = auxStr[k];
	}
}
