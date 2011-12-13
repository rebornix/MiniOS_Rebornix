.define _printc
.define _in_byte
.define _out_byte

_printc: 
	push bp
	mov bp, sp
	movb ah, #0xE
	movb al, 4(bp)
	mov  bx, #1
	int 0x10
	pop bp
	ret

_in_byte:
	push bp
	mov bp, sp
	mov dx, 4(bp)
	inb dx
	xorb ah, ah
	pop bp
	ret

_out_byte:
	push bp
	mov bp, sp
	movb al, 6(bp)
	mov dx, 4(bp)
	outb dx
	pop bp
	ret

