	.extern _prints
	
	mov ax, #0x07c0
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov sp, #0

	call _main

	push #prompt
	call _prints

	mov ax, #0x8000
	mov ds, ax
	mov ss, ax
	mov sp, #0
	jmpf 0x0, 0x8000

prompt:
	.asciz "jump to 0x8000:0x0000... \r\n"
