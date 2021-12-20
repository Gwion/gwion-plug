include ../config.mk
NAME=Sciter

SCITER_DIR := ${PWD}/../../../sciter/

#!CFLAGS+=${PWD}/./../../sciter/bin.lite/linux/x64/libsciter.so
CFLAGS+=-I${PWD}/../../../sciter/include
CFLAGS+=-I${PWD}/../../../sciter/demos.lite/sciter-glfw-opengl
CFLAGS+=-I${PWD}/../../../sciter/demos.lite/glfw/include
CFLAGS+=-I${PWD}/../../../sciter/demos.lite/glfw/deps/
CFLAGS+=`pkg-config gtk+-3.0 --cflags`
CFLAGS += -DWINDOWLESS
CFLAGS += -D_GLFW_X11

SRC += ${SCITER_DIR}/demos.lite/glfw/src/context.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/init.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/input.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/monitor.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/vulkan.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/window.c
SRC += ${SCITER_DIR}/demos.lite/glfw/deps/glad.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/x11_init.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/x11_window.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/linux_joystick.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/posix_time.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/x11_monitor.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/posix_thread.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/glx_context.c
SRC += ${SCITER_DIR}/demos.lite/glfw/src/egl_context.c

LDFLAGS+= -lX11 -lXrandr -lXinerama -lXcursor -lGL -lGLU

include ../config_post.mk
