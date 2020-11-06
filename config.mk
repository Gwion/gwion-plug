BASEDIR  ?= ../..
GWION    ?= ${BASEDIR}/gwion
INC      ?= ${BASEDIR}/include -I${BASEDIR}/util/include -I${BASEDIR}/ast/include
CC       ?= gcc
PLUG_DIR ?= $(shell ${GWION} -c 2>&1 | head -n 1 | cut -d '"' -f 2)
SRC      += $(wildcard *.c)
CPP_SRC  += $(wildcard *.cpp)
OBJ       = $(SRC:.c=.o)
OBJ      += $(CPP_SRC:.cpp=.o)
CFLAGS    = -I${INC}
CFLAGS   += -I.. -g
LDFLAGS   = -shared -fPIC
AUTO_INSTALL_DEPS ?= 0

NAME = $(shell basename `pwd`)

# outdated
ifeq ( $(shell (${GWION} -k 2>&1 | grep double)), 1)
CFLAGS   += -DSPFLOAT=double
else
CFLAGS   += -DSPFLOAT=float
endif

%.checklib:
	@echo "int main(){}" > tmp.c
ifeq (${AUTO_INSTALL_DEPS}, 1)
	@${CC} ${LDFLAGS} tmp.c 2>/dev/null || (rm tmp.c; ${MAKE} -s get-$*)
else
	@${CC} ${LDFLAGS} tmp.c 2>/dev/null || (rm tmp.c; ${MAKE} -s install-$*)
endif
	@rm -f tmp.c a.out

install-%:
	@$(info $* is not installed)
	@$(info you can install it with: make get-$*)
	@$(info or use your package manager to install it)
	@false
