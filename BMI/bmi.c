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

#include "bmi.h"
#include "bmi/tests.h"

#define GWI_BMI(o) (*(bmi_buffer**)(o)->data)

static CTOR(bmi_ctor) {
//  *(bmi_buffer**)o->data = 
}

static DTOR(bmi_dtor) {
  xfree(*(bmi_buffer**)o->data);
}

static m_int o_bmi_member_data;
static m_int o_bmi_static_data;
static m_int* bmi_static_value;

static MFUN(mfun) { /*code here */ }
static SFUN(sfun) { /*code here */ }

static MFUN(gwbmi_newrgb) {
  const m_uint r = *(m_uint*)MEM(0);
  const m_uint g = *(m_uint*)MEM(SZ_INT);
  const m_uint b = *(m_uint*)MEM(SZ_INT*2);
printf("rgb %u %u %u\n", r,g,b);
  *(bmi_component*)RETURN = BMI_RGB(r,g,b);
printf("component %u\n", *(bmi_component*)RETURN);
}

static MFUN(gwbmi_newgray) {
  const m_uint g = *(m_uint*)MEM(SZ_INT);
  *(bmi_component*)RETURN = BMI_GRY(g);
}

#define GWI_POINT(o) \
  BMI_POINT(*(m_uint*)(o)->data, *(m_uint*)((o)->data + SZ_INT))

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

static MFUN(gwbmi_fill_rect) {
  const M_Object rect = *(M_Object*)MEM(SZ_INT);
  const m_uint pixel = *(m_uint*)MEM(SZ_INT*2);
printf("red???? %u\n", pixel);
  const bmi_rect ret = GWI_RECT(rect);
printf("%u %u %u %u\n", ret.x , ret.y, ret.w, ret.h);
//  bmi_buffer_fill_rect(GWI_BMI(o), ret, pixel); I should check the buffe maybe?
printf("buffer (fill_rect) %p\n", GWI_BMI(o));
  bmi_buffer_fill_rect(GWI_BMI(o), ret, pixel);
//exit(3);
}

static MFUN(gwbmi_stroke_rect) {
  M_Object rect = *(M_Object*)MEM(SZ_INT);
  m_uint t = *(m_uint*)MEM(SZ_INT*2);
  m_uint pixel = *(m_uint*)MEM(SZ_INT*3);
  bmi_buffer_stroke_rect(GWI_BMI(o), GWI_RECT(rect), t, pixel);
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
printf("height %lu\n", height);
  const m_uint flags = *(m_uint*)MEM(SZ_INT*2);
  const M_Object ret = new_object_str(shred->info->vm->gwion, shred, "BMI");
  *(bmi_buffer**)ret->data = bmi_buffer_new(width, height, flags);
printf("buffer (create) %p\n", GWI_BMI(ret));
printf("buffer (create) %p\n", *(bmi_buffer**)ret->data);
  const size_t sz = bmi_buffer_content_size(*(bmi_buffer**)ret->data);
//  memset((*(bmi_buffer**)ret->data)->contents, 0, sz);
  *(M_Object*)RETURN = ret;
}

static SFUN(gwbmi_from_file) {
  FILE *f = fopen(STRING(*(M_Object*)MEM(SZ_INT)), "w");
  const M_Object ret = new_object_str(shred->info->vm->gwion, shred, "BMI");
  *(bmi_buffer**)ret->data = bmi_buffer_from_file(f);
  *(M_Object*)RETURN = ret;
  fclose(f);
}

GWION_IMPORT(BMI) {
  DECL_OB(const Type, t_bmi, = gwi_class_ini(gwi, "BMI", "Object"))
  gwi_class_xtor(gwi, bmi_ctor, bmi_dtor);
  t_bmi->nspc->info->offset += SZ_INT; //allocate room for the buffer
  // we don't want to create BMI by other means than provided constructors
  SET_FLAG(t_bmi, abstract);

  GWI_OB(gwi_class_ini(gwi, "Point", "Object"))
  GWI_BB(gwi_item_ini(gwi, "int", "x"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "y"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))
  GWI_BB(gwi_class_end(gwi))

  GWI_OB(gwi_class_ini(gwi, "Rect", "Object"))
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
  GWI_BB(gwi_class_end(gwi))

// TODO: should inherit from component
  DECL_OB(const Type, t_color, = gwi_mk_type(gwi, "Color", sizeof(bmi_component), NULL))
//  gwi_class_xtor(gwi, bmi_ctor, bmi_dtor);
  gwi_add_type(gwi, t_color);

  DECL_OB(const Type, t_flag, = gwi_struct_ini(gwi, "Flag"))
  GWI_BB(gwi_item_ini(gwi, "int", "none"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, 0))
  GWI_BB(gwi_item_ini(gwi, "int", "grayscale"))
  GWI_BB(gwi_item_end(gwi, ae_flag_static, num, BMI_FL_IS_GRAYSCALE))
  GWI_BB(gwi_struct_end(gwi))

printf("red %u\n",BMI_RGB_RED());

  DECL_OB(const Type, t_rgb, = gwi_struct_ini(gwi, "RGB"))
// more colors :)
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "White"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_WHITE())))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Black"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_BLACK())))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Red"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_RED())))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Green"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_GREEN())))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Blue"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_BLUE())))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Magenta"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_MAGENTA())))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Yellow"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_YELLOW())))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Cyan"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_RGB_CYAN())))
  GWI_BB(gwi_struct_end(gwi))

  DECL_OB(const Type, t_gry, = gwi_struct_ini(gwi, "Gray"))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "White"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_GRY_WHITE())))
  GWI_BB(gwi_item_ini(gwi, "BMI.Color", "Black"))
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_static, num, BMI_GRY_BLACK())))
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
  GWI_BB((o_bmi_member_data = gwi_item_end(gwi, ae_flag_none, num, 0)))

  GWI_BB(gwi_item_ini(gwi, "int", "static"))
  GWI_BB((o_bmi_static_data = gwi_item_end(gwi, ae_flag_static, num, 1234)))

  GWI_BB(gwi_func_ini(gwi, "Color", "newRGB"))
  GWI_BB(gwi_func_arg(gwi, "int", "r"))
  GWI_BB(gwi_func_arg(gwi, "int", "g"))
  GWI_BB(gwi_func_arg(gwi, "int", "b"))
  GWI_BB(gwi_func_end(gwi, gwbmi_newrgb, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "Color", "newGray"))
  GWI_BB(gwi_func_arg(gwi, "int", "g"))
  GWI_BB(gwi_func_end(gwi, gwbmi_newgray, ae_flag_static))

  GWI_BB(gwi_func_ini(gwi, "int", "mfun"))
  GWI_BB(gwi_func_arg(gwi, "int", "arg"))
  GWI_BB(gwi_func_end(gwi, mfun, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "sfun"))
  GWI_BB(gwi_func_arg(gwi, "int", "arg"))
  GWI_BB(gwi_func_end(gwi, sfun, ae_flag_static))

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

  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
