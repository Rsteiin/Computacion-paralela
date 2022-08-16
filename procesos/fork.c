// Uso de fork()
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int sum;
int pid;

int main (int argc, char *argv[]) {
  int i;
  int n;
  if (argc!=2) {printf ("USO: %s <n>\n", argv[0]); exit(1);}
  sum = 0;
  n = atoi(argv[1]);
  for (i=1;i<=4;i++) {  // crea 4 procesos hijos
    pid = fork();
    if (pid==0) break;
  }
  printf ("ID del Padre: %d  ID del Proceso: %d\n", getppid(), getpid());
  for (i=1; i<=n; i++) {
    printf ("Proceso (%d) - i: %d\n", getpid(), i);
//    fflush (stdout);
    sum += i;
  }
  printf ("Proceso (%d) - TOTAL: %d\n", getpid(), sum);
}
