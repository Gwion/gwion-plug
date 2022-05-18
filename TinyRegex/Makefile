include ../config.mk

CFLAGS+=-Itiny-regex-c
SRC += tiny-regex-c/re.c

all: tiny-regex-c ${NAME}.so

include ../config_post.mk

tiny-regex-c:
	git clone https://github.com/kokke/tiny-regex-c
