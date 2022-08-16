/*
 * Este programa codifica y serializa un arbol binario y lo envia por una
 * conexion TCP/IP. Actua como cliente.
 * xdr_pointer procesa el arbol recursivamente: escribe TRUE si el siguiente 
 * pointer no es NULL y llama xdr_reference para codificar el nodo usando
 * xdr_wl. Cuando el siguiente pointer es NULL, escribe FALSE y retorna
 * FALSE a xdr_wp
 */

#include <string.h>
#include <sys/types.h>
#include <rpc/xdr.h>

// Estructura de datos en cada nodo del arbol
typedef struct n {
  struct n *izq;
  int    i;
  struct n *der;
} nodo;

bool_t  xdr_nodo();
void  recorrer ();
int     errexit(const char *format, ...);
int     connectTCP(const char *host, const char *service);

int
main (int argc, char *argv[]) {
  XDR xdr, *xdrp;
  FILE *fp;
  int sd;
  nodo *ap, *p;
  char host[20], service[10];

  if (argc != 3) {
    errexit ("USO: tcp-encode <hostIP> <puertoTCP>\n");
  }

  strcpy (host, argv[1]);
  strcpy (service, argv[2]);

  sd = connectTCP(host, service);
  if (sd < 0) errexit ("connectTCP");
  fp = fdopen (sd, "w");
  if (fp == NULL) printf ("fdopen\n");

// Construccion "manual" del arbol

  ap = (nodo *)malloc (sizeof (nodo));
  ap->i = 1;

  p = (nodo *)malloc (sizeof (nodo));
  p->i = 2;
  ap->izq = p;

  p = (nodo *)malloc (sizeof (nodo));
  p->i = 5;
  ap->der = p;
  p->izq = NULL;

  p = (nodo *)malloc (sizeof (nodo));
  p->i = 3;
  ap->izq->izq = p;
  p->izq = NULL;
  p->der = NULL;

  p = (nodo *)malloc (sizeof (nodo));
  p->i = 4;
  ap->izq->der = p;
  p->izq = NULL;
  p->der = NULL;

  p = (nodo *)malloc (sizeof (nodo));
  p->i = 6;
  ap->der->der = p;
  p->izq = NULL;
  p->der = NULL;

// Funcion que recorre el arbol recursivamente (subarbol izquierdo, raiz, 
// subarbol derecho) imprimiendo el campo int de cada nodo.
  recorrer (ap);
  
  xdrp = &xdr;

// Relaciona xdr con la conexion tcp/ip
  xdrstdio_create (xdrp, fp, XDR_ENCODE);

// Filtro xdr que recorre el arbol recursivamente, codificando los nodos y
// serializandolos
  xdr_nodo (xdrp, ap);
sleep (5);
  if (fclose (fp) != 0) perror ("FCLOSE");
  xdr_destroy (xdrp);
  exit (0);
}

bool_t
xdr_nodo (XDR *xdrp, nodo *p) {
  if (!xdr_pointer (xdrp, (char **)&(p->izq), sizeof(nodo),(xdrproc_t)xdr_nodo))
    return (FALSE);
  if (xdr_int (xdrp, &p->i) == FALSE) return(FALSE);
  if (!xdr_pointer (xdrp,(char **)&(p->der), sizeof(nodo),(xdrproc_t)xdr_nodo))
    return (FALSE);
  return (TRUE);
}

void
recorrer (nodo *p) {
  if (p != NULL) {
    recorrer (p->izq);
    printf ("%d\n", p->i);
    recorrer (p->der);
  }
}


/*
 * Este programa recibe un arbol serializado y codificado por medio de una
 * conexion TCP/IP, recrea el arbol (lo "deserializa") y decodifica los datos 
 * de los nodos. xdr_wp hace esta tarea automaticamente. 
 * Este programa actua como servidor TCP/IP y es la contraparte de tcp-encode
 */

#include <sys/types.h>
#include <rpc/xdr.h>

typedef struct n {
  struct n *iz;
  int    i;
  struct n *der;
} nodo;

bool_t  xdr_nodo();
void  recorrer ();
int     errexit(const char *format, ...);
int     passiveTCP(unsigned short port, int qlen);

int
main (int argc, char *argv[]) {
  XDR xdr, *xdrp;
  FILE *fp;
  int sd, ns;
  int qlen = 5;
  int len;
  struct sockaddr sa;
  nodo *ap, *p;
  int pt;

  if (argc != 2) {
    errexit ("USO: tcp-decode <puertoTCP>\n");
  }

  pt = atoi (argv[1]);
  sd = passiveTCP (pt, qlen);
  if (sd < 0) errexit ("passiveTCP\n");
  ns = accept (sd, &sa, &len);
  fp = fdopen (ns, "r");
  ap = (nodo *)malloc (sizeof (nodo));

  xdrp = &xdr; 
  xdrstdio_create (xdrp, fp, XDR_DECODE);
  //printf ("durmiendo 5 secs...\n");
  xdr_nodo (xdrp, ap);
  printf ("%d\n", ap->i);
  recorrer (ap);
  if (fclose (fp) != 0) perror ("FCLOSE");
  xdr_destroy (xdrp);
  exit (0);
}

bool_t
xdr_nodo (XDR *xdrp, nodo *p) {
  if (!xdr_pointer (xdrp, (char **)&(p->iz), sizeof(nodo),(xdrproc_t)xdr_nodo))
    return (FALSE);
  if (xdr_int (xdrp, &p->i) == FALSE) return(FALSE);
  if (!xdr_pointer (xdrp,(char **)&(p->der), sizeof(nodo),(xdrproc_t)xdr_nodo))
    return (FALSE);
  return (TRUE);
}

void
recorrer (nodo *p) {
  if (p != NULL) {
    recorrer (p->iz);
    printf ("%d\n", p->i);
    recorrer (p->der);
  }
}

*
 * Este programa recibe un arbol serializado y codificado por medio de una
 * conexion TCP/IP, recrea el arbol (lo "deserializa") y decodifica los datos 
 * de los nodos. xdr_wp hace esta tarea automaticamente. 
 * Este programa actua como servidor TCP/IP y es la contraparte de tcp-encode
 */

#include <sys/types.h>
#include <rpc/xdr.h>

typedef struct n {
  struct n *iz;
  int    i;
  struct n *der;
} nodo;

bool_t  xdr_nodo();
void  recorrer ();
int     errexit(const char *format, ...);
int     passiveTCP(unsigned short port, int qlen);

int
main (int argc, char *argv[]) {
  XDR xdr, *xdrp;
  FILE *fp;
  int sd, ns;
  int qlen = 5;
  int len;
  struct sockaddr sa;
  nodo *ap, *p;
  int pt;

  if (argc != 2) {
    errexit ("USO: tcp-decode <puertoTCP>\n");
  }

  pt = atoi (argv[1]);
  sd = passiveTCP (pt, qlen);
  if (sd < 0) errexit ("passiveTCP\n");
  ns = accept (sd, &sa, &len);
  fp = fdopen (ns, "r");
  ap = (nodo *)malloc (sizeof (nodo));

  xdrp = &xdr; 
  xdrstdio_create (xdrp, fp, XDR_DECODE);
  //printf ("durmiendo 5 secs...\n");
  xdr_nodo (xdrp, ap);
  printf ("%d\n", ap->i);
  recorrer (ap);
  if (fclose (fp) != 0) perror ("FCLOSE");
  xdr_destroy (xdrp);
  exit (0);
}

bool_t
xdr_nodo (XDR *xdrp, nodo *p) {
  if (!xdr_pointer (xdrp, (char **)&(p->iz), sizeof(nodo),(xdrproc_t)xdr_nodo))
    return (FALSE);
  if (xdr_int (xdrp, &p->i) == FALSE) return(FALSE);
  if (!xdr_pointer (xdrp,(char **)&(p->der), sizeof(nodo),(xdrproc_t)xdr_nodo))
    return (FALSE);
  return (TRUE);
}

void
recorrer (nodo *p) {
  if (p != NULL) {
    recorrer (p->iz);
    printf ("%d\n", p->i);
    recorrer (p->der);
  }
}

*
 * manejo de listas encadenadas con XDR
 * DECODIFICA y DESERIALIZA desde archivo portable.ptr
 * ver archivo portable.ptr con "od -b portable.ptr"
 * El contenido es: TRUE TRUE FALSE 3 3 3.0 2 2 2.0 1 1 1.0
 * xdr_wl "regenera" lista encadenada
 */

#include <errno.h>
#include <sys/types.h>
#include <rpc/xdr.h>

typedef struct w {
  int    i;
  char   c;
  float  f;
  struct w *np;
} wl;

bool_t xdr_wl();

int
main (int argc, char *argv[]) {
  XDR xdr, *xdrp;
  FILE *fp;
  wl *wlp, *wp;

  if (argc != 2) {
    printf ("USO: xdrio <fname>\n");
    exit (1);
  }
  if ((fp = fopen(argv[1], "r")) == NULL) {
    printf ("FOPEN\n");
    exit (1);
  }

  xdrp = &xdr;
  xdrstdio_create (xdrp, fp, XDR_DECODE);
  wlp = malloc (sizeof (wl));
  xdr_wl (xdrp, wlp);
  if (fclose (fp) != 0) perror ("FCLOSE");

  wp = wlp;
  while (wp != NULL) {
    printf ("%d, %x, %f\n", wp->i, wp->c, wp->f);
    wp = wp->np;
  }

  exit (0);
}

bool_t
xdr_wl (XDR *xdrp, wl *wp) {
  if (xdr_int (xdrp, &wp->i) == FALSE) return(FALSE);
  if (xdr_char (xdrp, &wp->c) == FALSE) return(FALSE);
  if (xdr_float (xdrp, &wp->f) == FALSE) return (FALSE);
  if (!xdr_pointer (xdrp, (char **)&(wp->np), sizeof (wl), (xdrproc_t)xdr_wl))
    return (FALSE);
  return (TRUE);
}


