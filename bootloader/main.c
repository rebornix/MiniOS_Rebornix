#include"lib.h"
char buf[512];
void main(void)
{
	int i, j, k;
	unsigned int base = 0x8000;
	unsigned int server_a_base = 0x2000;
	unsigned int server_b_base = 0x3000;
	prints("Bootloader:\r\n");
	prints(" Loading #2..#129 sectors into M[0x8000:0x0000].");
	for(i = 0;i < 128; i++)
	{
	if((i &63) == 0)
	prints("\r\n ");
	read_sector(i+2, base, 0);
	base += 32;
	prints(".");
	}
	prints("Done. \r\n");
	
	prints(" Loading #130..#257 sectors into M[0x2000:0x0000].");
	for(j = 0;j < 128;j++)
	{
	if((j &63) == 0)
	prints("\r\n ");
	read_sector(j+2+128, server_a_base, 0);
	server_a_base += 32;
	prints(".");
	}
	prints("done. \r\n");

	prints(" Loading #258..# sectors into M[0x3000:0x0000].");
	for( k= 0;k < 128;k++)
	{
	if((k &63) == 0)
	prints("\r\n ");
	read_sector(k+2+256, server_b_base, 0);
	server_b_base += 32;
	prints(".");
	}
	prints("done. \r\n");
}
