#include "kernel.h"
struct message *test;
void do_invoke(int pid, void *message)
{
	kernel_send(pid, message);
	kernel_receive(message);
}
void irq0(void) {
	end_of_interrupt();
	schedule();
}
void irq1(void) {
	end_of_interrupt();
	if(current_pcb->ds == kernel_segment)
	{
		current_pcb->state = 0;
	}
	else /* int 0x80 for user process*/
	{	
		
	do_invoke((uint_16)current_pcb->ax, (struct message*)current_pcb->bx);
	}
	schedule();
}
void irq_key(void)
{
	static struct message m;
	m.type = HARDWARE_INT;
	kernel_send(2, &m);
	out_byte(0x20, 0x20);
	schedule();
}
