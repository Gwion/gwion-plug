INC      = include
CC       = gcc
SRC      = $(wildcard *.c)
CPP_SRC  = $(wildcard *.cpp)
OBJ      = $(SRC:.c=.o)
OBJ     += $(CPP_SRC:.cpp=.o)
CFLAGS   = -I${INC}
CFLAGS  += -I..
CFLAGS  += -DSPFLOAT=double -std=c++11
LDFLAGS  = -shared -fPIC
PLUG_DIR = /usr/lib/Gwion/add
