ifeq (,$(wildcard list.txt.orig))
$(shell cp list.txt.orig list.txt)
endif

DIR=$(wildcard */)

_list:
	+@ $(foreach dir,$(shell cat list.txt), ${MAKE} -s            -C $(dir);)

all:
	 +@ $(foreach dir,$(DIR), ${MAKE} -s            -C $(dir);)
	exit 0

install:
	 +@ $(foreach dir, $(DIR), ${MAKE} -s install   -C $(dir);)

clean:
	 +@ $(foreach dir, $(DIR), ${MAKE} -s clean     -C $(dir);)

uninstall:
	 +@ $(foreach dir, $(DIR), ${MAKE} -s uninstall -C $(dir);)
