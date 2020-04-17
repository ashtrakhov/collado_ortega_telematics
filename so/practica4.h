void dispatcher(int workpipe, int numwp, int ndiv);

void worker(int workpipe, int resultpipe);

void gatherer(int resultpipe);

double f(double x);

struct datos
{
  double ini;
  int div_paq;
  double tam_div;
};
