// En una matriz de N x M enteros entre 1 y 10, contar numero de 3's
// Procesar por FILAS

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

main (int argc, char *argv[]) {
  struct timeval ts, tf;
  int i, j, N, M;
  int **a;
  int c = 0;

  if (argc != 3) {printf ("USO: %s <dimX> <dimY>\n", argv[0]); exit (1);}
  N = atoi(argv[1]);
  M = atoi(argv[2]);

  a = malloc (N * sizeof (int *));
  if (a == NULL) { printf ("Memoria\n"); exit (1);}

  for (i=0; i<N; i++) {
    a[i] = malloc (M * sizeof (int));
    if (a[i] == NULL) { printf ("Memoria\n"); exit (1);}
  }

  srandom (177845);
  for (i=0; i<N; i++)
    for (j=0; j<M; j++)
      a[i][j] = random() % 10;
  
  gettimeofday (&ts, NULL);
  for (i=0; i<N; i++)
    for (j=0; j<M; j++)
      if (a[i][j] == 3) 
        c++;
  gettimeofday (&tf, NULL);
  printf ("TRES: %d (%f secs)\n", c, ((tf.tv_sec - ts.tv_sec)*1000000u + 
           tf.tv_usec - ts.tv_usec)/ 1.e6);
}
