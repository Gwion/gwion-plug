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

static CTOR(xml_ctor) {
  const Value v = (Value)map_at(&o->type_ref->nspc->info->value->map, 3);
  *(M_Object*)(o->data + SZ_INT*2) = new_array(shred->info->vm->gwion->mp, v->type, 0);
}

static DTOR(xml_dtor) { /*code here */ }

static m_int o_xml_member_data;
static m_int o_xml_static_data;
static m_int* xml_static_value;

static MFUN(mfun) { /*code here */ }
static SFUN(sfun) { /*code here */ }

GWION_IMPORT(XML) {
  DECL_OB(const Type, t_xml, = gwi_class_ini(gwi, "XML", "Object"));
  gwi_class_xtor(gwi, xml_ctor, xml_dtor);

  DECL_OB(const Type, t_attr, = gwi_class_ini(gwi, "Attribute", "Object"));
//  gwi_class_xtor(gwi, attr_ctor, attr_dtor);
  GWI_BB(gwi_item_ini(gwi, "string", "name"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))


  GWI_BB(gwi_union_ini(gwi, "Data"))
  GWI_BB(gwi_union_add(gwi, "string", "str"))
  GWI_BB(gwi_union_add(gwi, "int", "num"))
  GWI_BB(gwi_union_add(gwi, "float", "fnum"))
  GWI_BB(gwi_union_add(gwi, "None", "none"))
  GWI_OB(gwi_union_end(gwi, ae_flag_none))
  GWI_BB(gwi_item_ini(gwi, "string", "value"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_item_ini(gwi, "string", "name"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_item_ini(gwi, "Attribute[]", "attrributes"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_item_ini(gwi, "XML[]", "children"))
  GWI_BB(gwi_item_end(gwi, ae_flag_none, num, 0))

  GWI_BB(gwi_func_ini(gwi, "int", "mfun"))
  GWI_BB(gwi_func_arg(gwi, "int", "arg"))
  GWI_BB(gwi_func_end(gwi, mfun, ae_flag_none))

  GWI_BB(gwi_func_ini(gwi, "int", "sfun"))
  GWI_BB(gwi_func_arg(gwi, "int", "arg"))
  GWI_BB(gwi_func_end(gwi, sfun, ae_flag_static))

  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
