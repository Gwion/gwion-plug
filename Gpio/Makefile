include ../config.mk


CPPSRC += CppGPIO/src/*.cxx
CXXSRC += *.cc

GPIO_LIB := CppGpio/
ifeq ($(wilcard ${GPIO_LIB}), )
endif

all: GPIO.checklib ${NAME}.so

include ../config_post.mk

get-GPIO:
	git clone https://github.com/JoachimSchurig/CppGPIO/
