ifeq (,$(wildcard list.txt))
$(shell cp list.txt.orig list.txt)
endif

DIR=$(wildcard */)

_list:
	+@ $(foreach dir,$(shell cat list.txt), echo "building $(dir)"; ${MAKE} -s -C $(dir);)

static:
	+@ $(foreach dir,$(shell cat list.txt), echo "building $(dir)"; ${MAKE} -s -C $(dir) static;)

all:
	+@ $(foreach dir,$(DIR), ${MAKE} -s -C $(dir);)
	exit 0

install:
	 +@ $(foreach dir, $(DIR), ${MAKE} -s install   -C $(dir);)

local-install: clean _list
	+@ mkdir -p ~/.gwplug
	+@ $(foreach dir, $(shell cat list.txt), cp $(dir)/*.so ~/.gwplug;)

clean:
	+@ $(foreach dir, $(DIR), ${MAKE} -s clean     -C $(dir);)

uninstall:
	+@ $(foreach dir, $(DIR), ${MAKE} -s uninstall -C $(dir);)

faust2gw/faust2gw:
	${MAKE} -C faust2gw

faust: faust2gw/faust2gw
	@[ -z ${DSP_FILE} ] && (echo "usage: make faust DSP_FILE=somefile.dsp"; false) || true
	@mkdir $$(basename ${DSP_FILE} .dsp)
	@printf "CC=g++\ninclude ../config.mk\ninclude ../config_post.mk" > $$(basename ${DSP_FILE} .dsp)/Makefile
	@cp ${DSP_FILE} $$(basename ${DSP_FILE} .dsp)
	@cd $$(basename ${DSP_FILE} .dsp) && \
	../faust2gw/faust2gw $$(basename ${DSP_FILE}) && \
 	cp .faust2gw_tmp/$$(basename ${DSP_FILE}).cpp . && \
	make && rm -r .faust2gw_tmp
