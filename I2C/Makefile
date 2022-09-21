include ../config.mk

I2C_LIB := libi2c/libi2c.a
SRC += libi2c/libi2c.a
#CFLAGS+=-I<more include>
#LDFLAGS+=-l<more LIBS>

all: i2c.checklib libi2c/libi2c.a
	[ -d libi2c ] || git clone https://github.com/amaork/libi2c
	[ -d libi2c/libi2c.a ] || ${MAKE} make-i2c
	${MAKE} ${NAME}.so

include ../config_post.mk


get-i2c:
	git clone https://github.com/amaork/libi2c
	${MAKE} make-i2c

make-i2c:
	${MAKE} -C libi2c libi2c.a

libi2c/libi2c.a:
	${MAKE} -C libi2c libi2c.a
