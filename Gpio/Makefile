include ../config.mk

# here we use the C files directly,
# since the build system seems to fail in some cases
SRC +=  $(wildcard libgpiod/lib/*.c)

CFLAGS += -Ilibgpiod/include -DGPIOD_VERSION_STR=\"latest\" -D_GNU_SOURCE
#LDFLAGS += libgpiod/lib/.libs/libgpiod.a

all:
	[ -d libgpiod ] || git clone https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git/ && true
#	[ -f libgpiod/lib/.libs/libgpiod.a ] || (cd libgpiod && ./autogen.sh && ./configure --disable-shared --disable-bindings-cxx --disable-bindings-python --disable-tests --disable-tools && make)
	${MAKE} ${NAME}.so

include ../config_post.mk
