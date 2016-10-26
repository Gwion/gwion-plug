include ../config.mk
NAME=sporth
LDFLAGS+=-lsporth_dyn
#LDFLAGS+=-lsporth
include ../config_post.mk

# all: ${PLUG}
#
#	${PLUG}: ${C_OBJECTS} $(CXX_OBJECTS)
#	=> ${PLUG}: ${OBJ}
# ar rv 

sporth.so: sporth.o
	$(LD) -o $@ $(LDFLAGS) ${OBJ}
