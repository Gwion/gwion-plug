#include "plugin_dev.h"

// SFUN to define a static function
static SFUN(adder_add) {
  // we want to take arguments
  const m_int a = *(m_int*)MEM(0); // `a` is in the memory, at offset 0
  const m_int b = *(m_int*)MEM(SZ_INT); // `b` is in the memory, at offset SZ_INT
  // ... because `a` has size SZ_INT

  // we now set the return value
  *(m_int*)RETURN = a + b;
  // and we're done :)
}

// this is the *main* function of the plugin
GWION_IMPORT(Adder) {
  // DECL_OB will exit the function if gwi_class_ini_fails
  // gwi is the GWion Importer
  DECL_OB(const Type, t_adder, = gwi_class_ini(gwi, "Adder", "Object")) // gwi_class_ini(gwi, name, parent)

  // let's create a static function, named `add`
  GWI_BB(gwi_func_ini(gwi, "int", "add"))         // gwi_func_ini(gwi, return_type, name)
  GWI_BB(gwi_func_arg(gwi, "int", "a"))           // gwi_func_arg(gwi, arg_type, name)
  GWI_BB(gwi_func_arg(gwi, "int", "b"))
  GWI_BB(gwi_func_end(gwi, adder_add, ae_flag_static)) // gwi_func_end(gwi, function_to_import, flag)
  // *ae_flag_static* marks the function as static


  GWI_BB(gwi_class_end(gwi)) // GWI_BB will exit the function if *gwi_class_end* fails
  return GW_OK;
}
