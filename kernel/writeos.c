#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>

char buf[2][65536];
int main(int argc, char *argv[])
{
	int fd;
	fd = open(argv[1], O_RDONLY);
	if(fd <= 0)
	{
		printf("Cannot open file\n");
	}
	read(fd, buf[0], 32);
	read(fd, buf[0], 65536);  /* kernel*/
	close(fd);
	
	fd = open(argv[2], O_RDONLY);
	if(fd <= 0)
	{
		printf("Cannot open file\n");
	}
	read(fd, buf[1], 32);
	read(fd, buf[1], 65536);
	close(fd);
	

	fd = open("/dev/fd0", O_WRONLY);
	if(fd <= 0)
	{
		printf("Cannot open disk \n");
		exit(1);
	}
	lseek(fd, 512, SEEK_CUR);
	write(fd, buf, 65536*2);
	close(fd);
	printf("Done\n");
	exit(0);
}
