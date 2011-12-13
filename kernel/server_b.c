#include "kernel.h"
void server_b(void)
{
	struct message msg_b;
	msg_b.source = msg_b.type = msg_b.p1 = msg_b.p2 = msg_b.p3 = 0;
	printk("before b send to a\n");
	invoke(1, &msg_b);
	printk("after b receive from a \n");
	while(TRUE)
	{}  
}
