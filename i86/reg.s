.extern _kernel_segment
.define save_regs
.define restore_regs

	AX = 0 * 2
	BX = 1 * 2
	CX = 2 * 2
	DX = 3 * 2
	BP = 4 * 2
	SP = 5 * 2
	SI = 6 * 2
	DI = 7 * 2
	CS = 8 * 2
	DS = 9 * 2
	SS = 10 * 2
	ES = 11 * 2
	IP = 12 * 2
	FR = 13 * 2
	STACK = 14 * 2 + 256

save_regs:
	push ds
	push si
	cseg mov ds, _kernel_segment
	mov si, _current_pcb

	mov AX(si), ax
	mov BX(si), bx
	mov CX(si), cx
	mov DX(si), dx
	pop SI(si)
	mov DI(si), di
	mov BP(si), bp
	mov ES(si), es
	pop DS(si)
	pop dx 
	pop IP(si)
	pop CS(si)
	pop FR(si)
	mov SP(si), sp
	mov SS(si), ss

	mov ax, cs
	mov ss, ax
	lea sp, STACK(si)

	jmp (dx)

restore_regs:
	pop bx ! return value, should be ommited

	mov si, _current_pcb
	mov ax, AX(si)
	mov bx, BX(si)
	mov cx, CX(si)
	mov dx, DX(si)
	mov di, DI(si)
	mov bp, BP(si)
	mov es, ES(si)
	mov ss, SS(si)
	mov sp, SP(si)
	push FR(si)
	push CS(si)
	push IP(si)
	push DS(si)
	mov si, SI(si)
	pop ds
	iret
