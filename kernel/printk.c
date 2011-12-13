#include "i86/i86.h"
void printd(int data);
void prints(char *str);
void printx(unsigned int data);
void printk(char *ctl, ...) {
	/* TODO */	
	int arg_addr = ((int)&ctl) + 2;
	while(*ctl)
	{
		if(*ctl == '%')
		{
			ctl++;
			if(*ctl == 'd')
			{	
				ctl++;
				printd(*(int*)arg_addr);
				arg_addr +=2;
			}
			else if(*ctl == 's')
			{
				ctl++;
				prints(*(char**) arg_addr);
				arg_addr += 2;
			}
			else if(*ctl == 'x')
			{
				ctl++;
				printc(48);
				printc('x');
				printx(*(unsigned int*)arg_addr);
				arg_addr +=2;
			}
		}
		else
		{
			if(*ctl == '\n') printc('\r');
			printc(*ctl++);
		}
	}
}
void prints(char *str)
{
	for(;*str; str++)
		printc(*str);
} 
void printd(int data)
{
	int mod;
	if(data < 0)
	{
	printc('-');
	data = -1*data;
	}
	mod = data%10;
	 data = data/10;
	if(data == 0)
	{	
		printc(mod+48);
	}
	else
	{
		printd(data);
		printc(mod+48);
	}
}
void printx(unsigned int data)
{
	unsigned int mod = data%16;
	data = data/16;
	if(data != 0)
	{
		printx(data); 
	}
		if(mod == 10)
			printc('A');
		else if(mod == 11)
			printc('B');
		else if(mod == 12)
			printc('C');
		else if(mod == 13)
			printc('D');
		else if(mod == 14)
			printc('E');
		else if(mod == 15)
			printc('F');
		else
			printc(mod+48);

}
		

void test_printk(void) {
	printk("Here begins test for printk.\n");
	printk("  Numbers: %d, %d, %d, %d, %x.\n", 0, 42, 2011, -1024, 0xABCD);
	printk("  Strings: \"%s\", \"%s\"\n", "Hello", "World");
}
