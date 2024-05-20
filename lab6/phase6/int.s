main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$100, %eax
	movl	%eax, -4(%ebp)
	movl	$30, %eax
	movl	%eax, -8(%ebp)
	movl	$2, %eax
	movl	%eax, -12(%ebp)
	#Generating add 	
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	-8(%ebp), %eax
	addl	-12(%ebp), %eax
	movl	%eax, -16(%ebp)
	#Generating subtract 	
	#Generating subtract 	
	movl	-4(%ebp), %eax
	subl	-8(%ebp), %eax
	subl	-12(%ebp), %eax
	movl	%eax, -20(%ebp)
	#Generating multiply 	
	#Generating multiply 	
	movl	-4(%ebp), %eax
	imul	-8(%ebp), %eax
	imul	-12(%ebp), %eax
	movl	%eax, -24(%ebp)
	#Generating add 	
	#Generating divide 	
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %ecx
	cltd	
	idivl	%ecx
	addl	-12(%ebp), %eax
	movl	%eax, -28(%ebp)
	#Generating subtract 	
	#Generating remainder 	
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %ecx
	cltd	
	idivl	%ecx
	movl	%edx, %eax
	subl	-12(%ebp), %eax
	movl	%eax, -32(%ebp)
	pushl	-16(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-20(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-24(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-28(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-32(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	#Generating greater than 	
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -16(%ebp)
	#Generating equal 	
	#Generating less than 	
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	#Generating less than 	
	movl	-12(%ebp), %ecx
	cmpl	-8(%ebp), %ecx
	setl	%cl
	movzbl	%cl, %ecx
	cmpl	%ecx, %eax
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -20(%ebp)
	#Generating not equal 	
	#Generating greater or equal 	
	movl	-4(%ebp), %eax
	cmpl	-8(%ebp), %eax
	setge	%al
	movzbl	%al, %eax
	#Generating greater or equal 	
	movl	-8(%ebp), %ecx
	cmpl	-4(%ebp), %ecx
	setge	%cl
	movzbl	%cl, %ecx
	cmpl	%ecx, %eax
	setne	%al
	movzbl	%al, %eax
	movl	%eax, -24(%ebp)
	#Generating greater than 	
	#Generating add 	
	movl	-4(%ebp), %eax
	addl	-8(%ebp), %eax
	cmpl	-12(%ebp), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -28(%ebp)
	#Generating less than 	
	#Generating subtract 	
	movl	-4(%ebp), %eax
	subl	-8(%ebp), %eax
	#Generating multiply 	
	movl	%eax, -36(%ebp)
	movl	-8(%ebp), %eax
	imul	-12(%ebp), %eax
	movl	-36(%ebp), %ecx
	cmpl	%eax, %ecx
	setl	%al
	movzbl	%al, %eax
	movl	%eax, -32(%ebp)
	pushl	-16(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-20(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-24(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-28(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-32(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 36
	.globl	main

	.data
.L1:	.asciz	"%d\012"
