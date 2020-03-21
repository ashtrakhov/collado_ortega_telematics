#include <math.h>
#include "practica3.h"

double f(double x)
{
  if (x > 1)
    return 0;
  return sqrt(1 - x*x);
}
