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

Al iniciar el programa se le pedirá ingresar el nombre de usuario, este es permanente mientras el sistema permanezca encendido.
al ingresar recibirá un mensaje de bienvenida e instrucciones para poder continuar operando con nuestro sistema.

Se dispone de dos ventanas, la calculadora en la pantalla superior y la shell en la pantalla inferior. La pantalla activa se denotará por un cursor blanco. Se comienza en la shell donde será solicitado su nombre el cual no podrá ser cambiado una vez ingresado. Para cambiar entre pantallas solo debe presionar la tecla TAB y observará que el cursor se ha movido a la pantalla activa. Para finalizar el programa solo debe presionar la tecla ESC.

Dentro de la pantalla de la calculadora se pueden hacer las operaciones +, -, /, * siempre finalizando la operación con un = y utilizando espacios para separar cada elemento de su expresión. También se pueden usar paréntesis. Para borrar la línea entera presione Shift d.
Ej:  ( 2 + 3 ) / 5 =

Dentro de la pantalla 2 se puede correr las siguientes funciones ingresando los debidos parámetros requeridos separados por un espacio:
help: enumeración de las funciones disponibles del sistema
printTime: imprime en pantalla la hora actual
printTemperature: imprime en pantalla la temperatura de la computadora
printCPUInfo: imprime en pantalla la información sobre el CPU
inforeg: imprime en pantalla los registros guardados al apretar CTRL S
triggerException0: demostración de la excepción división por 0
triggerException6: demostración de la excepción invalid opcode
printmem: recibe una dirección de memoria y hace un volcado de memoria de 32 bytes a partir de dicho parámetro
clear: limpia la pantalla
