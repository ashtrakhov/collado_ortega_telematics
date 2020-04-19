#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "practica4.h"

int main(int argc, char* argv[])
{
  int fd1 = 0;
  int fd2 = 0;

  if (argc < 3 || argc > 4)
  {
    perror("Error en los parámetros de entrada\n");
  }

  while (1)
  {
    fd1 = open(argv[1], O_RDONLY);

    if (fd1 < 0)
    {
      perror("Error al abrir el fifo 1\n");

      exit(-1);
    }

    fd2 = open(argv[2], O_WRONLY);

    if (fd2 < 0)
    {
      perror("Error al abrir el fifo 2\n");

      exit(-1);
    }

    worker(fd1, fd2);

    close(fd1);

    close(fd2);

    if (argc != 4)
    {
      break;
    }
  }

  return 0;
}
