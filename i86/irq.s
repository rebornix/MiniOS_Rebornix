.extern _irq0
.extern _irq1
.extern save_regs
.extern restore_regs
.extern _irq_key


.define _disable_interrupt
.define _enable_interrupt
.define _idle_cpu
.define _block_kernel_thread
.define _irq0_handle
.define _irq1_handle
.define _keyboard_int

_disable_interrupt:
	cli
	ret

_enable_interrupt:
	sti
	ret

_idle_cpu:
	hlt
	ret
_block_kernel_thread:
	int 0x80
	ret 
	
_irq0_handle: ! interrupt handler for irq0
	call save_regs
	call _irq0
	call restore_regs

_irq1_handle:
	call save_regs
	call _irq1
	call restore_regs

_keyboard_int: 
	call save_regs
	call _irq_key
	call restore_regs
