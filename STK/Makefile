CC := ${CXX}

include ../config.mk

STK_LIB := stk/src/libstk.a
ifeq ($(wilcard ${STK_LIB}), )
LDFLAGS += -L$(shell dirname ${STK_LIB})
CFLAGS += -Istk/include
endif

CFLAGS += -I/usr/include/stk
LDFLAGS+=-lstk

all: t stk.checklib ${NAME}.so
t:
	echo ${CXX}
	echo ${CC}
include ../config_post.mk

get-stk:
	git clone https://github.com/thestk/stk
	cd stk && export CXXFLAGS="" && export LDFLAGS="" && (which autoconf && autoconf || true ) && ./configure && make -C src
