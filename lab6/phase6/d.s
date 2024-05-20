main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	fldl	.L1
	fstpl	-8(%ebp)
	fldl	.L3
	fstpl	-16(%ebp)
	fldl	.L5
	fstpl	-24(%ebp)
	#Generating add 	
	#Generating add 	
	fldl	-8(%ebp)
	fldl	-16(%ebp)
	faddp
	fldl	-24(%ebp)
	faddp
	fstpl	-32(%ebp)
	#Generating subtract 	
	#Generating subtract 	
	fldl	-8(%ebp)
	fldl	-16(%ebp)
	fsubrp	
	fldl	-24(%ebp)
	fsubrp	
	fstpl	-40(%ebp)
	#Generating multiply 	
	#Generating multiply 	
	fldl	-8(%ebp)
	fldl	-16(%ebp)
	fmulp	
	fldl	-24(%ebp)
	fmulp	
	fstpl	-48(%ebp)
	#Generating add 	
	#Generating divide 	
	fldl	-8(%ebp)
	fldl	-16(%ebp)
	fdivrp	
	fldl	-24(%ebp)
	faddp
	fstpl	-56(%ebp)
	#Generating subtract 	
	#Generating divide 	
	fldl	-8(%ebp)
	fldl	-16(%ebp)
	fdivrp	
	fldl	-24(%ebp)
	fsubrp	
	fstpl	-64(%ebp)
	fldl	-32(%ebp)
	subl	$8, %esp
	fstpl	(%esp)
#Generating Address
	leal .L7, %eax
	pushl	%eax
	call	printf
	addl	$12, %esp
	fldl	-40(%ebp)
	subl	$8, %esp
	fstpl	(%esp)
#Generating Address
	leal .L7, %eax
	pushl	%eax
	call	printf
	addl	$12, %esp
	fldl	-48(%ebp)
	subl	$8, %esp
	fstpl	(%esp)
#Generating Address
	leal .L7, %eax
	pushl	%eax
	call	printf
	addl	$12, %esp
	fldl	-56(%ebp)
	subl	$8, %esp
	fstpl	(%esp)
#Generating Address
	leal .L7, %eax
	pushl	%eax
	call	printf
	addl	$12, %esp
	fldl	-64(%ebp)
	subl	$8, %esp
	fstpl	(%esp)
#Generating Address
	leal .L7, %eax
	pushl	%eax
	call	printf
	addl	$12, %esp

main.exit:
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 64
	.globl	main

	.data
.L7:	.asciz	"%f\012"
.L1:	.double	100.000
.L5:	.double	2.00000
.L3:	.double	30.0000
