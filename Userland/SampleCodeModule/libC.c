// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <libC.h>
#include <stdarg.h>
#include <math.h>
#include <sysLib.h>

#define MAX_DIGITOS_EN_UN_NUMERO 20
#define DECIMALPLACES 4
#define MAX_PRINTABLE_CHARACTERS 1024
#define MAX_READABLE_CHARACTERS 1024
#define DELETE 0x0E

long stringtoLong_libc (char * string);
int longToString_libc(long value, char * buffer);

// https://www.geeksforgeeks.org/write-your-own-atoi/
int atoi(char* str) { 
    int res = 0; 
    int sign = 1; 
    int i = 0; 
    if (str[0] == '-') { 
        sign = -1; 
        i++; 
    } 
    for (; str[i] != '\0'; ++i) 
        res = res * 10 + str[i] - '0'; 
    return sign * res; 
} 

void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j) {
	while (i < j)
		swap(&buffer[i++], &buffer[j--]);

	return buffer;
}

// https://www.techiedelight.com/implement-itoa-function-in-c/
char* itoa(int value, char* buffer, int base) {
	if (base < 2 || base > 32)
		return buffer;

	int n = value;
	if (value < 0) 
		n *= -1;

	int i = 0;
	while (n) {
		int r = n % base;

		if (r >= 10) 
			buffer[i++] = 65 + (r - 10);
		else
			buffer[i++] = 48 + r;

		n = n / base;
	}

	if (i == 0)
		buffer[i++] = '0';

	if (value < 0 && base == 10)
		buffer[i++] = '-';

	buffer[i] = '\0';

	return reverse(buffer, 0, i - 1);
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

void * memset(void *s, int c, int n) {
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
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

int putChar(char str) {
	char buff[2] = {0};
	buff[0] = str;
	return write(STDOUT, buff, 1);
}

char getChar() {
	char res;
	if (read(STDIN, &res, 1) == -1)
		return -1;
	return res;
}

void printf(const char * format,...) {
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
				itoa(va_arg( valist, int ), aux, 10);
				/*
				int counter=intToString(va_arg( valist, int ),aux);//me transforma el int a un string
				if (counter>MAX_DIGITOS_EN_UN_NUMERO)
				{
					// should throw exception //
					//no acepta int's con mas de MAX_DIGITOS_EN_UN_NUMERO digitos.
				} */
				for (int j = 0; aux[j] != 0; j++)
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
			
	
    		
    		changeColor(0xFF0000, DEFAULT_BACKGROUND_COLOR);
			printf("Expresion %s is unsupported \n", format[i]);
    		changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
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
	write(STDOUT, output, output_pos);
}

int scanf(const char *format, ...) {
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
				//putChar(in);
			}
		} else {
			input[input_pos++] = in;
			//putChar(in);
		}
		in = getChar();
	}
	//putChar('\n');
	
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
    		changeColor(0xFF0000, DEFAULT_BACKGROUND_COLOR);
			printf("Expresion %s is unsupported \n",format[i]);
    		changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
			//expression not suported
			va_end(valist);
			return -1;
				break;
			}
		}
	}
	va_end(valist);
    return number_of_vars;
}

int sscanf(const char *str,const char *format, ...) {
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
    		changeColor(0xFF0000, DEFAULT_BACKGROUND_COLOR);
			printf("Expresion %s is unsupported \n",format[i]);
    		changeColor(DEFAULT_LETTER_COLOR, DEFAULT_BACKGROUND_COLOR);
			//expression not suported
			va_end(valist);
			return -1;
				break;
			}
		}
	}
	va_end(valist);
    return number_of_vars;
}
/*
int intToString(int value, char * buffer) {
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
*/
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
int longToString_libc(long value, char * buffer) {
	char *p = buffer;
	char *p1, *p2;
	int digits = 0;

	//Calculate characters for each digit
	do
	{
		int remainder = value % 10;
		*p++ =  remainder + '0';
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

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base) {
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

void * memcpy(void * destination, const void * source, uint64_t length) {
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}