#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "practica2.h"

int main()
{
  int e = 0;
  int fd = 0;
  int intercambios = 0;
  struct timeval t1, t2; 

  fd = open("datos.bin", O_RDWR);

  gettimeofday(&t1, NULL);

  if (fd < 0)
  {
    perror("Error al abrir el archivo\n");

    exit(-1);
  }
  else
  {
    intercambios = ordenanotas(fd);

    if (intercambios < 0)
    {
      exit(-1);
    }
  }

  gettimeofday(&t2, NULL);

  e = close(fd);

  if (e < 0)
  {
    perror("Error al cerrar el archivo");

    exit(-1);
  }

  printf("Intercambios: %d\n", intercambios);

  printf("Tiempo empleado: %lu us\n", ((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec)));

  exit(0);
}
