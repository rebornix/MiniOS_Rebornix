#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "i86/i86.h"
#include "types.h"
#include "proc.h"
#include "msg.h"
void tty_driver(void);
/* the kernel segment, initialize by start.s */
extern uint_16 kernel_segment;
/* the current process's pcb */
struct pcb *current_pcb;
struct pcb pcbs[NR_PROCESS];
/* a printf */
void printk(char *ctl, ...);
/* a printf test (for test) */
void test_printk(void);
void test1_a(void);
void test1_b(void);
void test1_c(void);
void test_thread_a(void);
void test_thread_b(void);
void test_thread_in_thread(void);
void test_msg_a(void);
void test_msg_b(void);
void test_msg_c(void);
void test_msg_d(void);
/* system panic, invoke this when encountered a error */
void panic(char *str);

/* setup interrupt hanlder */
void setup_irq(uint_16 irq, void (*ptr)(void));
/* interrupt handler for irq0 */
void irq0_handle(void);
void irq1_handle(void);
/* EOI command */
void end_of_interrupt(void);
void InitMsg(struct message *msg);
void invoke(int pid, struct message * msg);
void irq_key(void);
void irq0(void);
void irq1(void);

#endif
