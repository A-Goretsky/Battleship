all: server client

server: main.o networking.o
	gcc -o main main.o networking.o

server.o: main.c networking.h
	gcc -c main.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
