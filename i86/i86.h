#ifndef _I86_H_
#define _I86_H_

void printc(char str);

unsigned char in_byte(unsigned int port);
void out_byte(unsigned int port, unsigned char data);

void disable_interrupt(void);
void enable_interrupt(void);
void idle_cpu(void);
void block_kernel_thread(void);

void keyboard_int(void);
void save_regs(void);
void restore_regs(void);

void physics_copy(unsigned int src_segment, 
                  unsigned int src_offset,
                  unsigned int dest_segment,
                  unsigned int dest_offset);

#endif
