#include "kernel.h"

extern struct pcb pcbs[];
extern struct pcb *current_pcb; 
static int count = 0;
void schedule(void) { 
	/* TODO */
	/*
	while(current_pcb->next->state != 1)
		current_pcb = current_pcb->next;
	current_pcb = current_pcb->next;
	*/
	int i, j;
	i =(current_pcb - &pcbs[0])/(&pcbs[1] - &pcbs[0]);
	if( i == NR_PROCESS -1)
		i = 0;
	else 
	if(pcbs[i+1].used == FALSE || pcbs[i+1].state == 0)
	{
		for(j = i+1; j < NR_PROCESS; j++)
			if((pcbs[j].used == TRUE)& (pcbs[j].state == 1))
			{
				i = j;
				break;
			}
			else
				i = 0;
	}
	else
	{
		i++;
		i = i%16;
	}
	current_pcb = &pcbs[i];
	
	/* delete_thread*/
	/*
	count++;
	if(count == 50)
	{
		pcbs[2].used = FALSE;
	}
	*/
}
