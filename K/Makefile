#USE_LD=1 # needed for e.g. sporth static lib
include ../config.mk
NAME=K
CFLAGS+=-g
LDFLAGS+=-g
include ../config_post.mk

k.o:
	${CC} -g -fPIC -c k.c -o k.o

test: k.o
	${CC} -g ${CFLAGS} -lm -fPIC test.c -o test k.o
