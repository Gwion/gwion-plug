DIR=$(wildcard */)

all:
	 +@ $(foreach dir,$(DIR), make -s            -C $(dir);)

install:
	 +@ $(foreach dir, $(DIR), make -s install   -C $(dir);)

clean:
	 +@ $(foreach dir, $(DIR), make -s clean     -C $(dir);)

uninstall:
	 +@ $(foreach dir, $(DIR), make -s uninstall -C $(dir);)
