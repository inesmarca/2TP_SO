GLOBAL read
GLOBAL write
GLOBAL readPixel
GLOBAL writePixel
GLOBAL getExitFlag
GLOBAL getTemperature
GLOBAL getRegisters
GLOBAL clear
GLOBAL changeColor
GLOBAL setCursor
GLOBAL getTime
GLOBAL malloc
GLOBAL free
GLOBAL create
GLOBAL kill
GLOBAL getpid
GLOBAL nice
GLOBAL yield
GLOBAL sem_open
GLOBAL sem_wait
GLOBAL sem_post
GLOBAL sem_close
GLOBAL pipe
GLOBAL close
GLOBAL memState
GLOBAL getListPCB
GLOBAL getListSem
GLOBAL getPipeList

GLOBAL getListPids
GLOBAL getInfoPCB

section .text

; void read(int fd, char * buf, int cant)
read:
    mov rax, 0
    int 80h
    ret

; void write(int fd, const char * string, int cant)
write:
    mov rax, 1
    int 80h
    ret

; int readPixel(int * hexa, int x, int y)
readPixel:
    mov rax, 2
    int 80h
    ret

; void writePixel(int x, int y, int color)
writePixel:
    mov rax, 3
    int 80h
    ret

; void clear()
clear:
    mov rax, 4
    int 80h
    ret

; void changeScreen(int screen)
changeColor:
    mov rax, 5
    int 80h
    ret

;int getTemperature()
getTemperature:
    mov rax,6
    int 80h
    ret
	
; void getRegisters(uint64_t * buff) 
getRegisters:
    mov rax, 7
    int 80h
    ret

; void setCursor(int x, int y);
setCursor:
    mov rax, 8
    int 80h
    ret

; void getTime(int * buff);
getTime:
    mov rax, 9
    int 80h
    ret

malloc:
    mov rax, 10
    int 80h
    ret

free:
    mov rax, 11
    int 80h
    ret

kill:
    mov rax, 12
    int 80h
    ret

create:
    mov rax, 13
    int 80h
    ret

getpid:
    mov rax, 14
    int 80h
    ret

nice:
    mov rax, 15
    int 80h
    ret

yield:
    mov rax, 16
    int 80h
    ret

sem_open:
    mov rax, 17
    int 80h
    ret 

sem_wait:
    mov rax, 18
    int 80h
    ret

sem_post:
    mov rax, 19
    int 80h
    ret

sem_close:
    mov rax, 20
    int 80h
    ret

pipe:
    mov rax, 21
    int 80h
    ret

close:
    mov rax, 22
    int 80h
    ret

memState:
    mov rax, 23
    int 80h
    ret

getListPids:
    mov rax, 24
    int 80h
    ret

getInfoPCB:
    mov rax, 25
    int 80h
    ret

getListSem:
    mov rax, 26
    int 80h
    ret

getPipeList:
    mov rax, 27
    int 80h
    ret

