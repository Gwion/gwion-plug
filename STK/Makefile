include ../config.mk

STK_LIB := stk/src/libstk.a
ifeq ($(wilcard ${STK_LIB}), )
LDFLAGS += -L$(shell dirname ${STK_LIB})
CFLAGS += -Istk/include
else
CFLAGS += -I/usr/include/stk
endif

LDFLAGS += -lstk

all: stk.checklib ${NAME}.so

include ../config_post.mk

get-stk:
	git clone https://github.com/thestk/stk
	cd stk && export CXXFLAGS="" && export LDFLAGS="" && (which autoconf && autoconf || true ) && ./configure && make -C src
