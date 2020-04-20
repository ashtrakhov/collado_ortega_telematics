#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char* argv[])
{
  int i = 0;
  int num_div = 0;
  int num_paq = 0;
  int num_wor = 0;
  int pid = 0;

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

  if (mkfifo("trabajo", 00664) < 0)
  {
    perror("Error al crear el fifo 1\n");

    exit(-1);
  }

  if ((pid = fork()) == 0)
  {
    char* dargv[] = {"practica4_dispatcher", argv[1], argv[2], "trabajo", NULL};

    char* denv[] = {NULL};

    if (execve(dargv[0], dargv, denv) < 0)
    {
      perror("Error al lanzar el programa dispatcher\n");

      exit(-1);
    }
  }

  if (mkfifo("resultados", 00664) < 0)
  {
    perror("Error al crear el fifo 2\n");

    exit(-1);
  }

  for (i = 0; i < num_wor; i++)
  {
    if ((pid = fork()) == 0)
    {
      char* wargv[] = {"practica4_worker", "trabajo", "resultados", NULL};
      
      char* wenv[] = {NULL};
      
      if (execve(wargv[0], wargv, wenv) < 0)
      {
        perror("Error al lanzar el programa worker\n");
        
        exit(-1);
      }
    }
  }

  if ((pid = fork()) == 0)
  {
    char* gargv[] = {"practica4_gatherer", "resultados", NULL};

    char* genv[] = {NULL};

    if (execve(gargv[0], gargv, genv) < 0)
    {
      perror("Error al lanzar el programa gatherer\n");

      exit(-1);
    }
  }

  for (i = 0; i < (num_wor + 2); i++)
  {
    wait(NULL);
  }

  return 0;
}
