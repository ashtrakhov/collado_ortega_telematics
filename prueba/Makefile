all: practica4_pimc
	
#practica4_pipes practica4_dispatcher practica4_worker practica4_gatherer practica4_fifos

CFLAGS=-Wall -g -DDEBUG=1

practica4_pimc: practica4_pimc.o
	gcc practica4_pimc.o -o practica4_pimc -lm

#practica4_pipes: practica4_pipes.o practica4_agents.o
#gcc practica4_pipes.o practica4_agents.o -o practica4_pipes -lm

#practica4_dispatcher:

#practica4_worker:

#practica4_gatherer:

#practica4_fifos:

.PHONY: clean
clean:
	rm -f *.o
	rm -f practica4_pimc practica4_pipes practica4_dispatcher practica4_worker practica4_gatherer practica4_fifos
