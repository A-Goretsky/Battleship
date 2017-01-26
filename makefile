all: main

main: main.o networking.o print.o
	gcc -o main main.o networking.o print.o

main.o: main.c main.h networking.h
	gcc -c main.c

print.o: print.c print.h
	gcc -c print.c

run: main
	./main

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm main
	rm *~
