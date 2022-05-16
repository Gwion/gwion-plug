include ../config.mk

NUKLEAR_LIB := Nuklear/Nucklear.h
CFLAGS+=-INuklear -INuklear/demo/x11/
LDFLAGS+=-lX11

include ../config_post.mk

all: nuklear.checklib
	${MAKE} ${NAME}.so

get-nuklear:
	git clone https://github.com/Immediate-Mode-UI/Nuklear
