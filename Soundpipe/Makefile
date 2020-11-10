include ../config.mk

SOUNDPIPE_LIB := gwion-soundpipe/libsoundpipe.a
ifeq ($(wilcard ${SOUNDPIPE_LIB}), )
LDFLAGS += -L$(shell dirname ${SOUNDPIPE_LIB})
CFLAGS += -Igwion-soundpipe/h
endif

LDFLAGS+=-lsoundpipe

all: soundpipe.checklib ${NAME}.so

include ../config_post.mk

get-soundpipe:
	git clone https://github.com/Gwion/gwion-soundpipe
	cd gwion-soundpipe && ${MAKE}
