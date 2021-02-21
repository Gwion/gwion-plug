include ../config.mk
#CFLAGS+=-I<more include
LDFLAGS+=-lcmojify

CMOJIFY_LIB := cmojify/libcmojify.a
ifeq ($(wilcard ${CMOJIFY_LIB}), )
LDFLAGS += -L$(shell dirname ${CMOJIFY_LIB})
CFLAGS += -Icmojify
endif

all: cmojify.checklib ${NAME}.so

include ../config_post.mk

get-cmojify:
	git clone https://github.com/fennecdjay/cmojify
	cd cmojify && make libcmojify.a
