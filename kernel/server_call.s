.define _invoke

_invoke:
	push bp
	mov bp, sp
	mov ax, 4(bp)
	mov bx, 6(bp)
	int 0x80
	pop bp
	ret
