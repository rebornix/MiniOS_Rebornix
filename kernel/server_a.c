#include "kernel.h"
void server_a(void)
{	
 	struct message msg_a;
	msg_a.source = msg_a.type = msg_a.p1 = msg_a.p2 = msg_a.p3 = 0;
	printk("before a send to b \n");
	invoke(2, &msg_a);
	printk("after a receive from b \n");
	while(TRUE)
	{}
}
