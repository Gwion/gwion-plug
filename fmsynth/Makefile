include ../config.mk

NAME=fmsynth

all: fmsynth.checklib ${NAME}.so
include ../config_post.mk
LDFLAGS+=-lfmsynth
FMSYNTH_LIB := libfmsynth/libfmsynth.a
ifeq ($(wilcard ${FMSYNTH_LIB}), )
LDFLAGS += -L$(shell dirname ${FMSYNTH_LIB})
endif

get-fmsynth:
	git clone --depth=1 https://github.com/Themaister/libfmsynth
	cd libfmsynth; make
