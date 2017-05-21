#!/bin/sh
#needs an argument
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
#$1#
  a [Gwion](https://github.com/fennecdjay/Gwion) plugin.  
##description##
use [$1](https://github.com/.../$1)
##configuration##
check your [Gwion-plug](https://github.com/fennecdjay/Gwion-plug) configuration. (e.g. edit config.mk)  
edit Makefile
##building##
ensure [$1](https://github.com/.../$1) is installed
\`\`\`
make
\`\`\`
##instalation##
\`\`\`
make install
\`\`\`
##usage##
check .gw files in the directory.
EOF

cat << EOF > $1/${1,,}.c
#include "type.h"
#include "dl.h"
#include "import.h"
#include "err_msg.h"
#include "lang.h"

static struct Type_ t_${1,,} = { "$1", SZ_INT, &t_object };

CTOR($1_ctor) { /*code here */ }

DTOR($1_dtor) { /*code here */ }

m_int o_${1}_member_data;
m_int o_${1}_static_data;
m_int* o_${1}_static_value;

static MFUN(mfun) { /*code here */ }
static SFUN(sfun) { /*code here */ }

IMPORT
{
  DL_Func* fun;

  CHECK_BB(add_global_type(env, &t_${1,,}))
  CHECK_BB(import_class_begin(env, &t_${1,,}, env->global_nspc, ${1}_ctor, ${1}_dtor))

  o_${1}_member_data = import_mvar(env, "int",  "member", 0, 0, "doc");

  o_${1}_static_value = malloc(sizeof(m_int));
  o_${1}_static_data = import_svar(env, "int", "static", 1, 0, o_${1}_static_value, "doc.");

  fun = new_DL_Func("int", "mfun",  (m_uint)mfun);
    dl_func_add_arg(fun, "int", "arg");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "sfun",  (m_uint)sfun);
    dl_func_add_arg(fun, "int", "arg");
  CHECK_OB(import_mfun(env, fun))

  CHECK_BB(import_class_end(env))
  return 1;
}
EOF
