all: practica2_std practica2_mmap

practica2_std: practica2.o ordenanotas_std.o
	gcc -Wall practica2.o ordenanotas_std.o -o practica2_std
ordenanotas_std.o: ordenanotas_std.c practica2.h
	gcc -c -Wall ordenanotas_std.c

practica2_mmap: practica2.o ordenanotas_mmap.o
	gcc -Wall practica2.o ordenanotas_mmap.o -o practica2_mmap
ordenanotas_mmap.o: ordenanotas_mmap.c practica2.h
	gcc -c -Wall ordenanotas_mmap.c

practica2.o: practica2.c practica2.h
	gcc -c -Wall practica2.c

.PHONY: clean
clean:
	rm -f *.o
	rm -f practica2_std practica2_mmap

.PHONY: datos
datos:
	tar -xzf datos.tgz
