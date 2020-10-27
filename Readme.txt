x64BareBones is a basic setup to develop operating systems for the Intel 64 bits architecture.

The final goal of the project is to provide an entry point for a kernel and the possibility to load extra binary modules separated from the main kernel.

Environment setup:
1- Install the following packages before building the Toolchain and Kernel:

nasm qemu gcc make

2- Build the Toolchain

Execute the following commands on the x64BareBones project directory:

  user@linux:$ cd Toolchain
  user@linux:$ make all

3- Build the Kernel

From the x64BareBones project directory run:

  user@linux:$ make all

4- Run the kernel

From the x64BareBones project directory run:

  user@linux:$ ./run.sh


Author: Rodrigo Rearden (RowDaBoat)
Collaborator: Augusto Nizzo McIntosh

Manual de Usuario:Pure Infernal 64bit

Quickstart Guide:

Si desea utilizar el memory manager buddy debe compilar de la siguiente manera:
  
  make MM=BUDDY

Una vez inicializado la shell le saltara un prompt preguntándole su nombre. 
Puede utilizar el comando help el cual le mostrara todas las funciones disponibles que puede ejecutar.

- Background para correr un proceso en background llame a dicho proceso seguido por un ''\&'', un ejemplo de uso sería ''loop \&''. 
Tenga en cuenta que no pueden correr dos procesos en foreground durante el mismo tiempo de vida de uno.

- Pipes Para utilizar pipes debe posicionar sus procesos entre un pipe ''|''. Un ejemplo de uso sería ''ps | cat''.

- EOF: para mandar un EOF a través del teclado se debe utilizar las teclas Control D. 
Esto puede ser utilizado para marcar la finalización de mensaje al correr la función wc. 

- Philosophers: al correr la función de philosophers puede ingresar ''a'' para agregar un filósofo, ''r'' para remover a un filósofo y ''c'' para finalizar la ejecución. 
Es recomendado no bajar de 3 filósofos porque puede haber comportamiento inesperado.

- Kill: la syscall kill recibe como parámetro el pid y el estado al cual se lo quiere cambiar, siendo 0 estado de KILL, 1 estado de BLOCKED y 2 estado de UNBLOCK. 
Tener en cuenta que en el shell existe una función distinta para cada estado.

- Tests de la Cátedra: para correr los tests provisto por la cátedra primero se debe ingresar el comando ''test'' el cual le mostrara en pantalla los ids correspondientes para correr el test que quiera. Dichos tests se van a correr en background.

- Test de Semáforos: para los tests de semáforos se debió matar a la shell para evitar que dicho test tenga una larga duración y para poder mostrar la correcta funcionalidad de los semáforos. 
Por ende una vez finalizado el test deberá volver a iniciar el programa para continuar utilizándolo.

- Memory Manager: para compilar utilizando el memory manager buddy debe compilar haciendo ''make MM=BUDDY''.
