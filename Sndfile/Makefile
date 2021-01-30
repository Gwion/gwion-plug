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
	cd libsndfile && LDFLAGS="" CFLAGS="" cmake -G "Unix Makefiles" -DBUILD_PROGRAMS=off -DBUILD_EXAMPLES=off -DBUILD_TESTING=off -DBUILD_REGTEST=off -DCMAKE_C_FLAGS=-fPIC -DENABLE_STATIC_RUNTIME=on . && cmake --build .
	echo "\033[31mlist:\033[0m"; ls libsndfile
