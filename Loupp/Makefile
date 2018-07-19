#USE_LD=1 # needed for e.g. sporth static lib
CPP_SRC := $(wildcard Fx/*.cpp)
include ../config.mk
NAME=Loup
CFLAGS+=-I/usr/include/opencv -Iinclude
LDFLAGS+=`pkg-config --cflags --libs opencv`
include ../config_post.mk
