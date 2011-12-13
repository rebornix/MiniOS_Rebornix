.define _physics_copy

_physics_copy: ! physics memory copy
	push bp
	mov bp, sp

	mov ax, 4(bp)
	mov es, ax
	mov bx, 6(bp)
	eseg movb al, (bx)
	push ax

	mov ax, 8(bp)
	mov es, ax
	mov bx, 10(bp)
	pop ax
	eseg movb (bx), al

	pop bp
	ret

