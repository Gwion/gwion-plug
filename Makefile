DIR=$(wildcard */)

list:
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
