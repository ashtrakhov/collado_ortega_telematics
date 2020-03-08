#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include "practica2.h"

int ordenanotas(int fd)
{
  int i = 0;
  int j = 0;
  int c = 0;
  int ne = 0;
  int tame = 0;
  uint tam = 0;;
  struct evaluacion aux;
  struct evaluacion *p;
  
  tam = lseek(fd, 0, SEEK_END);

  if (tam <= 0)
  {
    perror("Archivo vacío");

    exit(-1);
  }
  
  tame = sizeof(struct evaluacion);
  
  ne = tam / tame;
  
  lseek(fd, 0, SEEK_SET);

  p = mmap(NULL, tam, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (p < 0)
  {
    perror("Error al emplear el mmap");

    exit(-1);
  }

  for (i = 1; i < ne; i++)
  {
    for (j = 0; j < ne - i; j++)
    {
      if (p[j].notamedia < p[j + 1].notamedia)
      {
        aux = p[j];

        p[j] = p[j+ 1];

        p[j + 1] = aux;

        c++;
      }
    }
  }

  munmap(p, tam);
  
  if (p < 0)
  {
    perror("Error al emplear el munmap");

    exit(-1);
  }

  return c;
}
