#include <iostream>
#include <cv.hpp>

#include "Gwion.hpp"

#include "loup.hpp"
#include "fx.hpp"
#include "ciel.hpp"
#include "trounoir.hpp"
#include "bg.hpp"
#include "sobel.hpp"
#include "cartoon1.hpp"
#include "vaisseau.hpp"
#include "vaisseau2.hpp"
#include "vaisseau3.hpp"

#define LOUP(o) (*(Loup**)(o->data + o_loup_cam))
m_int o_loup_cam;

CTOR(loup_ctor) {
  LOUP(o) = new Loup;
}

DTOR(loup_dtor) {
  Loup* loup = LOUP(o);
  delete loup;
}

MFUN(loup_scale_get) {
  *(m_float*)RETURN = LOUP(o)->joy()->scale;
}

MFUN(loup_scale_set) {
  *(m_float*)RETURN = LOUP(o)->joy()->scale = *(m_float*)MEM(SZ_INT);
}

MFUN(loup_x_get) {
  *(m_float*)RETURN = LOUP(o)->joy()->x;
}

MFUN(loup_x_set) {
  *(m_float*)RETURN = LOUP(o)->joy()->x = *(m_float*)MEM(SZ_INT);
}

MFUN(loup_y_get) {
  *(m_float*)RETURN = LOUP(o)->joy()->y;
}

MFUN(loup_y_set) {
  *(m_float*)RETURN = LOUP(o)->joy()->y = *(m_float*)MEM(SZ_INT);
}

MFUN(loup_fx_add) {
  m_int ret = -1;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  std::string str(STRING(obj));
  if(str == "trounoir")
    ret = LOUP(o)->add(new TrouNoir);
  else if(str == "ciel")
    ret = LOUP(o)->add(new Ciel);
  else if(str == "bg")
    ret = LOUP(o)->add(new Bg);
  else if(str == "sobel")
    ret = LOUP(o)->add(new sobel);
  else if(str == "cartoon1")
    ret = LOUP(o)->add(new Cartoon1);
  else if(str == "vaisseau")
    ret = LOUP(o)->add(new Vaisseau);
  else if(str == "vaisseau2")
    ret = LOUP(o)->add(new Vaisseau2);
  else if(str == "vaisseau3")
    ret = LOUP(o)->add(new Vaisseau3);
  release(obj, shred);
  *(m_int*)RETURN = ret;
}

MFUN(loup_fx_set) {
  int index = *(m_int*)MEM(SZ_INT);
  int state = *(m_int*)MEM(SZ_INT * 2);
  *(m_int*)RETURN = LOUP(o)->set(index, state);
}

extern "C" {
IMPORT {
  const Type t_loup = gwi_mk_type(gwi, "Loup", SZ_INT, t_object);
  CHECK_BB(gwi_class_ini(gwi, t_loup, loup_ctor, loup_dtor))

  CHECK_BB(gwi_item_ini(gwi, "int", "@cam"))
  CHECK_BB((o_loup_cam = gwi_item_end(gwi, ae_flag_member, NULL)))

  CHECK_BB(gwi_func_ini(gwi, "float", "scale", loup_scale_get))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "float", "scale", loup_scale_set))
    CHECK_BB(gwi_func_arg(gwi, "float", "f"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "float", "x", loup_x_get))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "float", "x", loup_x_set))
    CHECK_BB(gwi_func_arg(gwi, "float", "f"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "float", "y", loup_y_get))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "float", "y", loup_y_set))
    CHECK_BB(gwi_func_arg(gwi, "float", "f"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "int", "add", loup_fx_add))
    CHECK_BB(gwi_func_arg(gwi, "string", "fx"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_func_ini(gwi, "int", "set", loup_fx_set))
    CHECK_BB(gwi_func_arg(gwi, "int", "index"))
    CHECK_BB(gwi_func_arg(gwi, "int", "state"))
  CHECK_BB(gwi_func_end(gwi, ae_flag_member))

  CHECK_BB(gwi_class_end(gwi));
  return 1;
}
}
