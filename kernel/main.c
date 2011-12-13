#include "kernel.h" 
char stack1[KSTACK_SIZE]; 
char stack2[KSTACK_SIZE]; 
char stack3[KSTACK_SIZE];
char stack4[KSTACK_SIZE];
extern uint_8 stack_a[], stack_b[];
int main(void) {
	int i;

	/* TODO: run test_printk() */
	/*
	test_printk();
*/
	disable_interrupt();
	init_pcbs();
	setup_irq(8, irq0_handle);
	setup_irq(0x80, irq1_handle);
	setup_irq(9, keyboard_int);
	/*
	kthread_create(test1_a, stack1+KSTACK_SIZE);
	kthread_create(test1_b, stack2+KSTACK_SIZE);
	kthread_create(test1_c, stack3+KSTACK_SIZE);
	kthread_create(test_thread_a, stack1+KSTACK_SIZE);
	kthread_create(test_thread_b, stack2+KSTACK_SIZE);
	
	kthread_create(test_msg_c, stack1+KSTACK_SIZE);
	kthread_create(test_msg_d, stack2+KSTACK_SIZE);
	
	kthread_create(test_thread_in_thread, stack1+KSTACK_SIZE);
	*/
	UserThread_create(0x2000);
	kthread_create(tty_driver, stack2+KSTACK_SIZE);
        enable_interrupt(); 
	
	while (1) {
		idle_cpu();
	}
}


