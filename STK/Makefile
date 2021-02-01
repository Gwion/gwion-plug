include ../config.mk

STK_LIB := stk/src/libstk.a
ifeq ($(wilcard ${STK_LIB}), )
LDFLAGS += -L$(shell dirname ${STK_LIB})
CFLAGS += -Istk/include
else
CFLAGS+=-I/usr/include/stk
endif

NAME=STK
LDFLAGS+=-lstk

include ../config_post.mk

get-sndfile:
  git clone https://github.com/libsndfile/libsndfile
  cd libsndfile && autoconf && ./configure && make
