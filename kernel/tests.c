#include "kernel.h"
char stackson[KSTACK_SIZE];
void InitMsg(struct message *msg)
{
	msg->source = 0;
	msg->type = 0;
	msg->p1 = 0;
	msg->p2 = 0;
	msg->p3 = 0;
}
void EvalMsg(struct message *msg, int source, int type , int p1, int p2, int p3)
{
	msg->source = source;
	msg->type = type;
	msg->p1 = p1;
	msg->p2 = p2;
	msg->p3 = p3;
}
/* testcase1: simple abc */
void test1_a(void) {
	while (TRUE) 
	{	
	printk("a");
	idle_cpu();
	}
}
void test1_b(void) {
	while (TRUE) 
	{
	printk("b");
	idle_cpu();
	}

}
void test1_c(void) {
	while (TRUE) 
	{
	printk("c");
	idle_cpu();
	}
}

/* testcase2: critical area test
 * if run with "#define ATOMIC", we will always get 0 as the last input
 * elsewise, we will get random numbers.
 *
 * try this you will have a better understanding of "critical area" */

static uint_16 count = 0;
struct semaphore mutex = 1;
void test_thread_a(void) {
	int i, j, k, l, tmp;
	for (i = 0; i < 256; i ++)
	for (j = 0; j < 128; j ++)
	for (k = 0; k < 1; k ++) { /* loop for 512*512*512 times */
		kernel_sem_p(&mutex);
		tmp = count + 1; /* HEY!!!!!! CRITICAL AREA!!!!!! */
		count = tmp;
		kernel_sem_v(&mutex);
	}
	disable_interrupt();
	printk("A reported: total = %d\n", count); 
	enable_interrupt();
	while (TRUE) idle_cpu();
}
void test_thread_b(void) {
	int i, j, k, l, tmp;
	for (i = 0; i < 256; i ++)
	for (j = 0; j < 128; j ++)
	for (k = 0; k < 1; k ++) {
		kernel_sem_p(&mutex);
		tmp = count + 1;
		count = tmp;
		kernel_sem_v(&mutex);
	}
	disable_interrupt();
	printk("B reported: total = %d\n", count);
	enable_interrupt();
	while (TRUE) idle_cpu();
}

/* you are to design new testcases for:
    * more critical areas, or any funny things
    * thread dynamic create (create thread in another thread)
	* thread create-elimination (create-eliminate, ...)
	* any other testcase you wanted
	*
	* may the force be with you.
*/
void test_thread_in_thread(void)
{
	while(TRUE)
	{
		printk("f");
		kthread_create(test1_a, stackson + KSTACK_SIZE);
		idle_cpu();
	}
}
void test_msg_a()
{
	struct message a, b;
	a.source = 7;
	a.type = 0;
	a.p1 = 1;
	a.p2 = 2;
	a.p3 = 3;
	kernel_send(2, &a);
	printk("A send a message to B\n");
	kernel_receive(&b);
	printk("A receive a message from B  source: %d, type: %d, p1: %d, p2: %d, p3: %d\n", b.source, b.type, b.p1, b.p2, b.p3);
	while(1)
		idle_cpu();
}
void test_msg_b()
{
	struct message  d;
	struct message c = {0, 0, 0, 0, 0};
	/*
	c.source = 0;
	c.type = 0;
	c.p1 = 0;
	c.p2 = 0;
	c.p3 = 0;
	*/
	kernel_receive(&c);
	printk("B receive a message from A  source: %d, type: %d, p1: %d, p2: %d, p3: %d\n", c.source, c.type, c.p1, c.p2, c.p3);
	d.source = 32;
	d.type = 1;
	d.p1 = 3;
	d.p2 = 6;
	d.p3 = 9;
	kernel_send(1, &d);
	printk("B send a message to A\n");
	while (1)
		idle_cpu();
}
void test_msg_c()
{
	struct message x ;

	struct message y ;
	struct message z ;
	EvalMsg(&x, 12, 2, 23, 76, 28);
	EvalMsg(&y, 23, 1 ,73, 27, 56);
	InitMsg(&z);
	kernel_send(2, &x);
	printk("C send a message x to D source: %d, type %d, p1: %d, p2: %d, p3: %d \n", x.source, x.type, x.p1 ,x.p2, x.p3);
	kernel_send(2, &y);
	printk("C send a message y to D source: %d, type %d, p1: %d, p2: %d, p3: %d \n", y.source, y.type, y.p1 ,y.p2, y.p3);
	kernel_receive(&z);
	printk("C receive a message z from D source: %d, type %d, p1: %d, p2: %d, p3: %d \n", z.source, z.type, z.p1 ,z.p2, z.p3);
	while(1)
		idle_cpu();
}

void test_msg_d()
{
	struct message x ;
	struct message y ;
	struct message z = {64, 0, 45, 36, 27};
	InitMsg(&x);
	InitMsg(&y);
	EvalMsg(&z, 64, 0, 45, 36, 27);
	kernel_receive(&x);
	printk("D receive a message x from C source: %d, type %d, p1: %d, p2: %d, p3: %d \n", x.source, x.type, x.p1 ,x.p2, x.p3);
	kernel_receive(&y);
	printk("D receive a message y from D source: %d, type %d, p1: %d, p2: %d, p3: %d \n", y.source, y.type, y.p1 ,y.p2, y.p3);
	kernel_send(1, &z);
	printk("D send a message x to C source: %d, type %d, p1: %d, p2: %d, p3: %d \n", z.source, z.type, z.p1 ,z.p2, z.p3);
	while (1)
		idle_cpu();
}
