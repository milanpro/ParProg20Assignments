	.file	"experiment_a.c"
	.text
	.p2align 4,,15
	.globl	benchmark
	.type	benchmark, @function
benchmark:
.LFB0:
	.cfi_startproc
	.p2align 4,,10
	.p2align 3
.L3:
	movabsq	$100000000000, %rax
	.p2align 4,,10
	.p2align 3
.L2:
	subq	$1, %rax
	jne	.L2
	jmp	.L3
	.cfi_endproc
.LFE0:
	.size	benchmark, .-benchmark
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB1:
	.cfi_startproc
	xorl	%eax, %eax
	call	benchmark
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.ident	"GCC: (Debian 8.3.0-6) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
