	.file	"test.c"
	.comm	a,4000,32
	.comm	aa,12,8
	.comm	dl,4,4
	.comm	bb,4,4
	.comm	cc,8,8
	.comm	k,4,4
	.text
	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movl	a+1828(%rip), %eax
	movl	%eax, k(%rip)
	movl	$0, dl(%rip)
	movl	k(%rip), %eax
	movl	%eax, bb(%rip)
	movq	$bb, cc(%rip)
	nop
	popq	%rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
