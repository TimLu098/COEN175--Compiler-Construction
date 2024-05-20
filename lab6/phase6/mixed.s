main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	movl	$100, %eax
	movl	%eax, -16(%ebp)
	movl	$30, %eax
	movl	%eax, -20(%ebp)
	fldl	.L0
	fstpl	-44(%ebp)
#Generating Cast 
	#Generating add 	
#Generating Cast 
	#Generating add 	
	movl	-16(%ebp), %eax
	addl	-20(%ebp), %eax
	movl	%eax, -48(%ebp)
	fildl	-48(%ebp)
	fldl	-44(%ebp)
	faddp
	subl	$4, %esp
	fstcw	(%esp)
	fstcw	2(%esp)
	orw	$0x0c00, 2(%esp)
	fldcw	2(%esp)
	fistpl	-52(%ebp)
	fldcw	(%esp)
	movl	-52(%ebp), %eax
	movl	%eax, -4(%ebp)
#Generating Cast 
	#Generating subtract 	
#Generating Cast 
	#Generating subtract 	
	movl	-16(%ebp), %eax
	subl	-20(%ebp), %eax
	movl	%eax, -56(%ebp)
	fildl	-56(%ebp)
	fldl	-44(%ebp)
	fsubrp	
	subl	$4, %esp
	fstcw	(%esp)
	fstcw	2(%esp)
	orw	$0x0c00, 2(%esp)
	fldcw	2(%esp)
	fistpl	-60(%ebp)
	fldcw	(%esp)
	movl	-60(%ebp), %eax
	movl	%eax, -8(%ebp)
#Generating Cast 
	#Generating multiply 	
#Generating Cast 
	#Generating multiply 	
	movl	-16(%ebp), %eax
	imul	-20(%ebp), %eax
	movl	%eax, -64(%ebp)
	fildl	-64(%ebp)
	fldl	-44(%ebp)
	fmulp	
	subl	$4, %esp
	fstcw	(%esp)
	fstcw	2(%esp)
	orw	$0x0c00, 2(%esp)
	fldcw	2(%esp)
	fistpl	-68(%ebp)
	fldcw	(%esp)
	movl	-68(%ebp), %eax
	movl	%eax, -12(%ebp)
	#Generating add 	
#Generating Cast 
	#Generating divide 	
	movl	-16(%ebp), %eax
	movl	-20(%ebp), %ecx
	cltd	
	idivl	%ecx
	movl	%eax, -72(%ebp)
	fildl	-72(%ebp)
	fldl	-44(%ebp)
	faddp
	fstpl	-28(%ebp)
	#Generating subtract 	
#Generating Cast 
	#Generating remainder 	
	movl	-16(%ebp), %eax
	movl	-20(%ebp), %ecx
	cltd	
	idivl	%ecx
	movl	%edx, -76(%ebp)
	fildl	-76(%ebp)
	fldl	-44(%ebp)
	fsubrp	
	fstpl	-36(%ebp)
	pushl	-4(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-8(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	pushl	-12(%ebp)
#Generating Address
	leal	.L1, %eax
	pushl	%eax
	call	printf
	addl	$8, %esp
	fldl	-28(%ebp)
	subl	$8, %esp
	fstpl	(%esp)
#Generating Address
	leal	.L4, %eax
	pushl	%eax
	call	printf
	addl	$12, %esp
	fldl	-36(%ebp)
	subl	$8, %esp
	fstpl	(%esp)
#Generating Address
	leal	.L4, %eax
	pushl	%eax
	call	printf
	addl	$12, %esp
	#Generating greater than 	
	movl	-16(%ebp), %eax
	cmpl	-20(%ebp), %eax
	setg	%al
	movzbl	%al, %eax
	movl	%eax, -4(%ebp)
	#Generating equal 	
#Generating Cast 
	fildl	-20(%ebp)
	fldl	-44(%ebp)
	fcompp	
	fstsw	%ax
	sahf	
	sete	%al
	movzbl	%al, %eax
	movl	%eax, -8(%ebp)
	#Generating equal 	
	#Generating less than 	
	movl	-16(%ebp), %eax
	cmp	-20(%ebp), %eax
	setl	%al
	movzbl	%al, %eax
	#Generating less than 	
#Generating Cast 
	fildl	-20(%ebp)
	fldl	-44(%ebp)
	movl	%eax, -80(%ebp)
