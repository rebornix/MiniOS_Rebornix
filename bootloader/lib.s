.define _printc
_printc:
	push bp
	mov bp, sp
	movb ah, #0xe
	movb al, 4(bp)
	mov bx, #1
	int 0x10
	pop bp
	ret

.define _disk_read
_disk_read:
	push bp
	mov bp, sp

	mov cx, 4(bp)
	mov dx, 6(bp)
	mov ax, 8(bp)
	mov es, ax
	mov bx, 10(bp)
	mov ax, #0x0201

	int 0x13

	pop bp
	ret
