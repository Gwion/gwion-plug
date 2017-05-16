#!/bin/sh
#needs an argument
[ "$1" ] || {
	echo "usage: $0 <plugin name>"
	exit 1
}

mkdir $1
echo "\
#USE_LD=1 # needed for e.g. sporth static lib
include ../config.mk
NAME=$1
#CFLAGS+=-I<more include>
#LDFLAGS+=-l<more LIBS>
include ../config_post.mk
" > $1/Makefile

echo "\
#$1#
  a [Gwion](https://github.com/fennecdjay/Gwion) plugin.  
##description##
use [$1](https://github.com/.../$1)
##configuration##
check your [Gwion-plug](https://github.com/fennecdjay/Gwion-plug) configuration. (e.g. edit config.mk)  
edit Makefile
##building##
ensure [$1](https://github.com/.../$1) is installed
```
make
```
##instalation##
```
make install
```
##usage##
check .gw files in the directory." > $1/README.md
