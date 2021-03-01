include ../config.mk

CFLAGS+=-ITinySoundFont

all: TinySoundFont/tsf.h ${NAME}.so

include ../config_post.mk

TinySoundFont/tsf.h:
	git clone https://github.com/schellingb/TinySoundFont
