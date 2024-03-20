%include "lib.inc"
%include "dict.inc"
%include "words.inc"

%define MAX_SIZE 255
%define STDERR 2
%define WRITE_SYSCALL 1
global _start
section .rodata
incorrect_input_message: db "Incorrect input", 0
error_message : db "No such element", 0
section .text
_start:
	sub rsp, MAX_SIZE+1
	mov rdi, rsp
	mov rsi, MAX_SIZE
	call read_word
	test rax, rax
	je .print_word_too_long
	mov rdi, rsp
	mov rsi, one
	call find_word
	test rax, rax
	je .print_no_such_element
	mov rdi, rax
	push rdi
	call string_length
	pop rdi
    inc rdi
	add rdi, rax
	call print_string
	xor rdi, rdi
	jmp .end
.print_word_too_long:
	mov rdi, incorrect_input_message
	jmp .print_error
.print_no_such_element:
	mov rdi, error_message
.print_error:
	push rdi
	call string_length
	pop  rsi
	mov  rdx, rax
    mov  rax, WRITE_SYSCALL
    mov  rdi, STDERR
    syscall
.end:
	add rsp, MAX_SIZE+1
	call exit
