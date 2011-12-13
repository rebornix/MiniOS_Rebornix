all: io.o irq.o mem.o reg.o
io.o: io.s
	cc -mi86 -Was-ncc -c io.s
irq.o: irq.s
	cc -mi86 -Was-ncc -c irq.s
mem.o: mem.s
	cc -mi86 -Was-ncc -c mem.s
reg.o: reg.s
	cc -mi86 -Was-ncc -c reg.s
clean:
	rm *.o
