#include "kernel.h"

void end_of_interrupt(void) {
	out_byte(0x20, 0x20);
}

void write_byte(uint_8 data, uint_16 seg, uint_16 offset) {
	physics_copy(kernel_segment, &data, seg, offset);
}
void setup_irq(uint_16 irq, void (*ptr)(void)) {
	write_byte(((uint_16)ptr) % 256, 0, 4 * irq + 0);
	write_byte(((uint_16)ptr) / 256, 0, 4 * irq + 1);
	write_byte((kernel_segment) % 256, 0, 4 * irq + 2);
	write_byte((kernel_segment) / 256, 0, 4 * irq + 3);
}
