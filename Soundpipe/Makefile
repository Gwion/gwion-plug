include ../config.mk

SOUNDPIPE_LIB := gwion-soundpipe/libsoundpipe.a
ifeq ($(wilcard ${SOUNDPIPE_LIB}), )
LDFLAGS += -L$(shell dirname ${SOUNDPIPE_LIB})
CFLAGS += -Igwion-soundpipe/h
endif

SNDFILE_LIB := ../Sndfile/libsndfile/libsndfile.a
ifeq ($(wilcard ${SNDFILE_LIB}), )
CFLAGS += -I../Sndfile/libsndfile/include
SNDFILE_CFLAGS += -I../../Sndfile/libsndfile/include
endif


ifeq ($(shell uname), Darwin)
# litteraly take all object in soundpipe lol
CFLAGS += $(shell bash ./mac-helper.sh)
OBJ += $(shell find gwion-soundpipe -name "*.o" | sed 's/\s2.o//g')
else
LDFLAGS += -lsoundpipe
endif

all: soundpipe.checklib
	${MAKE} ${NAME}.so

ifeq (${USE_DOUBLE}, 1)
SNDFILE_CFLAGS += -DUSE_DOUBLE
#SNDFILE_CFLAGS += -DSPFLOAT="double"
#else
#SNDFILE_CFLAGS += -DSPFLOAT="float"
endif
include ../config_post.mk

get-soundpipe:
	git clone https://github.com/Gwion/gwion-soundpipe
	${MAKE} make-soundpipe

make-soundpipe:
	if [ $USE_DOUBLE = 1 ]; then echo "USE_DOUBLE = 1" >> gwion-soundpipe/config.def.mk; fi
	export LDFLAGS=""; export USE_DOUBLE=${USE_DOUBLE} && export CFLAGS="${SNDFILE_CFLAGS}" && ${MAKE} USE_DOUBLE=${USE_DOUBLE} -C gwion-soundpipe libsoundpipe.a
