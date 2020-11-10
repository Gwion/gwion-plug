include ../config.mk

SOUNDPIPE_LIB := ../Soundpipe/gwion-soundpipe/libsoundpipe.a
ifeq ($(wilcard ${SOUNDPIPE_LIB}), )
LDFLAGS += -L$(shell dirname ${SOUNDPIPE_LIB})
CFLAGS += -I../Soundpipe/gwion-soundpipe/h
endif

include ../config_post.mk

