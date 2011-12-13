/* this is a test server,
 * run in user-space.
 *
 * it read a line from the terminal,
 * then convert all lowercase letters
 * to uppercase,
 * and write it down.
 */
#include "kernel.h"
char *hell = "May the force be with you!\n";
char buf[100];

int main(void) {
	int i, len;
	struct message m;
	char *str;
	m.source = 1;
	m.type = DEV_WRITE;
	m.p1   = (int)hell;
	m.p2   = 27;
	invoke(2, &m);

	while (1) {
		m.type = DEV_READ;
		m.p1 = (int)buf;
		invoke(2, &m);
		
		len = m.p1;
		buf[len ++] = '\n';
		buf[len ] = 0;
		for (str = buf; *str; str ++)
			if (*str >= 'a' && *str <= 'z')
				*str = *str + 'A' - 'a';
		m.source  =1;
		m.type = DEV_WRITE;
		m.p1 = (int)buf;
		m.p2 = len;
		invoke(2, &m);
	}

}

