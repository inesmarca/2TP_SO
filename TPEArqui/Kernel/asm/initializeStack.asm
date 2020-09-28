GLOBAL initializeStack

section .text


;	"R15:   ", "R14:   ", "R13:   ", "R12:   ", "R11:   ", 
;	"R10:   ", "R9:    ", "R8:    ", "RSI:   ", "RDI:   ", 
;	"RBP:   ", "RDX:   ", "RCX:   ", "RBX:   ", "RAX:   ", 
;	"RIP:   ", "CS:    ", "FLAGS: ", "RSP:   ", "SS"


; uint64_t * initializeStack(uint64_t * rsp, void * wrapper, void * func, int argc, char * argv[], int pid)
initializeStack:
    mov rbx, rsp    ; guardo el rsp actual

    mov rsp, rdi    ; muevo el rsp al rsp del nuevo proceso

    push 0x0
    push rdi        ; puntero al rbp
    push 0x202
    push 0x8
    push rsi        ; puntero al wrapper
    push 1
    push 2
    push r8         ; puntero a argv[]
    push 3
    push 4
    push rdx        ; puntero al main
    push rcx        ; puntero al argc
    push r9         ; puntreo al pid del proceso
    push 6
    push 7
    push 8
    push 9
    push 10
    push 11
    push 12

    mov rax, rsp    ; retorno la posicion del rsp luego de hacer los push
    mov rsp, rbx    ; vuelvo al rsp anterior
    ret
