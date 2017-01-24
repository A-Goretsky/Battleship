all: main

main: main.o networking.o
	gcc -o main main.o networking.o

main.o: main.c networking.h
	gcc -c main.c

run: main
	./main

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm main
	rm *~
