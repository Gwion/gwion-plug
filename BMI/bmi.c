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
#include "gwi.h"
#include "gack.h"

#include "bmi.h"
#include "bmi/tests.h"

#define GWI_BMI(o) (*(bmi_buffer**)(o)->data)

static DTOR(bmi_dtor) {
  xfree(*(bmi_buffer**)o->data);
}

static MFUN(gwbmi_newrgb) {
  const m_uint r = *(m_uint*)MEM(0);
  const m_uint g = *(m_uint*)MEM(SZ_INT);
  const m_uint b = *(m_uint*)MEM(SZ_INT*2);
  *(bmi_component*)RETURN = BMI_RGB(r,g,b);
}

static MFUN(gwbmi_newgray) {
  const m_uint g = *(m_uint*)MEM(SZ_INT);
  *(bmi_component*)RETURN = BMI_GRY(g);
}

#define GWI_POINT(o) \
  BMI_POINT(*(m_uint*)(o)->data, *(m_uint*)((o)->data + SZ_INT))

static GACK(bmi_point_gack) {
    const M_Object o = *(M_Object*)VALUE;
    const bmi_point point = GWI_POINT(o);
    INTERP_PRINTF("(x: %u, y: %u)\n", point.x, point.y)
}

static MFUN(gwbmi_draw_point) {
  M_Object point = *(M_Object*)MEM(SZ_INT);
  m_uint pixel = *(m_uint*)MEM(SZ_INT*2);
  bmi_buffer_draw_point(GWI_BMI(o), GWI_POINT(point), pixel);
}

#define GWI_RECT(o) \
  BMI_RECT(                             \
      *(m_uint*)((o)->data),            \
      *(m_uint*)((o)->data + SZ_INT),   \
      *(m_uint*)((o)->data + SZ_INT*2), \
      *(m_uint*)((o)->data + SZ_INT*3))

static GACK(bmi_rect_gack) {
    const M_Object o = *(M_Object*)VALUE;
    const bmi_rect rect = GWI_RECT(o);
    INTERP_PRINTF("(x: %u, y: %u): %u by %u\n", rect.x, rect.y, rect.w, rect.h);
}

static MFUN(gwbmi_fill_rect) {
  const M_Object rect = *(M_Object*)MEM(SZ_INT);
  const m_uint pixel = *(m_uint*)MEM(SZ_INT*2);
  const bmi_rect ret = GWI_RECT(rect);
  bmi_buffer_fill_rect(GWI_BMI(o), ret, pixel);
}

static MFUN(gwbmi_stroke_rect) {
  M_Object rect = *(M_Object*)MEM(SZ_INT);
  m_uint t = *(m_uint*)MEM(SZ_INT*2);
  m_uint pixel = *(m_uint*)MEM(SZ_INT*3);
  bmi_buffer_stroke_rect(GWI_BMI(o), GWI_RECT(rect), t, pixel);
}

static MFUN(gwbmi_rect_inset) {
  const m_uint delta = *(m_uint*)MEM(SZ_INT);
  const m_uint edge  = *(m_uint*)MEM(SZ_INT*2);
  bmi_inset_rect(&GWI_RECT(o), delta, edge);
}

static MFUN(gwbmi_rect_set) {
  const m_uint width = *(m_uint*)MEM(SZ_INT);
  const m_uint edge  = *(m_uint*)MEM(SZ_INT*2);
  bmi_set_rect(&GWI_RECT(o), width, edge);
}

static MFUN(gwbmi_rect_clip) {
  M_Object rect = *(M_Object*)MEM(SZ_INT);
  bmi_rect ret = GWI_RECT(o);
  bmi_clip_rect(&ret, GWI_RECT(rect));
  *(m_uint*)(o)->data = ret.x;
  *(m_uint*)((o)->data + SZ_INT) = ret.y;
  *(m_uint*)((o)->data + SZ_INT*2) = ret.y;
  *(m_uint*)((o)->data + SZ_INT*3) = ret.y;
}


// Clips the specified rectangle to the given bounds
//void bmi_clip_rect(bmi_rect* rect, const bmi_rect bounds);


//Color needs a *Gack*

static SFUN(test_ppm) {
  *(m_uint*)RETURN = test_create_smile_ppm();
}

static MFUN(gwbmi2file) {
  FILE *f = fopen(STRING(*(M_Object*)MEM(SZ_INT)), "w");
  *(m_uint*)RETURN = bmi_buffer_to_file(f, *(bmi_buffer**)o->data);
  fclose(f);
}

static MFUN(gwbmi2ppm) {
  FILE *f = fopen(STRING(*(M_Object*)MEM(SZ_INT)), "w");
  *(m_uint*)RETURN = bmi_buffer_to_ppm(f, *(bmi_buffer**)o->data);
  fclose(f);
}

static MFUN(gwbmi2bmp) {
  FILE *f = fopen(STRING(*(M_Object*)MEM(SZ_INT)), "w");
  *(m_uint*)RETURN = bmi_buffer_to_bmp(f, *(bmi_buffer**)o->data);
  fclose(f);
}

static SFUN(gwbmi_create) {
  const m_uint width = *(m_uint*)MEM(0);
  const m_uint height = *(m_uint*)MEM(SZ_INT);
  const m_uint flags = *(m_uint*)MEM(SZ_INT*2);
  bmi_buffer *buffer = bmi_buffer_new(width, height, flags);
  if(!buffer)
    Except(shred, "[{+R}BMI{0}] can't create buffer.")
  const M_Object ret = new_object_str(shred->info->vm->gwion, shred, "BMI");
  *(bmi_buffer**)ret->data = buffer;
  const size_t sz = bmi_buffer_content_size(buffer);
  memset(buffer->contents, 0, sz);
  *(M_Object*)RETURN = ret;
}

static SFUN(gwbmi_from_file) {
  FILE *f = fopen(STRING(*(M_Object*)MEM(SZ_INT)), "w");
  bmi_buffer* buffer = bmi_buffer_from_file(f);
  if(!buffer)
    Except(shred, "[{+R}BMI{0}] can't open file.")
  const M_Object ret = new_object_str(shred->info->vm->gwion, shred, "BMI");
  *(bmi_buffer**)ret->data = buffer;
  *(M_Object*)RETURN = ret;
  fclose(f);
}

static INSTR(gwbmi_color_assign) {
  POP_REG(shred, sizeof(bmi_component));
  *(bmi_component*)REG(-sizeof(bmi_component)) = *(bmi_component*)REG(0);
}

GWION_IMPORT(BMI) {
  DECL_OB(const Type, t_bmi, = gwi_class_ini(gwi, "BMI", "Object"))
  gwi_class_xtor(gwi, NULL, bmi_dtor);
  t_bmi->nspc->info->offset += SZ_INT; //allocate room for the buffer
  // we don't want to create BMI by other means than provided constructors
  SET_FLAG(t_bmi, abstract);

  DECL_OB(const Type, t_point, = gwi_class_ini(gwi, "Point", "Object"))
  gwi_gack(gwi, t_point, bmi_point_gack);
  GWI_BB(gwi_item_ini(gwi, "int", "x"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "y"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_enum_ini(gwi, "Edge"))
  GWI_BB(gwi_enum_add(gwi, "left", BMI_RECT_EDGE_LEFT))
  GWI_BB(gwi_enum_add(gwi, "right", BMI_RECT_EDGE_RIGHT))
  GWI_BB(gwi_enum_add(gwi, "top", BMI_RECT_EDGE_TOP))
  GWI_BB(gwi_enum_add(gwi, "bottom", BMI_RECT_EDGE_BOTTOM))
  GWI_BB(gwi_enum_end(gwi))

  DECL_OB(const Type, t_rect, = gwi_class_ini(gwi, "Rect", "Object"))
  gwi_gack(gwi, t_rect, bmi_rect_gack);
  GWI_BB(gwi_item_ini(gwi, "int", "x"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "y"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "w"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "h"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_func_ini(gwi, "void", "clipTo"))
  GWI_BB(gwi_func_arg(gwi, "Rect", "rect"))
  GWI_BB(gwi_func_end(gwi, gwbmi_rect_clip, ae_flag_none))
  GWI_BB(gwi_func_ini(gwi, "void", "inset")) //???
  GWI_BB(gwi_func_arg(gwi, "int", "delta"))
  GWI_BB(gwi_func_arg(gwi, "Edge", "edge")) // as we don't have the enum yet
  GWI_BB(gwi_func_end(gwi, gwbmi_rect_inset, ae_flag_none))
  GWI_BB(gwi_func_ini(gwi, "void", "set")) //???
  GWI_BB(gwi_func_arg(gwi, "int", "width"))
  GWI_BB(gwi_func_arg(gwi, "Edge", "edge")) // as we don't have the enum yet
  GWI_BB(gwi_func_end(gwi, gwbmi_rect_set, ae_flag_none))


  GWI_BB(gwi_class_end(gwi))

// TODO: should inherit from component
  DECL_OB(const Type, t_color, = gwi_mk_type(gwi, "Color", sizeof(bmi_component), NULL))
  gwi_add_type(gwi, t_color);

  DECL_OB(const Type, t_flag, = gwi_struct_ini(gwi, "Flag"))
  GWI_BB(gwi_item_ini(gwi, "int", "none"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "grayscale"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_FL_IS_GRAYSCALE))
  GWI_BB(gwi_struct_end(gwi))

  DECL_OB(const Type, t_rgb, = gwi_struct_ini(gwi, "RGB"))
// more colors :)
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "White"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_WHITE()))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Black"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_BLACK()))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Red"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_RED()))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Green"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_GREEN()))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Blue"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_BLUE()))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Magenta"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_MAGENTA()))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Yellow"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_YELLOW()))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Orange"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_ORANGE()))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Cyan"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_CYAN()))
  GWI_BB(gwi_struct_end(gwi))

  DECL_OB(const Type, t_gry, = gwi_struct_ini(gwi, "Gray"))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "White"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_GRY_WHITE()))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Black"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_GRY_BLACK()))
  GWI_BB(gwi_struct_end(gwi))

  GWI_BB(gwi_func_ini(gwi, "void", "draw_point"))
  GWI_BB(gwi_func_arg(gwi, "Point", "point"))
  GWI_BB(gwi_func_arg(gwi, "Color", "color"))
  GWI_BB(gwi_func_end(gwi, gwbmi_draw_point, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "fill_rect"))
  GWI_BB(gwi_func_arg(gwi, "Rect", "rect"))
  GWI_BB(gwi_func_arg(gwi, "Color", "color"))
  GWI_BB(gwi_func_end(gwi, gwbmi_fill_rect, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "void", "stroke_rect"))
  GWI_BB(gwi_func_arg(gwi, "Rect", "rect"))
  GWI_BB(gwi_func_arg(gwi, "int", "t"))
  GWI_BB(gwi_func_arg(gwi, "Color", "color"))
  GWI_BB(gwi_func_end(gwi, gwbmi_stroke_rect, ae_flag_none))

  GWI_BB(gwi_item_ini(gwi, "int", "member"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_item_ini(gwi, "int", "static"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, 1234))

  GWI_BB(gwi_func_ini(gwi, "Color", "newRGB"))
  GWI_BB(gwi_func_arg(gwi, "int", "r"))
  GWI_BB(gwi_func_arg(gwi, "int", "g"))
  GWI_BB(gwi_func_arg(gwi, "int", "b"))
  GWI_BB(gwi_func_end(gwi, gwbmi_newrgb, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "Color", "newGray"))
  GWI_BB(gwi_func_arg(gwi, "int", "g"))
  GWI_BB(gwi_func_end(gwi, gwbmi_newgray, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "int", "test_ppm"))
  GWI_BB(gwi_func_end(gwi, test_ppm, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "int", "to_file"))
  GWI_BB(gwi_func_arg(gwi, "string", "filename"))
  GWI_BB(gwi_func_end(gwi, gwbmi2file, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "to_ppm"))
  GWI_BB(gwi_func_arg(gwi, "string", "filename"))
  GWI_BB(gwi_func_end(gwi, gwbmi2ppm, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "to_file"))
  GWI_BB(gwi_func_arg(gwi, "string", "filename"))
  GWI_BB(gwi_func_end(gwi, gwbmi2bmp, ae_flag_none))

  // constructor functions
  GWI_BB(gwi_func_ini(gwi, "BMI", "create"))
  GWI_BB(gwi_func_arg(gwi, "int", "width"))
  GWI_BB(gwi_func_arg(gwi, "int", "height"))
  GWI_BB(gwi_func_arg(gwi, "int", "flags"))
  GWI_BB(gwi_func_end(gwi, gwbmi_create, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "BMI", "to_file"))
  GWI_BB(gwi_func_arg(gwi, "string", "filename"))
  GWI_BB(gwi_func_end(gwi, gwbmi2bmp, ae_flag_none))

  GWI_BB(gwi_union_ini(gwi, "Result"))
  GWI_BB(gwi_union_add(gwi, "BMI", "success"))
  GWI_BB(gwi_union_add(gwi, "string", "error"))
  GWI_BB(gwi_union_end(gwi, ae_flag_none))

  const Type t = str2type(gwi->gwion, "BMI.Result", gwi->loc);
  const m_uint scope = env_push_type(gwi->gwion->env, t);
  GWI_BB(gwi_func_ini(gwi, "BMI", "to_file"))
  GWI_BB(gwi_func_arg(gwi, "string", "filename"))
  GWI_BB(gwi_func_end(gwi, gwbmi2bmp, ae_flag_none))
  env_pop(gwi->gwion->env, scope);
  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "BMI.Color", "BMI.Color", "BMI.Color"))
  GWI_BB(gwi_oper_add(gwi, opck_rassign))
  GWI_BB(gwi_oper_end(gwi, "=>", gwbmi_color_assign))
  return GW_OK;
}
