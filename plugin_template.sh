#!/bin/sh

set -e

[ "$1" ] || {
	echo "usage: $0 <plugin name>"
	exit 1
}

mkdir $1
cat << EOF >> $1/Makefile
#USE_LD=1 # needed for e.g. sporth static lib
include ../config.mk
NAME=$1
#CFLAGS+=-I<more include>
#LDFLAGS+=-l<more LIBS>
include ../config_post.mk
EOF

cat << EOF > $1/README.md
# $1
  a [Gwion](https://github.com/fennecdjay/Gwion) plugin.  
## Description
use [$1](https://github.com/.../$1)
## Configuration
check your [Gwion-plug](https://github.com/fennecdjay/Gwion-plug) configuration. (e.g. edit config.mk)  
edit Makefile
## Building
ensure [$1](https://github.com/.../$1) is installed
\`\`\`
make
# optionnal
make install
\`\`\`
# Usage
check .gw files in the directory.
EOF

cat << EOF > $1/${1,,}.c
#include "type.h"
#include "instr.h"
#include "import.h"
//#include "err_msg.h"
//#include "lang.h"
//#include "ugen.h"

static struct Type_ t_${1,,} = { "$1", SZ_INT, &t_object };

CTOR(${1,,}_ctor) { /*code here */ }

DTOR(${1,,}_dtor) { /*code here */ }

m_int o_${1,,}_member_data;
m_int o_${1,,}_static_data;
m_int* ${1,,}_static_value;

static MFUN(mfun) { /*code here */ }
static SFUN(sfun) { /*code here */ }

IMPORT
{
  CHECK_BB(importer_class_begin(importer, &t_${1,,},${1,,}_ctor, ${1,,}_dtor))

  o_${1,,}_member_data = importer_add_var(importer, "int",  "member", ae_flag_member, NULL);

  ${1,,}_static_value = malloc(sizeof(m_int));
  o_${1,,}_static_data = importer_addvar(importer, "int", "static", ae_flag_static, ${1,,}_static_value);

  importer_func_begin(importer, "int", "mfun",  (m_uint)mfun);
    importer_add_arg(importer, "int", "arg");
  CHECK_BB(importer_add_fun(importer, ae_flag_member))

  importer_func_init(importer, "int", "sfun",  (m_uint)sfun);
    importer_add_arg(importer, "int", "arg");
  CHECK_BB(importer_add_fun(importer, ae_flag_static))

  CHECK_BB(importer_class_end(importer))
  return 1;
}
EOF
