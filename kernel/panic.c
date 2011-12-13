#include "kernel.h"

void panic(char *str) {
	/* TODO: you can modify it as your wish */
	disable_interrupt();
	printk("\n\n** Kernel panic: %s **\n", str);
	while (1) {
		idle_cpu();
	}
}
