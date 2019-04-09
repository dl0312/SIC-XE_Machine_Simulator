	.file	"test.c"			; compile 한 파일의 이름입니다.
	.comm	a,1000,32			; ~ char a[10][10][10] ~ 1000(10*10*10*1(byte))
	.comm	aa,16,16			; ~ struct AA { ... } aa ~ word alignment에 의해 char 타입이 4byte를 차지하여 전체 16(4+4+4+4)byte가 할당됩니다.
	.comm	dl,4,4				; ~ enum days { ... } dl ~ enum은 결과적으로 int 타입이기 때문에 4byte가 할당됩니다.
	.comm	bb,8,8				; ~ union BB { ... } bb ~ union은 속하는 타입의 최대 크기가 union의 크기입니다. 따라서 가장 큰 long형의 크기인 8byte가 할당됩니다.
	.comm	cc,8,8				; ~ int *cc ~ 주소값은 8byte를 할당합니다. 
	.comm	k,4,4				; ~ int k ~ int 타입은 4byte를 할당합니다.
	.comm	i,4,4				; ~ float i ~ float 타입은 4byte를 할당합니다.
	.comm	j,4,4				; ~ float j ~ float 타입은 4byte를 할당합니다.
	.text
	.globl	main
	.type	main, @function
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
	pxor	%xmm0, %xmm0		; .
	cvtsi2ss	%eax, %xmm0		; int 타입 데이터를 float 타입에 넣어야하기 때문에 type implicit conversion을 합니다.
	movss	%xmm0, aa+12(%rip)	; struct aa에서 char, int, char 각 1,4,1를 차지하지만 word alignment에 의해 char 타입이 각각 word size인 4byte를 차지하여 aad에서 12를 더한 주소에 값을 저장합니다.
	movl	k(%rip), %eax		; ~ bb.b = k ~ k에 해당하는 값을 eax에 넣습니다.
	movl	%eax, bb(%rip)		; bb는 union으로 이 상황에서는 int 타입으로 생각해서 eax에 있는 값을 그냥 넣어줍니다.
	movl	k(%rip), %eax		; ~ j = i + k ~ 우선 k 에 해당하는 값을 eax에 넣습니다.
	pxor	%xmm0, %xmm0		; .
	cvtsi2ss	%eax, %xmm0		; int 타입을 float 타입으로 type implicit conversion합니다.
	movss	i(%rip), %xmm1		; .
	addss	%xmm1, %xmm0		; 구해진 두 i와 float 타입으로 바뀐 k를 더합니다.
	movss	%xmm0, j(%rip)		; 더한 결과를 j에 넣습니다.
	movl	k(%rip), %eax		; ~ j = i + (float)k ~ 더하기 전에 type explicit conversion을 한 경우입니다.
	pxor	%xmm0, %xmm0		; type explicit conversion을 했지만, assemble code는 같습니다.
	cvtsi2ss	%eax, %xmm0		; .
	movss	i(%rip), %xmm1		; .
	addss	%xmm1, %xmm0		; .
	movss	%xmm0, j(%rip)		; .
	movq	$bb, cc(%rip)		; ~ cc = &bb.b ~ bb.b에 해당하는 주소 값을 cc에 넣습니다.
	nop
	popq	%rbp
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
