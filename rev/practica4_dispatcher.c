#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "practica4.h"

int main(int argc, char* argv[])
{
  int num_div = 0;
  int num_paq = 0;
  int fd1 = 0;

  if (argc != 4)
  {
    perror("Error en los parámatros de entrada\n");

    exit(-1);
  }

  num_div = atoi(argv[1]);

  num_paq = atoi(argv[2]);

  if (num_div < 1 || num_paq < 1)
  {
    perror("Error en los parámetros de entrada\n");

    exit(-1);
  }

  if (num_paq > num_div)
  {
    perror("Error en los parámetros de entrada\n");

    exit(-1);
  }

  fd1 = open(argv[3], O_WRONLY);

  if (fd1 < 0)
  {
    perror("Error al abrir el fifo 1\n");

    exit(-1);
  }

  dispatcher(fd1, num_paq, num_div);

  close(fd1);

  return 0;
}
