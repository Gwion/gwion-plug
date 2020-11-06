#!/bin/sh

set -e

[ "$1" ] || {
	echo "usage: $0 <plugin name> (parent type)"
	exit 1
}

PARENT_CLASS=t_object

[ "$2" ] && PARENT_CLASS="$2"


mkdir "$1"
cat << EOF >> "$1/Makefile"
#USE_LD=1 # needed for e.g. sporth static lib
include ../config.mk
NAME=$1
#CFLAGS+=-I<more include>
#LDFLAGS+=-l<more LIBS>
include ../config_post.mk
EOF

cat << EOF > "$1/README.md"
# $1
  a [Gwion](https://github.com/Gwion/Gwion) plugin.  
## Description
use [$1](https://github.com/.../$1)
## Configuration
check your [Gwion-plug](https://github.com/Gwion/gwion-plug) configuration. (e.g. edit config.mk)  
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

cat << EOF > "$1/${1,,}.c"
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "gwion.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "array.h"

static CTOR(${1,,}_ctor) { /*code here */ }

static DTOR(${1,,}_dtor) { /*code here */ }

static m_int o_${1,,}_member_data;
static m_int o_${1,,}_static_data;
static m_int* ${1,,}_static_value;

static MFUN(mfun) { /*code here */ }
static SFUN(sfun) { /*code here */ }

GWION_IMPORT($1) {
  DECL_OB(const Type, t_${1,,}, = gwi_class_ini(gwi, "${1,,}", "$PARENT_CLASS"))
  gwi_class_xtor(gwi, ${1,,}_ctor, ${1,,}_dtor);

  CHECK_BB(gwi_item_ini(gwi, "int", "member"))
  CHECK_BB((o_${1,,}_member_data = gwi_item_end(gwi, ae_flag_none, NULL)))

  ${1,,}_static_value = malloc(sizeof(m_int));
  CHECK_BB(gwi_item_ini(gwi, "int", "static"))
  CHECK_BB((o_${1,,}_static_data = gwi_item_end(gwi, ae_flag_static, ${1,,}_static_value)))

  CHECK_BB(gwi_func_ini(gwi, "int", "mfun"))
  CHECK_BB(gwi_func_arg(gwi, "int", "arg"))
  CHECK_BB(gwi_func_end(gwi, mfun, ae_flag_none))

  CHECK_BB(gwi_func_ini(gwi, "int", "sfun"))
  CHECK_BB(gwi_func_arg(gwi, "int", "arg"))
  CHECK_BB(gwi_func_end(gwi, sfun, ae_flag_static))

  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
EOF
