include ../config.mk

NUKLEAR_LIB := Nuklear/Nuklear.h
CFLAGS += -INuklear -INuklear/demo/x11/
LDFLAGS += -lX11

all: nuklear.checklib
	${MAKE} ${NAME}.so

include ../config_post.mk

get-nuklear:
	git clone https://github.com/Immediate-Mode-UI/Nuklear
