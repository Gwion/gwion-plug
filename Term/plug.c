#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "array.h"
#include <signal.h>
#include "term.h"


static terminal_attr_t term;
static void resize(int _ NUSED) {
  terminal_get_attr(&term);
}

#define term_dims(dim)              \
static SFUN(term_dims_##dim) {      \
  *(m_uint*)RETURN = term.size.dim; \
}
term_dims(cols)
term_dims(rows)
term_dims(width)
term_dims(height)

#define term_func(flag)                                            \
static SFUN(term_func_##flag) {                                    \
  *(m_uint*)RETURN = terminal_get_flag(&term, TerminalFlag##flag); \
}
term_func(SupportsUTF8)
term_func(SupportsColor)
term_func(IsBlocking)
term_func(IsLineBuffered)

static SFUN(term_color1) {
  char c[16];
  sprintf(c, "\e[%lum", 30 + *(m_uint*)MEM(0));
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion->mp, shred, c);
}

static SFUN(term_color2) {
  char c[16];
  sprintf(c, "\e[%lu;%lum", 30 + *(m_uint*)MEM(0), 40 + *(m_uint*)MEM(SZ_INT));
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion->mp, shred, c);
}

static SFUN(term_color3) {
  char c[16];
  sprintf(c, "\e[%lu;%lu;%lum", 30 + *(m_uint*)MEM(0),
                            40 + *(m_uint*)MEM(SZ_INT),
                            *(m_uint*)MEM(SZ_INT*2));
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion->mp, shred, c);
}

static SFUN(term_color4) {
  char c[16];
  sprintf(c, "\e[%lu;%lu;%sm", 30 + *(m_uint*)MEM(0),
                            40 + *(m_uint*)MEM(SZ_INT),
                            STRING(*(M_Object*)MEM(SZ_INT*2)));
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion->mp, shred, c);
}

static INSTR(term_attr_concat) {
  POP_REG(shred, SZ_INT);
  char c[16];
  sprintf(c, "%lu;%lu", *(m_uint*)REG(-SZ_INT), *(m_uint*)REG(0));
  *(M_Object*)REG(-SZ_INT) = new_string(shred->info->vm->gwion->mp, shred, c);
}

static INSTR(term_str_attr_concat) {
  POP_REG(shred, SZ_INT);
  const M_Object _lhs = *(M_Object*)REG(-SZ_INT);
  const m_str lhs = STRING(_lhs);
  char c[strlen(lhs) + 16];
  sprintf(c, "%s;%lu", lhs, *(m_uint*)REG(0));
  *(M_Object*)REG(-SZ_INT) = new_string(shred->info->vm->gwion->mp, shred, c);
}

static INSTR(term_attr_str_concat) {
  POP_REG(shred, SZ_INT);
  const M_Object _rhs = *(M_Object*)REG(0);
  const m_str rhs = STRING(_rhs);
  char c[strlen(rhs) + 16];
  sprintf(c, "%lu;%s", *(m_uint*)REG(-SZ_INT), rhs);
  *(M_Object*)REG(-SZ_INT) = new_string(shred->info->vm->gwion->mp, shred, c);
}

static INSTR(term_str_str_concat) {
  POP_REG(shred, SZ_INT);
  const M_Object _lhs = *(M_Object*)REG(-SZ_INT);
  const m_str lhs = STRING(_lhs);
  const M_Object _rhs = *(M_Object*)REG(0);
  const m_str rhs = STRING(_rhs);
  char c[strlen(lhs) + strlen(rhs) + 2];
  sprintf(c, "%s;%s", lhs, rhs);
  *(M_Object*)REG(-SZ_INT) = new_string(shred->info->vm->gwion->mp, shred, c);
}

GWION_IMPORT(Term) {
  resize(0);
#ifndef BUILD_ON_WINDOWS
  signal(SIGWINCH, resize);
#endif
  DECL_OB(const Type, t_term, = gwi_class_ini(gwi, "Term", "Object"))

  GWI_BB(gwi_func_ini(gwi, "int", "cols"))
  GWI_BB(gwi_func_end(gwi, term_dims_cols, ae_flag_static))
  GWI_BB(gwi_func_ini(gwi, "int", "rows"))
  GWI_BB(gwi_func_end(gwi, term_dims_rows, ae_flag_static))
  GWI_BB(gwi_func_ini(gwi, "int", "width"))
  GWI_BB(gwi_func_end(gwi, term_dims_width, ae_flag_static))
  GWI_BB(gwi_func_ini(gwi, "int", "height"))
  GWI_BB(gwi_func_end(gwi, term_dims_height, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "bool", "supportsUTF8"))
  GWI_BB(gwi_func_end(gwi, term_func_SupportsUTF8, ae_flag_static))
  GWI_BB(gwi_func_ini(gwi, "bool", "supportsColor"))
  GWI_BB(gwi_func_end(gwi, term_func_SupportsColor, ae_flag_static))
  GWI_BB(gwi_func_ini(gwi, "bool", "isBlocking"))
  GWI_BB(gwi_func_end(gwi, term_func_IsBlocking, ae_flag_static))
  GWI_BB(gwi_func_ini(gwi, "bool", "isLineBuffered"))
  GWI_BB(gwi_func_end(gwi, term_func_IsLineBuffered, ae_flag_static))

  GWI_BB(gwi_enum_ini(gwi, "Color"));
  GWI_BB(gwi_enum_add(gwi, "black", 0));
  GWI_BB(gwi_enum_add(gwi, "red", 1));
  GWI_BB(gwi_enum_add(gwi, "green", 2));
  GWI_BB(gwi_enum_add(gwi, "yellow", 3));
  GWI_BB(gwi_enum_add(gwi, "blue", 4));
  GWI_BB(gwi_enum_add(gwi, "magenta", 5));
  GWI_BB(gwi_enum_add(gwi, "cyan", 6));
  GWI_BB(gwi_enum_add(gwi, "white", 7));
  GWI_BB(gwi_enum_add(gwi, "default", 9));
  GWI_BB(gwi_enum_end(gwi));

  GWI_BB(gwi_enum_ini(gwi, "Attr"));
  GWI_BB(gwi_enum_add(gwi, "off", 0));
  GWI_BB(gwi_enum_add(gwi, "bold", 1));
  GWI_BB(gwi_enum_add(gwi, "dim", 2));
  GWI_BB(gwi_enum_add(gwi, "italic", 3));
  GWI_BB(gwi_enum_add(gwi, "underscore", 4));
  GWI_BB(gwi_enum_add(gwi, "blink", 5));
  GWI_BB(gwi_enum_add(gwi, "fastblink", 6));
  GWI_BB(gwi_enum_add(gwi, "reverse", 7));
  GWI_BB(gwi_enum_add(gwi, "concealed", 8));
  GWI_BB(gwi_enum_add(gwi, "crossed", 9));
  GWI_BB(gwi_enum_end(gwi));

  GWI_OB(gwi_class_ini(gwi, "AttrStr", "string"))
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_func_ini(gwi, "string", "color"))
  GWI_BB(gwi_func_arg(gwi, "Color", "foreground"))
  GWI_BB(gwi_func_end(gwi, term_color1, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "string", "color"))
  GWI_BB(gwi_func_arg(gwi, "Color", "foreground"))
  GWI_BB(gwi_func_arg(gwi, "Color", "background"))
  GWI_BB(gwi_func_end(gwi, term_color2, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "string", "color"))
  GWI_BB(gwi_func_arg(gwi, "Color", "foreground"))
  GWI_BB(gwi_func_arg(gwi, "Color", "background"))
  GWI_BB(gwi_func_arg(gwi, "Attr",  "attribute"))
  GWI_BB(gwi_func_end(gwi, term_color3, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "string", "color"))
  GWI_BB(gwi_func_arg(gwi, "Color", "foreground"))
  GWI_BB(gwi_func_arg(gwi, "Color", "background"))
  GWI_BB(gwi_func_arg(gwi, "AttrStr",  "attribute"))
  GWI_BB(gwi_func_end(gwi, term_color4, ae_flag_static))

  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "Term.Attr", "Term.Attr", "Term.AttrStr"))
  GWI_BB(gwi_oper_end(gwi, "|", term_attr_concat))

  GWI_BB(gwi_oper_ini(gwi, "Term.AttrStr", "Term.Attr", "Term.AttrStr"))
  GWI_BB(gwi_oper_end(gwi, "|", term_str_attr_concat))

  GWI_BB(gwi_oper_ini(gwi, "Term.Attr", "Term.AttrStr", "Term.AttrStr"))
  GWI_BB(gwi_oper_end(gwi, "|", term_attr_str_concat))

  GWI_BB(gwi_oper_ini(gwi, "Term.AttrStr", "Term.AttrStr", "Term.AttrStr"))
  GWI_BB(gwi_oper_end(gwi, "|", term_str_str_concat))

  return GW_OK;
}
