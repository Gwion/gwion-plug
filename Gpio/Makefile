include ../config.mk

CFLAGS += -Ic-periphery/src
SRC += c-periphery/src/gpio.c

all:
	[ -d c-periphery ] || git clone https://github.com/vsergeev/c-periphery/ || true
	${MAKE} ${NAME}.so

include ../config_post.mk
