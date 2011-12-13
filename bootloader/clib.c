void disk_read(int, int, int, int);
void printc(char);

void read_sector(int id, int seg, int offset)
{
	int p = (id-1)/18;	
	int q = (id-1)%18;
	int head = p&1;
	int track = p>>1;
	int sector = q+1;
	disk_read((track<<8)|sector, (head<<8), seg, offset);
}

void prints(char *str)
{ for(;*str;str++)
  printc(*str);
}

