#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "practica4.h"

int main(int argc, char* argv[])
{
  int i = 0;
  int num_div = 0;
  int num_paq = 0;
  int num_wor = 0;
  int pid = 0;
  int pipefd1[2];
  int pipefd2[2];

  if (argc != 4)
  {
    perror("Error en los parámetros de entrada\n");

    exit(-1);
  }

  num_div = atoi(argv[1]);

  num_paq = atoi(argv[2]);

  num_wor = atoi(argv[3]);

  if (num_div < 1 || num_paq < 1 || num_wor < 1)
  {
    perror("Error en los parámetros de entrada\n");

    exit(-1);
  }

  if (num_paq > num_div || num_wor > num_div || num_wor > num_paq)
  {
    perror("Error en los parámetros de entrada\n");

    exit(-1);
  }

  pipe(pipefd1);

  if ((pid = fork()) == 0)
  {
    close(pipefd1[0]);

    dispatcher(pipefd1[1], num_paq, num_div);

    close(pipefd1[1]);

    exit(0);
  }

  close(pipefd1[1]);

  pipe(pipefd2);

  for (i = 0; i < num_wor; i++)
  {
    if ((pid = fork()) == 0) {
      close(pipefd2[0]);

      worker(pipefd1[0], pipefd2[1]);

      close(pipefd1[0]);

      close(pipefd2[1]);

      exit(0);
    }
  }

  close(pipefd1[0]);

  close(pipefd2[1]);

  if ((pid = fork()) == 0)
  {
    gatherer(pipefd2[0]);

    close(pipefd2[0]);

    exit(0);
  }
    close(pipefd2[0]);

    for (i = 0; i < (num_wor + 2); i++)
    {
      wait(NULL);
    }
    
    /*for (i = 0; i < num_wor; i++)
    {
      if ((pid = fork()) != 0)
      {
        close(pipefd2[1]);
        
        exit();
      }
      else
      {
        close(pipefd2[0]);
        
        worker(pipefd1[0], pipefd2[1]);
        
        exit();
      }
    }*/
  }

  /*if ((pid = fork()) != 0)
  {
    close(pipefd1[1]);

    exit();
  }
  else
  {
    close(pipefd1[0]);

    gatherer(pipefd2[0]);

    exit();
  }*/
}
