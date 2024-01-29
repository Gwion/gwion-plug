include ../config.mk

I2C_LIB := libi2c/libi2c.a
SRC += libi2c/src/i2c.c
CFLAGS += -Ilibi2c/include/
CFLAGS += -D"perror(a)="

all: i2c.checklib
	[ -d libi2c ] || git clone https://github.com/amaork/libi2c
	${MAKE} ${NAME}.so

include ../config_post.mk


get-i2c:
	git clone https://github.com/amaork/libi2c
