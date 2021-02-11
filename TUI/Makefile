TUI_DIR=../../../tui/src

SRC += $(shell find ${TUI_DIR} -type f -name '*.c')
SRC += $(shell find ${TUI_DIR}/../xml -type f -name '*.c')
include ../config.mk

CFLAGS+=-I${TUI_DIR} -I${TUI_DIR}/../xml

include ../config_post.mk
