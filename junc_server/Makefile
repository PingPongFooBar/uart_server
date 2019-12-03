uart_server: junc_server.o sock.o junc.o
	gcc -o junc_server junc_server.o sock.o junc.o -lpthread
junc_server.o:junc_server.c
	gcc -o junc_server.o -c junc_server.c
sock.o:sock.c
	gcc -o sock.o -c sock.c
junc.o:junc.c
	gcc -o junc.o -c junc.c
clean:
	rm -f junc_server sock junc *.o
