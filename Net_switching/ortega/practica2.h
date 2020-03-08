struct evaluacion
{
  char  id[16];
  char  apellido1[32];
  char  apellido2[32];
  char  nombre[32];
  float nota1p;
  float nota2p;
  float notamedia;
  char  photofilename[20];
  int   photosize;
  char  photodata[16000];
};

int ordenanotas(int fd);
