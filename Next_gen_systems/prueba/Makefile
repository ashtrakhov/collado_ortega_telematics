all: practica3_1 practica3_2

practica3_1: practica3.o f1.o
	gcc practica3.o f1.o -o practica3_1 -lm -lpthread
f1.o: f1.c practica3.h
	gcc -g -c -Wall f1.c

practica3_2: practica3.o f2.o
	gcc practica3.o f2.o -o practica3_2 -lm -lpthread
f2.o: f2.c practica3.h
	gcc -g -c -Wall f2.c

practica3.o: practica3.c practica3.h
	gcc -g -c -Wall practica3.c

.PHONY: clean
clean:
	rm -f *.o
	rm -f practica3_1 practica3_2
