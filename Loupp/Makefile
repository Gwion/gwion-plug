#USE_LD=1 # needed for e.g. sporth static lib
CPP_SRC := $(wildcard Fx/*.cpp)
include ../config.mk
NAME=Loup
CFLAGS+=-Iinclude `pkg-config --cflags opencv4`
LDFLAGS+=`pkg-config --libs opencv4`
include ../config_post.mk
