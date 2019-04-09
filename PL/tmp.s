	.file	"test.c"			;		
	.comm	a,1000,32			;
	.comm	aa,16,16			;
	.comm	dl,4,4				;
	.comm	bb,8,8				;
	.comm	cc,8,8				;
	.comm	k,4,4				;
	.comm	i,4,4				;
	.comm	j,4,4				;
	.text						;
	.globl	main				;
	.type	main, @function		;
main:							
	pushq	%rbp				; ~ void main(void) ~ 반환값과 인자에 대한 공간을 스택에 할당해줍니다.
	movq	%rsp, %rbp			; 하지만, 둘 다 void 타입이라 그냥 지나갑니다.
	movzbl	a+300(%rip), %eax	; ~ k = a[3][0][0] ~  a 주소에 300(3*10*10*1(byte))을 더한 주소의 값을 eax에 넣습니다.
	movsbl	%al, %eax			; movsbl을 이용해 char 타입을 int 타입으로 type implicit conversion합니다.
	movl	%eax, k(%rip)		; 결과값을 k에 넣습니다.
	movzbl	a+215(%rip), %eax	; ~ aa.d = a[2][1][5] + a[1][2][3] ~ a[2][1][5]를 구하기 위해 위와 같이 215를 더한 주소의 값을 eax에 넣습니다.
	movsbl	%al, %edx			; movsbl을 이용해 char 타입을 int 타입으로 type implicit conversion합니다.
	movzbl	a+123(%rip), %eax	; a[1][2][3]을 구하기 위해 위와 같이 123을 더한 주소의 값을 eax에 넣습니다.
	movsbl	%al, %eax			; movsbl을 이용해 char 타입을 int 타입으로 type implicit conversion합니다.
	addl	%edx, %eax			; a[2][1][5]와 a[1][2][3]에 해당하는 값을 더합니다.
	pxor	%xmm0, %xmm0		; int 타입 데이터를 float 타입에 넣어야하기 때문에 type implicit conversion을 합니다.
	cvtsi2ss	%eax, %xmm0		;
	movss	%xmm0, aa+12(%rip)	; struct aa에서 char, int, char 각 1,4,1를 차지하지만 word alignment에 의해 char 타입이 각각 word size인 4byte를 차지하여 aad에서 12를 더한 주소에 값을 저장합니다.
	movzbl	aa+8(%rip), %eax	;
	movsbl	%al, %eax			;
	movl	%eax, dl(%rip)		;
	movl	k(%rip), %eax		;
	movl	%eax, bb(%rip)		;
	movl	k(%rip), %eax		;
	pxor	%xmm0, %xmm0		;
	cvtsi2ss	%eax, %xmm0		;
	movss	i(%rip), %xmm1		;
	addss	%xmm1, %xmm0		;
	movss	%xmm0, j(%rip)		;
	movq	$bb, cc(%rip)		;
	nop							; finish main function
	popq	%rbp				;
	ret							;
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
