include ../config.mk
LDFLAGS+=-lsndfile

SNDFILE_LIB := libsndfile/libsndfile.a
ifeq ($(wilcard ${SNDFILE_LIB}), )
LDFLAGS += -L$(shell dirname ${SNDFILE_LIB})
CFLAGS += -Ilibsndfile/include
endif

all: soundpipe.checklib ${NAME}.so

include ../config_post.mk

get-soundpipe:
  git clone https://github.com/libsndfile/libsndfile
	cd libsndfile
	cmake -DBUILD_PROGRAMS=off -DBUILD_EXAMPLES=off -DBUILD_TESTING=off -DBUILD_REGTEST=off --build .
	make
