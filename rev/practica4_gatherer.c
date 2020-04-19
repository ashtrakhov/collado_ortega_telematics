#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "practica4.h"

int main(int argc, char* argv[])
{
  int fd2 = 0;

  if (argc < 2 || argc > 3)
  {
    perror("Error en los párametros de entrada\n");

    exit(-1);
  }

  while (1)
  {
    fd2 = open(argv[1], O_RDONLY);

    if (fd2 < 0)
    {
      perror("Error al abrir el fifo 2\n");

      exit(-1);
    }

    gatherer(fd2);

    close(fd2);

    if (argc != 3)
    {
      break;
    }
  }

  return 0;
}
