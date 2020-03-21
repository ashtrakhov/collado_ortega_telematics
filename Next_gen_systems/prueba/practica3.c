#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "practica3.h"

double res = 0;
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[])
{
  int i = 0;
  int num_hil = 0;
  int num_div = 0;
  int r = 0;
  pthread_t *h;
  struct datos *dat;

  if (argc != 3)
  {
    perror("Error en los parámetros de entrada\n");

    exit(-1);
  }

  num_hil = atoi(argv[1]);

  num_div = atoi(argv[2]);

  if (num_hil < 1 || num_div < 1)
  {
    perror("Error en los parámetros de entrada\n");

    exit(-1);
  }

  h = (pthread_t *) calloc(num_hil, sizeof(pthread_t));

  dat = (struct datos *) calloc(num_hil, sizeof(struct datos));

  r = num_div % num_hil;

  for (i = 0; i < num_hil; i++)
  {
    dat[i].div_hil = num_div / num_hil;
    
    if (r != 0)
    {
      dat[i].div_hil += 1;
      
      r--;
    }
  }

  /*while (r != 0)
  {
    for (i = 0; i < num_hil; i++)
    {
      if (r != 0)
      {
        dat[i].div_hil += 1;
        
        r--;
      }
    }
  }*/

  for (i = 0; i < num_hil; i++)
  {
    dat[i].tam_div = 1 / (double)num_div;
    
    dat[i].ini = i * dat[i].tam_div * dat[i].div_hil;
    
    pthread_create(&h[i], NULL, fhilo, &dat[i]);
  }

  for (i = 0; i < num_hil; i++)
  {
    pthread_join(h[i], NULL);
  }

  res *= 4;

  printf("Valor calculado: %.15lf\n", res);
}

void *fhilo(void *dat)
{
  int i = 0;
  double in = 0;
  double fi = 0;
  double a = 0;
  struct datos d;

  d = *((struct datos *)dat);

  for (i = 0; i < d.div_hil; i++)
  {
    in = f(d.ini + i * d.tam_div);

    fi = f(d.ini + (i + 1) * d.tam_div);

    a += ((in + fi) * d.tam_div) / 2;
  }

  pthread_mutex_lock(&mut);
  
  res += a;
  
  pthread_mutex_unlock(&mut);
  
  pthread_exit(NULL);
}
