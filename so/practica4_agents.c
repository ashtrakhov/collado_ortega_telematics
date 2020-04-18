#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "practica4.h"

void dispatcher(int workpipe, int numwp, int ndiv)
{
  int i = 0;
  int r = 0;
  double desp = 0;
  struct datos paq;

  r = ndiv % numwp;

  paq.tam_div = 1 / (double)ndiv;

  for (i = 0; i < numwp; i++)
  {
    paq.div_paq = ndiv / numwp;

    if (r != 0)
    {
      paq.div_paq += 1;

      r--;
    }

    paq.ini = desp;

    desp += paq.tam_div * paq.div_paq;
    
    write(workpipe, &paq, sizeof(struct datos));
  }
}

void worker(int workpipe, int resultpipe)
{
  int i = 0;
  double in = 0;
  double fi = 0;
  double a = 0;
  struct datos p;

  printf("Worker");
  
  while (read(workpipe, &p, sizeof(struct datos)) > 0)
  {
    //printf(" Ini = %lf\n", p.ini);

    for (i = 0; i < p.div_paq; i++)
    {
      //printf(" Ini = %lf\n", (p.ini + i * p.tam_div));
      
      //printf(" Fin = %lf\n", (p.ini + (i + 1) * p.tam_div));
      
      in = f(p.ini + i * p.tam_div);

      fi = f(p.ini + (i + 1) * p.tam_div);

      a += ((in + fi) * p.tam_div) / 2;
    }
    
    write(resultpipe, &a, sizeof(double));
  }

  //printf(" Fin = %lf\n", fi);
}

void gatherer(int resultpipe)
{
  double sum = 0;
  double res = 0;

  while (read(resultpipe, &sum, sizeof(double)) > 0)
  {
    res += sum;
  }

  res *= 4;
  
  printf("Valor calculado: %.20lf\n", res);
}

double f(double x)
{
  if (x > 1)
  {
    x = 1;
  }

  return 1/(1 + x*x);
}
