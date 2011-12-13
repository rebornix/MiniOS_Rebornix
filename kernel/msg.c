#include "kernel.h"

struct message MsgQue[NR_PROCESS];
uint_16 queue_num  = 0;
void enqueue_message(struct message msg);
void dequeue_message(void);

void kernel_send(int pid, struct message *m)
{
	int i, j;
	struct message temp;
	struct pcb *p = &(pcbs[pid]);
	/* m.source */
	for(j = 1; j< NR_PROCESS;j++)
	{
		if(current_pcb->ds == pcbs[i].ds)
		{
			m->source = pcbs[i].pid;
		}
	}
	disable_interrupt();

	if(p->message.count <0)
		/* directly copy the message */
	{
	/*
		physics_copy(current_pcb->ds, (uint_16)&(m->source), p->ds, (uint_16)&( ( (struct message *)p->message_addr )->source) );
		physics_copy(current_pcb->ds, (uint_16)&(m->type), p->ds, (uint_16)&( ( (struct message *)p->message_addr )->type) );
		physics_copy(current_pcb->ds, (uint_16)&(m->p1), p->ds, (uint_16)&( ( (struct message *)p->message_addr )->p1) );
		physics_copy(current_pcb->ds, (uint_16)&(m->p2), p->ds, (uint_16)&( ( (struct message *)p->message_addr )->p2) );
		physics_copy(current_pcb->ds, (uint_16)&(m->p3), p->ds, (uint_16)&( ( (struct message *)p->message_addr )->p3) );
	*/
		for(i = 0; i< 10; i++)
		{
			physics_copy(current_pcb->ds, (uint_16)m+i, p->ds, p->message_addr+i);
		}
	
	}
	else
	 /*enqueue the message */
	{
	/*
		physics_copy(current_pcb->ds, (uint_16)&(m->source), kernel_segment, (uint_16)&(temp.source));
		physics_copy(current_pcb->ds, (uint_16)&(m->type), kernel_segment, (uint_16)&(temp.type));
		physics_copy(current_pcb->ds, (uint_16)&(m->p1), kernel_segment, (uint_16)&(temp.p1));
		physics_copy(current_pcb->ds, (uint_16)&(m->p2), kernel_segment, (uint_16)&(temp.p2));
		physics_copy(current_pcb->ds, (uint_16)&(m->p3), kernel_segment, (uint_16)&(temp.p3));
		enqueue_message(temp);
	*/
	
		for(i = 0; i< 10; i++)
		{
			physics_copy(current_pcb->ds, (uint_16)m+i, kernel_segment, (uint_16)&temp+i);
		}
		enqueue_message(temp);
	}
	pcbs[pid] = *p;
	kernel_sem_v(&(pcbs[pid].message));
	enable_interrupt();
	
}
void kernel_receive(struct message *m)
{
	int i;
	disable_interrupt();

	if(current_pcb->message.count >0)
	{
		kernel_sem_p(&current_pcb->message);
		/*dequeue the message*/
		/*
		physics_copy(kernel_segment, (uint_16)&(MsgQue[0].source), current_pcb->ds, (uint_16)&(m->source));
		physics_copy(kernel_segment, (uint_16)&(MsgQue[0].type), current_pcb->ds, (uint_16)&(m->type));
		physics_copy(kernel_segment, (uint_16)&(MsgQue[0].p1), current_pcb->ds, (uint_16)&(m->p1));
		physics_copy(kernel_segment, (uint_16)&(MsgQue[0].p2), current_pcb->ds, (uint_16)&(m->p2));
		physics_copy(kernel_segment, (uint_16)&(MsgQue[0].p3), current_pcb->ds, (uint_16)&(m->p3));
		*/
		
		for(i = 0; i< 10; i++)
		{
			physics_copy(kernel_segment, (uint_16)&MsgQue[0]+i, current_pcb->ds, (uint_16)m+i);
		}
		dequeue_message();
	}
	else
	{
		/*save message address into pcb */
		current_pcb->message_addr = (uint_16)m;
		kernel_sem_p(&current_pcb->message);
	}
	enable_interrupt();
}
void enqueue_message(struct message msg)
{
	if(queue_num < NR_PROCESS)
	{
		MsgQue[queue_num] = msg;	
		
	}
	queue_num++;
}
void dequeue_message(void)
{
	int i;
	for(i = 0;i<NR_PROCESS-1;i++)
	{
		MsgQue[i] = MsgQue[i+1];
	}
	queue_num--;
}
