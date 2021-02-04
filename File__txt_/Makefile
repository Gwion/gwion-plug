
FILE_LIB := libfmsynth/libfmsynth.a
ifeq ($(wilcard ${FILE_LIB}), )
SRC += file/file/file.c
CFLAGS+=-Ifile/file
LDFLAGS += -L$(shell dirname ${FILE_LIB})
endif

include ../config.mk

all: file.checklib ${NAME}.so

include ../config_post.mk

get-file:
	git clone --depth=1 https://github.com/euppal/file
	cd file/file; make
