USE_LD=1
include ../config.mk

NAME=WebSocket

CFLAGS += -Wall -Wextra
CFLAGS += `pkg-config libwebsockets --cflags`

LDFLAGS += `pkg-config libwebsockets --libs`

include ../config_post.mk
