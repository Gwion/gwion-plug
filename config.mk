BASEDIR  ?= ../..
GWION    ?= ${BASEDIR}/gwion
INC      ?= ${BASEDIR}/include -I${BASEDIR}/util/include -I${BASEDIR}/ast/include
CC       ?= gcc
PLUG_DIR ?= $(shell ${GWION} -c 2>&1 | head -n 1 | cut -d '"' -f 2)
SRC      += $(wildcard *.c)
CPP_SRC  += $(wildcard *.cpp)
OBJ       = $(SRC:.c=.o)
OBJ      += $(CPP_SRC:.cpp=.o)
CFLAGS   += -I${INC}
CFLAGS   += -I.. -g
LDFLAGS   = -shared -fPIC
AUTO_INSTALL_DEPS ?= 0

NAME = $(shell basename `pwd`)

ifeq (${BUILD_ON_WINDOWS}, 1)
CFLAGS += -DBUILD_ON_WINDOWS=1 -D_XOPEN_SOURCE=700 -Wl,--export-all-symbols -static
LDFLAGS += -shared -lpsapi -fPIC -Wl,--export-all -Wl,--enable-auto-import
LDFLAGS += -L${BASEDIR} -lgwion
LDLAGS += ${BASEDIR}/libgwion.dll.a
#LDFLAGS += -L${BASEDIR}/ast -lgwion-ast
LDFLAGS += -L${BASEDIR}/ast
#LDLAGS += ${BASEDIR}/libgwion-ast.dll.a
#LDFLAGS += -L${BASEDIR}/util -lgwion-util
LDFLAGS += -L${BASEDIR}/util
LDLAGS += ${BASEDIR}/libgwion-util.dll.a
LDLAGS += ../../libgwion.a
LDLAGS += ../../util/libgwion-ast.a
LDLAGS += ../../util/libgwion-util.a
endif

ifeq ($(shell uname), Darwin)
LDFLAGS += -undefined dynamic_lookup
endif

ifeq (${USE_DOUBLE}, 1)
CFLAGS +=-DUSE_DOUBLE
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
