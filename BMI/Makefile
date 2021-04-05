include ../config.mk


LDFLAGS += -lbmi

BMI_LIB := bmi/libbmi.a
ifeq ($(wilcard ${BMI_LIB}), )
LDFLAGS += -L$(shell dirname ${BMI_LIB})
CFLAGS += -Ibmi/include
endif

all: bmi.checklib ${NAME}.so

include ../config_post.mk

get-bmi:
	git clone https://github.com/euppal/bmi
	cd bmi && make static
