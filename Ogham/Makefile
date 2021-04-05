include ../config.mk


LDFLAGS += -logham

OGHAM_LIB := ogham/libogham.a
ifeq ($(wilcard ${OGHAM_LIB}), )
LDFLAGS += -L$(shell dirname ${OGHAM_LIB})
CFLAGS += -Iogham/src
endif

all: ogham.checklib ${NAME}.so

include ../config_post.mk

get-ogham:
	git clone https://github.com/euppal/ogham
	make -C ogham static
