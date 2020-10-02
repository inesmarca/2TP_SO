GLOBAL getKey

getKey:
    push rbp
    mov rbp, rsp

    mov rax, 0
    mov al, 1
    out 64h, al
    in al, 60h

    mov rsp, rbp
    pop rbp
    ret