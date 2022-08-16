// En una matriz de N x M enteros entre 1 y 10, contar numero de 3's
// Usa threads con mutex

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct args {
  int thID;
  int min;
  int max;
} arg;

int **a;
int c = 0;
int N, M, T;
pthread_mutex_t mx;

void tres (arg *args) {
  int i, j;
  
  for (i=args->min; i<args->max; i++) {
    for (j=0; j<M; j++) {
      if (a[i][j] == 3) {
        pthread_mutex_lock(&mx);
        c++;
        pthread_mutex_unlock(&mx);
      }
    }
  }
}

main (int argc, char *argv[]) {
  struct timeval ts, tf;
  int i, j;
  int cl, fr;
  pthread_t *th;
  arg *args;

  if (argc != 4) {printf ("USO: %s <dimX> <dimY> <Ths>\n", argv[0]); exit (1);}

  N = atoi(argv[1]);
  M = atoi(argv[2]);
  T = atoi(argv[3]);

  th = malloc (T * sizeof (pthread_t)); 
  a = malloc (N * sizeof (int *));
  args = malloc (T * sizeof (arg));
  if (a==NULL || th==NULL || args==NULL) { printf ("Memoria\n"); exit (1);}
 
  for (i=0; i<N; i++) {
    a[i] = malloc (M * sizeof (int));
    if (a[i] == NULL) { printf ("Memoria\n"); exit (1);}
  }

  srandom (177845);
  for (i=0; i<N; i++)
    for (j=0; j<M; j++)
      a[i][j] = random() % 10;
  (void) pthread_mutex_init(&mx, 0);

  cl = (int)ceil((float)N/(float)T);
  fr = (int)floor((float)N/(float)T);

  for (i=0; i<N%T; i++) {
    args[i].thID = i;
    args[i].min = i*cl;
    args[i].max = (i+1)*cl;
  }

  for (i=N%T; i<T; i++) {
    args[i].thID = i;
    args[i].min = (N%T)*cl + (i-(N%T))*fr;
    args[i].max = (N%T)*cl + (i-(N%T)+1)*fr;
  }

  gettimeofday (&ts, NULL);
  for (i=0; i<T; i++)
    pthread_create (&th[i], NULL, (void *) tres, (void *)&args[i]);
  
  for (i=0; i<T; i++)
    if (pthread_join (th[i], NULL)) {printf ("PJOIN (%d)\n", i); exit (1);};
  gettimeofday (&tf, NULL);

  printf ("TRES: %d (%f secs)\n", c, ((tf.tv_sec - ts.tv_sec)*1000000u +
          tf.tv_usec - ts.tv_usec)/ 1.e6);
  exit (0);
}

