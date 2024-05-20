poly:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$poly.size, %esp
#Generating Dereference
	movl	16(%ebp), %eax
	fldl	(%eax)
	fstpl	-12(%ebp)
