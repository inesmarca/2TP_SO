GLOBAL getTempTargetInfo
GLOBAL getTempOffsetInfo

getTempTargetInfo:
    push rbp
    mov rbp,rsp

    mov ecx,1A2h;   setear la direc de memoria para temperature_status
    ;rdmsr; me tira la info de la temp en edx:eax 
    mov eax,0x5640000

    mov rsp, rbp
    pop rbp
    ret 

getTempOffsetInfo:

    push rbp
    mov rbp,rsp

    mov ecx,19Ch;   setear la direc de memoria para temperature_status
    ;rdmsr; me tira la info de la temp en edx:eax 
    mov eax,0x884C2808

    mov rsp, rbp
    pop rbp
    ret 