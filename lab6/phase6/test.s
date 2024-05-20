main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
	pushl	$1
	pushl	$3
	call	mkdouble
	addl	$8, %esp
	fstpl	-20(%ebp)
	pushl	$1
	pushl	$2
	call	mkdouble
	addl	$8, %esp
	fstpl	-28(%ebp)
	movl	$2, %eax
	movl	%eax, -4(%ebp)
	movl	$3, %eax
	movl	%eax, -8(%ebp)
	#Generating less than 	
#Generating Cast 
	fildl	-4(%ebp)
	fldl	-20(%ebp)
