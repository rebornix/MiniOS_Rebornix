/* The terminal driver.
 * this driver receives several types of messages:
 *   DEV_READ: user process read from terminal.
 *             when read done, a message is fired.
 *   DEV_WRITE: user process write to terminal. 
 *              when write done, a message is fired.
 *   HARDWARE_INT: hardware interrupt message 
 *
 * to get all these work, you must:
 *   1. write a keyboard interrupt handler, just like our timer interrupt
 *      FOR THIS, YOU WILL WRITE VERY LITTLE ASSEMBLY CODE!!! CAREFUL!!!
 *   2. create a kernel thread start from 
 *        void tty_driver(void);
 *
 * you may have some problem during merging this code,
 * so you must read and understand it before you start.
 *  */

#include "kernel.h"
#define TTY_BUF_SIZE 256

void init_keymaps(void);
int read_scancode(void);
void update_flags(int);
char get_key(int);
void tty_enqueue(char);
void ring_buffer_enqueue(char*);
void ring_buffer_dequeue(char*);
int ring_buffer_empty(void);

/* the scancode -> ascii code map */
char key_map[256] = {
	0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
	'-', '=', '\b', '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u',
	'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's', 'd', 'f', 'g',
	'h', 'j', 'k', 'l', ';', '\'', 0, 0, 0, 'z', 'x', 'c', 'v',
	'b', 'n', 'm', ',', '.', '/'
};
/* the shifted key map */
char key_map_shift[256] = {
	0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
	'_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U',
	'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S', 'D', 'F', 'G',
	'H', 'J', 'K', 'L', ':', '"', 0, 0, 0, 'Z', 'X', 'C', 'V',
	'B', 'N', 'M', '<', '>', '?'
};

/* functional key scancodes */
#define CTRL_HIT		29
#define CTRL_RELEASE	157
#define ALT_HIT			56
#define ALT_RELEASE		184
#define SHIFT_HIT		42
#define SHIFT_RELEASE	170
#define CAPS_LOCK		58

/* keypressed flags */
static int ctrl = FALSE;
static int alt = FALSE;
static int shift = FALSE;
static int caps = FALSE;

/* IMPORTANT: let this be PID of tty_driver */

static char tmp_buf[100];
static int read_request = 0;
static int read_stack[NR_PROCESS];
static int read_buffer[NR_PROCESS];

/* the main driver */
void tty_driver(void) {
	static struct message m; /* a message for send and receive */
	m.type = m.source = m.p1 = m.p2 = m.p3 = 0;
	init_keymaps(); /* initialize keymaps */

	while (TRUE) { /* forever, wait for message */
		kernel_receive(&m);
		if (m.type == HARDWARE_INT) { /* hardware interrupt */
			int code;
			char ch;

			code = read_scancode(); /* read something from hardware */
			update_flags(code);
			ch = get_key(code);

			if (ch != 0) {
				tty_enqueue(ch); /* if is a valid key, enqueue it */
			}
		} else if (m.type == DEV_READ) { /* read request */
			read_stack[read_request] = m.source; /* just enqueue it */
			read_buffer[read_request] = m.p1;
			read_request ++;
		} else if (m.type == DEV_WRITE) { /* write request */
			char ch;
			int i, seg = pcbs[m.source].ds; /* ATTENTION!! YOU MUST IMPLEMENT THIS!!! */
			
			for (i = 0; i < m.p2; i ++) { /* get a byte, a cross-segment copy */
				physics_copy(seg,
					(uint_16)(m.p1 + i),
					kernel_segment,
					&ch );
				printc(ch ); /* print it */
			}
			m.type = DEV_DONE;
			kernel_send(m.source, &m); /* send ack */
		}
		if (read_request > 0 && ring_buffer_empty() == FALSE) { /* if there is buffered text, and someone is reading */
			int pid = read_stack[read_request - 1]; /* pop it */
			int addr = read_buffer[read_request - 1];
			int i, seg = pcbs[pid].ds; /* same as write */
			char *str;

			read_request --;
			ring_buffer_dequeue(tmp_buf); /* dequeue */
			str = tmp_buf;
			for (i = 0; *str; i ++, str ++) { /* copy read result */
				physics_copy(
					kernel_segment,
					(uint_16)str, (uint_16)seg, (struct message*)(addr + i));
			}
			m.p1 = i;
			m.type = DEV_DONE;
			kernel_send(pid, &m); /* send ack */
		}
	}
}

void init_keymaps(void) {
	key_map[43] = '\\';
	key_map_shift[43] = '|';
	key_map[41] = '`';
	key_map_shift[41] = '~';
	key_map[57] = ' ';
	key_map_shift[57] = ' ';
}

int read_scancode(void) {
	int code = in_byte(0x60);
	int val = in_byte(0x61);
	out_byte(0x61, val | 0x80);
	out_byte(0x61, val);
	return code;
}

void update_flags(int code) {
	if      (code == CTRL_HIT) ctrl = TRUE;
	else if (code == CTRL_RELEASE) ctrl = FALSE;
	else if (code == ALT_HIT) alt = TRUE;
	else if (code == ALT_RELEASE) alt = FALSE;
	else if (code == SHIFT_HIT) shift = TRUE;
	else if (code == SHIFT_RELEASE) shift = FALSE;
	else if (code == CAPS_LOCK) caps ^= TRUE;
}

char get_key(int code) {
	int need_shift = shift;
	char ch = key_map[code];
	if (code >= 128 || ch == 0) return 0;
	if (ch >= 'a' && ch <= 'z' && caps)
		need_shift = TRUE;
	if (need_shift) return key_map_shift[code];
	else return ch;
}

static char ring_buffer[TTY_BUF_SIZE];
static int front = 0, rear = 0;

static char line_buffer[81];
static int cursor = 0;


void ring_buffer_enqueue(char *str) {
	for (; *str; str ++) {
		ring_buffer[rear] = *str;
		rear = (rear + 1) % TTY_BUF_SIZE;
	}
	ring_buffer[rear] = 0;
	rear = (rear + 1) % TTY_BUF_SIZE;
}
void ring_buffer_dequeue(char *str) {
	while (ring_buffer[front] != 0) {
		*str = ring_buffer[front];
		str ++;
		front = (front + 1) % TTY_BUF_SIZE;
	}
	*str = 0;
	front = (front + 1) % TTY_BUF_SIZE;
}
int ring_buffer_empty(void) {
	return front == rear;
}

void tty_enqueue(char ch) {
	if (ch == '\b') {
		if (cursor > 0) {
			cursor --;
			printk("\b \b");
		}
	} else if (ch == '\n') {
		printk("\n");
		line_buffer[cursor] = 0;
		ring_buffer_enqueue(line_buffer);
		cursor = 0;
	} else {
		if (cursor < 80) {
			printc(ch);
			line_buffer[cursor ++] = ch;
		}
	}
}
