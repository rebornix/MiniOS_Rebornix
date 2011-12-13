kernel: start.s main.c printk.o panic.o proc.o \
        tests.o util.o timer.o sched.o msg.o tty.o server_call.o
	cd i86 && exec make 
	cc -Was-ncc -mi86 -.o -com -s \
		start.s \
		main.c \
		printk.o \
		panic.o \
		proc.o \
		msg.o \
		tests.o \
  		tty.o \
		server_call.o \
		util.o \
		timer.o \
		sched.o \
		i86/io.o \
		i86/irq.o \
		i86/mem.o \
		i86/reg.o \
		-o kernel
printk.o: printk.c
	cc -Was-ncc -mi86 -c printk.c
panic.o: panic.c
	cc -Was-ncc -mi86 -c panic.c
proc.o: proc.c
	cc -Was-ncc -mi86 -c proc.c
tests.o: tests.c
	cc -Was-ncc -mi86 -c tests.c
msg.o: msg.c
	cc -Was-ncc -mi86 -c msg.c
tty.o : tty.c
	cc -Was-ncc -mi86 -c tty.c
server_call.o: server_call.s
	cc -Was-ncc -mi86 -c server_call.s
server.o: server.c
	cc -Was-ncc -mi86 -c server.c
util.o: util.c
	cc -Was-ncc -mi86 -c util.c
timer.o: timer.c
	cc -Was-ncc -mi86 -c timer.c
sched.o: sched.c
	cc -Was-ncc -mi86 -c sched.c
server: server.c server_call.o  
	  cc -Was-ncc -mi86 -.o -com -s server.c  server.o server_call.o   -o  server 

install: kernel server
	./writeos kernel server
clean:
	cd i86 && exec make clean
	rm *.o kernel
