include ../config.mk
SOUNDPIPE_LIB := ../Soundpipe/gwion-soundpipe/libsoundpipe.a
$(warning hoho ${SOUNDPIPE_LIB})
ifneq ($(wildcard ${SOUNDPIPE_LIB}), )
$(warning hohoho ${SOUNDPIPE_LIB})
LDFLAGS += -L$(shell dirname ${SOUNDPIPE_LIB})
CFLAGS += -I../Soundpipe/gwion-soundpipe/h
endif
include ../config_post.mk
