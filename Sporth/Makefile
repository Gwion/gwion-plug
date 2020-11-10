include ../config.mk

LDFLAGS+=-lsporth

SOUNDPIPE_LIB := ../Soundpipe/gwion-soundpipe/libsoundpipe.a
ifeq ($(wilcard ${SOUNDPIPE_LIB}), )
LDFLAGS += -L$(shell dirname ${SOUNDPIPE_LIB})
CFLAGS += -I../Soundpipe/gwion-soundpipe/h
SPORTH_CFLAGS += -I$(shell realpath ../Soundpipe/gwion-soundpipe/h)
endif

SPORTH_LIB := gwion-sporth/libsporth.a
ifeq ($(wilcard ${SPORTH_LIB}), )
LDFLAGS += -L$(shell dirname ${SPORTH_LIB})
CFLAGS += -Igwion-sporth/h
endif

all: sporth.checklib ${NAME}.so

include ../config_post.mk

get-sporth:
	git clone --depth=1 https://github.com/Gwion/gwion-sporth
	cd gwion-sporth && CFLAGS=${SPORTH_CFLAGS} ${MAKE} libsporth.a
