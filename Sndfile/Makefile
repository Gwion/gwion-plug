include ../config.mk

SNDFILE_LIB := libsndfile/libsndfile.a
ifeq ($(wilcard ${SNDFILE_LIB}), )
LDFLAGS += -L$(shell dirname ${SNDFILE_LIB})
CFLAGS += -Ilibsndfile/include
endif

LDFLAGS += -lsndfile

all: sndfile.checklib ${NAME}.so

include ../config_post.mk

get-sndfile:
	git clone https://github.com/libsndfile/libsndfile
	cd libsndfile && cmake -DBUILD_PROGRAMS=off -DBUILD_EXAMPLES=off -DBUILD_TESTING=off -DBUILD_REGTEST=off .
	${MAKE} -C libsndfile
