include ../config.mk

STK_LIB := stk/build/libstk.a
ifeq ($(wilcard ${STK_LIB}), )
LDFLAGS += -L$(shell dirname ${STK_LIB})
CFLAGS += -Istk/include
else
LDFLAGS += -lstk
CFLAGS += -I/usr/include/stk
CFLAGS += -I/usr/local/include/stk
endif

include ../config_post.mk

get-stk:
	git clone https://github.com/thestk/stk
	cd stk && export CXXFLAGS="" && export LDFLAGS="" && mkdir build && cd build && cmake .. && make
#	cd stk && export CXXFLAGS="" && export LDFLAGS="" && (which autoconf && autoconf || true ) && ./configure && make -C src
