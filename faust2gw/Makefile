CC?=cc

CFLAGS=-std=c99

CFLAGS += -I/media/DOC3/Gwion/Gwion/include
CFLAGS += -I/media/DOC3/Gwion/Gwion/ast/include/
CFLAGS += -I/media/DOC3/Gwion/Gwion/util/include/
CFLAGS += -I/media/DOC3/Gwion/Gwion/util/libtermcolor/include

faust2gw: faust2gw.o

faust2gw.o: gwion_faust.template.h

gwion_faust.template.h: gwion_faust.template.cpp
	sed  -e 's/\\/\\\\/g' -e 's/"/\\"/g' -e 's/\(.*\)/"\1",/' $< >$@

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf faust2gw *.o

install: faust2ck
	cp -f faust2ck /usr/local/bin/
