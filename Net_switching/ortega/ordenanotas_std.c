#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "practica2.h"

int ordenanotas(int fd)
{
  int i = 0;
  int j = 0;
  int c = 0;
  int ne = 0;
  int tame = 0;
  uint tam = 0;;
  struct evaluacion s1, s2;

  tam = lseek(fd, 0, SEEK_END);

  if (tam <= 0)
  {
    perror("Archivo vacío");

    exit(-1);
  }

  tame = sizeof(struct evaluacion);

  ne = tam / tame;

  for (i = 1; i < ne; i++)
  {
    for (j = 0; j < ne - i; j++)
    {
      lseek(fd, j * tame, SEEK_SET);

      read(fd, &s1, tame);

      read(fd, &s2, tame);

      if (s1.notamedia < s2.notamedia)
      {
        lseek(fd, j * tame, SEEK_SET);

        write(fd, &s2, tame);

        write(fd, &s1, tame);

        c++;
      }
    }
  }

  return c;
}
