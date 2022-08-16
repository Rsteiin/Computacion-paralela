// sumSec.c - programa secuencial, procesos suman enteros del 1 al n
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int sum;

int main (argc, argv)
int  argc;
char *argv[];
{
  int i;
  int n;	//parametro de entrada
  int pid;	//process ID

  if (argc != 2) {
    printf ("USO: sum <valor de n>\n");
    exit (1);
  }
  n = atoi (argv[1]);
  sum = 0;
  pid = getpid();
  for (i=1; i<=n; i++) {
    printf ("%5d: Valor de i: %d\n", pid, i);
    fflush (stdout);
    sum += i;
  }
  printf ("Suma: %d\n", sum);
  exit (0);
}
