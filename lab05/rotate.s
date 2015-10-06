# Author: Jeremy Jacobson
# Email: jjacobson@sandiego.edu

.text
.globl rotate
.type	rotate, @function

rotate:
	pushl	%ebp                # stack set-up code
	movl	%esp, %ebp   
	movl	0x10(%ebp), %ebx	# %ebx = c 
	movl	(%ebx), %eax		# %eax = *%ebx (temp = *c)
	movl	0xc(%ebp), %esi 	# %esi = b
	movl	(%esi), %edx		# %edx = *%esi (*b)
	movl	%edx, (%ebx)		# *%ebx = %edx (*c = *b)
	movl	0x8(%ebp), %ebx		# %ebx = a
	movl	(%ebx), %edx		# %edx = *%esi (*a)
	movl 	%edx, (%esi)		# *%esi = %edx (*b = *a)
	movl	%eax, (%ebx)		# *%ebx = %eax (*a = temp)
	leave                       # function return code
	ret

