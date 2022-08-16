SRCS=tres_fil.c tres_col.c tres_threads.c tres_mutex.c tres_local.c \
     tres_priv.c tres_cache_line.c 
OBJS=tres_fil.o tres_col.o tres_threads.o tres_mutex.o tres_local.o \
     tres_priv.o tres_cache_line.o 
PROGS=tres_fil tres_col tres_threads tres_mutex tres_local \
     tres_priv tres_cache_line 

all:	${PROGS}

tres_fil:	tres_fil.c
	cc -o tres_fil tres_fil.c

tres_col:	tres_col.c
	cc -o tres_col tres_col.c

tres_threads:	tres_threads.c
	cc -o tres_threads tres_threads.c -lpthread -lm

tres_mutex:	tres_mutex.c
	cc -o tres_mutex tres_mutex.c -lpthread -lm

tres_priv:	tres_priv.c
	cc -o tres_priv tres_priv.c -lpthread -lm

tres_local:	tres_local.c
	cc -o tres_local tres_local.c -lpthread -lm

tres_cache_line:	tres_cache_line.c
	cc -o tres_cache_line tres_cache_line.c -lpthread -lm

clean:
	rm -f ${PROGS}
