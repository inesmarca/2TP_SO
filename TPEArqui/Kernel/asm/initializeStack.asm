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

    push 0x0                                ;SS
    push rdi        ; puntero al rbp        ;RSP
    push 0x202                              ;FLAGS
    push 0x8                                ;CS
    push rsi        ; puntero al wrapper    ;RIP
    push 2                                  ;RBX
    push r9                                 ;RCX
    push r8                                 ;RDX    
    push 5                                  ;RBP
    push rdx        ; puntero al argc       ;RDI
    push rcx        ; puntero al argv[]     ;RSI
    push 6                                  ;R8
    push 7                                  ;R9
    push 8                                  ;R10
    push 9                                  ;R11
    push 10                                 ;R12
    push 11                                 ;R13
    push 12                                 ;R14
    push 13                                 ;R15

    mov rax, rsp    ; retorno la posicion del rsp luego de hacer los push
    mov rsp, rbx    ; vuelvo al rsp anterior
    ret

