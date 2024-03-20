; lib.asm
%define SYSCALL_WRITE 1
%define SYSCALL_EXIT 60
%define STDOUT 1
%define NEXT_LINE_SYMBOL 0xA
%define TAB_SYMBOL 0x9
%define SPACE_SYMBOL 0x20
%define NINE_SYMBOL '9'
%DEFINE ZERO_SYMBOL '0'
%define MINUS '-'
%define PLUS '+'

%macro check_symbols 1
	cmp %1, SPACE_SYMBOL
	je .skip
	cmp %1, TAB_SYMBOL
	je .skip
	cmp %1, NEXT_LINE_SYMBOL
	je .skip
%endmacro

global exit
global string_length
global print_string
global print_newline
global print_char
global print_int
global print_uint
global string_equals
global read_char
global read_word
global parse_uint
global parse_int
global string_copy

section .text
 
; Принимает код возврата и завершает текущий процесс
exit:
	mov  rax, SYSCALL_EXIT            ; invoke 'exit' system call
    syscall


; Принимает указатель на нуль-терминированную строку, возвращает её длину
string_length:
	xor rax, rax
    .loop:
        cmp byte [rdi+rax], 0
        je .ret ; Jump if equal/Jump if zero
        inc rax
        jmp .loop
    
    .ret:
        ret


; Принимает указатель на нуль-терминированную строку, выводит её в stdout
print_string:
	push rdi
    call string_length
    pop  rsi
    mov  rdx, rax
    mov  rax, SYSCALL_WRITE 
    mov  rdi, STDOUT
    syscall
    ret


; Переводит строку (выводит символ с кодом 0xA)
print_newline:
    mov     rdi, NEXT_LINE_SYMBOL       ;'\n' symbol

	
; Принимает код символа и выводит его в stdout
print_char:
	push rdi
    mov  rax, SYSCALL_WRITE 
	mov rsi, rsp
	mov rdi, STDOUT
	mov rdx, 1
	syscall
	pop rdi
	ret
	

; Выводит знаковое 8-байтовое число в десятичном формате 
 print_int:
	cmp rdi, 0
	jge print_uint
	neg rdi
	push rdi
	mov rdi, MINUS
	call print_char
	pop rdi
	

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
 print_uint:
	mov rax, rdi
	mov rdi, rsp
	sub rsp, 32
	mov r10, 10
	dec rdi
	.loop:
    	xor rdx, rdx
		dec rdi
    	div r10
    	add dl, ZERO_SYMBOL
    	mov byte[rdi], dl
    	test rax, rax
    	jne .loop
	call print_string
	add rsp, 32
	ret



; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
.loop:
    mov dl, byte[rdi]
    cmp dl, byte[rsi]
    jne .neq
    inc rdi
    inc rsi
    test dl, dl
    je .eq
    jmp .loop
.neq:
    xor rax, rax
    ret
.eq:
    mov rax, 1
    ret


; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    push 0 
    xor rax, rax
    xor rdi, rdi 		
    mov rsi, rsp
    mov rdx, 1
    syscall  
    pop rax
    ret

	
; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор
read_word:
	push rbx
	push r12
	push r13
        mov r12, rdi
        mov r13, rsi
		xor rbx, rbx      
	.loop:
        call read_char
        check_symbols (rax)
        test rax, rax
        je .end
        cmp rbx, r13
        jge .buffoverflow
        mov byte[r12 + rbx], al
        inc rbx
        jmp .loop
.buffoverflow:
	xor rax, rax
	pop r13
	pop r12
	pop rbx
        ret
.skip:
        test rbx, rbx
        je .loop
.end:
        mov byte[r12 + rbx], 0
        mov rax, r12
	mov rdx, rbx
	pop r13
	pop r12
	pop rbx
	ret


; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
	xor rax, rax	
	xor rcx, rcx
	mov r10, 10
.loop:	
	mov r9b, [rdi + rcx]
	cmp r9, NINE_SYMBOL
	jg .not_num
	cmp r9, ZERO_SYMBOL
	jl .not_num
	sub r9, ZERO_SYMBOL
	mul r10
	add rax, r9
	inc rcx
	jmp .loop
.not_num:
	test rcx, rcx
	je .fail
	mov rdx, rcx
	ret
.fail:
	xor rax, rax
	xor rdx, rdx
	ret


; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
	cmp byte [rdi],  MINUS
    je .neg
	cmp byte [rdi],  PLUS
    je .plus
    jmp parse_uint

.neg:
    inc rdi
    call parse_uint
	neg rax
    inc rdx
    ret

.plus:
	inc rdi
	call parse_uint
	inc rdx
	ret






; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
	xor rax, rax
.loop:
	cmp rax, rdx
	jge .fail
	mov cl, byte[rdi + rax]
	mov byte[rsi + rax], cl
	inc rax
	test cl, cl
	je .end
	jmp .loop
.fail:	
	xor rax, rax
.end:
	ret 
