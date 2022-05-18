include ../config.mk
CFLAGS += -Itinysr
SRC += tinysr/tinysr.c

all: tinysr.checklib ${NAME}.so

include ../config_post.mk

get-tinysr:
	git clone https://github.com/petersn/tinysr
	${MAKE} -C tinysr
