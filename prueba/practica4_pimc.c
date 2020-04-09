#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>

void manejador(int n, siginfo_t *info, void *context);

int main(int argc, char* argv[])
{
  int i = 0;
  int num_pun = 0;
  int temp = 0;
  int c = 0;
  double x = 0;
  double y = 0;
  double dis = 0;
  double res = 0;
  struct sigaction s;

  if (argc != 3)
  {
    perror("Error en los parámetros de entrada\n");

    exit(-1);
  }

  num_pun = atoi(argv[1]);
  
  temp = atoi(argv[2]);

  if (num_pun < 1 || temp < 1)
  {
    perror("Error en los parámetros de entrada\n");

    exit(-1);
  }

  s.sa_sigaction = manejador;

  sigemptyset(&s.sa_mask);

  s.sa_flags = SA_SIGINFO;

  sigaction(SIGINT, &s, NULL);

  sigaction(SIGALRM, &s, NULL);
  
  sigaction(SIGUSR1, &s, NULL);

  alarm(temp);

  printf("PID: %d\n", getpid());

  srand48(time(NULL));

  for (i = 0; i < num_pun; i++)
  {
    x = drand48();

    y = drand48();

    dis = sqrt((x - 0.5)*(x - 0.5) + (y - 0.5)*(y - 0.5));

    if (dis <= 0.5)
    {
      c++;
    }
  }

  res = c / (double)num_pun;

  res *= 4;

  printf("Valor calculado: %.20lf\n", res);

  return 0;
void manejador(int n, siginfo_t *info, void *context);
}

void manejador(int n, siginfo_t *info, void *context)
{
  if (n == SIGINT)
  {
    printf("Prueba Ctrl+C\n");
    
    //signal(SIGINT, SIG_DFL);

    exit(0);
  }

  if (n == SIGALRM)
  {
    printf("Prueba alarma\n");
    
    //signal(SIGALRM, SIG_DFL);
    
    exit(0);
  }

  if (n == SIGUSR1)
  {
    printf("Prueba externa\n");
    
    //signal(SIGUSR1, SIG_DFL);
    
    exit(0);
  }
}
