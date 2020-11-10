include ../config.mk

LDFLAGS+=-lfmsynth

FMSYNTH_LIB := libfmsynth/libfmsynth.a
ifeq ($(wilcard ${FMSYNTH_LIB}), )
INC += -Ilibfmsynth/include
LDFLAGS += -L$(shell dirname ${FMSYNTH_LIB})
endif

all: fmsynth.checklib ${NAME}.so

include ../config_post.mk

get-fmsynth:
	git clone --depth=1 https://github.com/Themaister/libfmsynth
	cd libfmsynth; make
