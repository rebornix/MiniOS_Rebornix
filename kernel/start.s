.define _kernel_segment

	mov ax, cs
	mov _kernel_segment, ax

	jmp _main

_kernel_segment:
	.data2 0

