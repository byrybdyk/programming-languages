%include "lib.inc"
global find_word
%define WORD_SIZE 8
section .text

find_word:
	push r12
	push r13
	mov r12, rsi ; dictionary entry
	mov r13, rdi ; line
	
.loop:
    mov rsi, r12
	mov rdi, r13
	add rsi, WORD_SIZE
	call string_equals
	test rax, rax
	jnz .found
	mov r12, [r12]
	test r12, r12
	jz .not_found
	jmp .loop
.found:
	add r12, WORD_SIZE
	mov rax, r12
	jmp .end
.not_found:
	xor rax, rax
.end:
	pop r12
	pop r13
	ret
