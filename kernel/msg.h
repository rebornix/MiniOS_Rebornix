#include "kernel.h"

typedef struct message
{
	int source;
	int type;
	int p1;
	int p2;
	int p3;
};

void kernel_send(int pid, struct message *m);
void kernel_receive(struct message *m);
