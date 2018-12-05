#USE_LD=1 # needed for e.g. sporth static lib
CPP_SRC := $(wildcard Fx/*.cpp)
include ../config.mk
NAME=Loup
CFLAGS+=-I/usr/include/opencv4/ -Iinclude -I/usr/include/opencv4/opencv2
LDFLAGS+=-lopencv
include ../config_post.mk
