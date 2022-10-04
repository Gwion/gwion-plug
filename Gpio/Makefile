include ../config.mk

CFLAGS += -Ilibgpiod/include
LDFLAGS += libgpiod/lib/.libs/libgpiod.a
all:
	[ -d libgpiod ] || git clone https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/ && true
	[ -f libgpiod/lib/.libs/libgpiod.a ] || (cd libgpiod && ./autogen.sh && ./configure --disable-shared --disable-bindings-cxx --disable-bindings-python --disable-tests --disable-tools && make)
	${MAKE} ${NAME}.so

include ../config_post.mk
