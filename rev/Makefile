all: practica4_pimc practica4_pipes practica4_dispatcher practica4_worker practica4_gatherer
	
#practica4_fifos

CFLAGS=-Wall -g

practica4_pimc: practica4_pimc.o
	gcc practica4_pimc.o -o practica4_pimc -lm

practica4_pipes: practica4_pipes.o practica4_agents.o
	gcc practica4_pipes.o practica4_agents.o -o practica4_pipes -lm

practica4_dispatcher: practica4_dispatcher.o practica4_agents.o
	gcc practica4_dispatcher.o practica4_agents.o -o practica4_dispatcher -lm

practica4_worker: practica4_worker.o practica4_agents.o
	gcc practica4_worker.o practica4_agents.o -o practica4_worker -lm

practica4_gatherer: practica4_gatherer.o practica4_agents.o
	gcc practica4_gatherer.o practica4_agents.o -o practica4_gatherer -lm

#practica4_fifos:

run_fifos:
	mkfifo trabajo resultados
	./practica4_gatherer resultados &
	./practica4_worker trabajo resultados &
	./practica4_worker trabajo resultados &
	./practica4_dispatcher 1000000000 100 trabajo

.PHONY: clean
clean:
	rm -f *.o
	rm -f practica4_pimc practica4_pipes practica4_dispatcher practica4_worker practica4_gatherer practica4_fifos
