#include "kernel.h" 
struct pcb pcbs[NR_PROCESS];
struct pcb *current_pcb;

void block_thread(void);
void init_pcbs(void) {
	/* TODO */
	int i;
	pcbs[0].used = TRUE;  /*idle*/
	current_pcb = &pcbs[0];
	pcbs[0].state = 1;
	for(i = 1; i < NR_PROCESS; i++)
	{
		pcbs[i].ax = pcbs[i].bx = pcbs[i].cx = pcbs[i].dx = 0 ;
		pcbs[i].used = FALSE;
		pcbs[i].state = 0;
		pcbs[i].prev = 0;
		pcbs[i].next = 0;
		pcbs[i].message.count = 0;
		pcbs[i].message.blocked_process = 0;
		pcbs[i].pid = i;
	}
}
void kthread_create(void (*ptr) (void), void *stk_top) {
	/* TODO */
	int i = 0;
	while((pcbs[i].used == TRUE) & (i < NR_PROCESS))
		i++;
	pcbs[i].state = 1; 
	pcbs[i].ip = (uint_16) ptr;
	pcbs[i].sp = (uint_16) stk_top;
	pcbs[i].used = TRUE;
	pcbs[i].fr = 0x0200;
	pcbs[i].cs = pcbs[i].ds = pcbs[i].ss = kernel_segment;
}
void UserThread_create(uint_16 segment)
{
 	int i = 0;
	while((pcbs[i].used == TRUE)&(i <NR_PROCESS))
		i++;
	pcbs[i].state = 1;
	pcbs[i].used = TRUE;
	pcbs[i].fr = 0x282;
	pcbs[i].ip = 0;
	pcbs[i].sp = 0xffff; 
	pcbs[i].es = 0;
	pcbs[i].cs = pcbs[i].ds = pcbs[i].ss = segment;
}
void block_thread()
{
	if(current_pcb->ds == kernel_segment)
	{
		block_kernel_thread();
	}
	else /* user thread*/
	current_pcb->state = 0;
}
void W(struct semaphore* sem)
{
	struct pcb *temp;
	if(sem->blocked_process == 0)
	{
		sem->blocked_process = current_pcb;
	}
	else
	{
		temp = sem->blocked_process;
		while(temp->next != 0)
		{
			temp = temp->next;
		}
		temp->next = current_pcb;	
	 }
	block_thread();
}
void R(struct semaphore *sem)
{
	sem->blocked_process->state = 1;
	sem->blocked_process = sem->blocked_process->next;
}
void kernel_sem_p(struct semaphore *sem)
{
	disable_interrupt();
	sem->count--;
	if(sem->count < 0)
	{
		W(sem);
	}
	enable_interrupt();
}
void kernel_sem_v(struct semaphore *sem)
{
	disable_interrupt();
	sem->count++;
	if(sem->count <= 0)
	{
		R(sem);
	/*	sem->blocked_process->state = 1;*/
	}
	enable_interrupt();
}
