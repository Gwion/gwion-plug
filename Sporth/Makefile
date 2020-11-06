include ../config.mk

LDFLAGS+=-lsporth

SPORTH_LIB := gwion-sporth/libsporth.a
ifeq ($(wilcard ${SPORTH_LIB}), )
LDFLAGS += -L$(shell dirname ${SPORTH_LIB})
CFLAGS += -Igwion-sporth/h
endif

all: soundpipe.checklib sporth.checklib ${NAME}.so

soundpipe.checklib:
	${MAKE} -C ../Soundpipe soundpipe.checklib

include ../config_post.mk

get-sporth:
	git clone --depth=1 https://github.com/Gwion/gwion-sporth
	cd gwion-sporth && ${MAKE} libsporth.a
