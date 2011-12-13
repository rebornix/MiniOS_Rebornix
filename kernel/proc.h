#ifndef _PROC_H_
#define _PROC_H_

#include "types.h"

/* total processes in the system */
#define NR_PROCESS    16
/* kernel stack size */
#define KSTACK_SIZE   256
/* BEWARE THAT YOU HAVE ONLY 64KiB MEMORY! */
typedef struct semaphore 
{
	int count;
	struct pcb* blocked_process;
};
/* the PCB */
typedef struct pcb {
	uint_16 ax, bx, cx, dx;
	uint_16 bp, sp, si, di;
	uint_16 cs, ds, ss, es;
	uint_16 ip, fr;

	uint_8 kernel_stack[KSTACK_SIZE];
	
	bool used;
	unsigned int state;
	unsigned int pid;
	struct semaphore message;
	int message_addr;
	struct pcb *prev, *next;
};
void kernel_sem_p(struct semaphore *sem);
void kernel_sem_v(struct semaphore *sem);
/* initialize pcbs */
void init_pcbs(void);
/* create a kernel thread at ptr, with stack top stk_top */
void kthread_create(void (*ptr) (void), void *stk_top);
void UserThread_create(uint_16 segment);
#endif
