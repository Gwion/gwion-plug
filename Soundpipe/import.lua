function declare_c_param(param, offset)
  if string.match(param.type, "int") then
    print("  m_int "..param.name.." = *(m_int*)(shred->mem+"..offset..");")
  elseif string.match(param.type, "SPFLOAT$") then
    print("  m_float "..param.name.." = *(m_float*)(shred->mem+"..offset..");")
  elseif string.match(param.type, "SPFLOAT*") then
    print("  m_float "..param.name.." = *(m_float*)(shred->mem+"..offset..");")
  elseif string.match(param.type, "char%s*") then
    print("  M_Object "..param.name.."_obj = *(M_Object*)(shred->mem+"..offset..");")
    print("  m_str "..param.name.." = STRING("..param.name.."_obj);")
  elseif string.match(param.type, "sp_ftbl%s%*%*") then
    print("  M_Object "..param.name.."_ptr = *(M_Object*)(shred->mem+"..offset..");")
    print("  m_uint "..param.name.."_iter;")
    print("  sp_ftbl** "..param.name.." = (sp_ftbl**)xmalloc(m_vector_size(ARRAY("..param.name.."_ptr)) * SZ_INT);")
    print("  for("..param.name.."_iter = 0; "..param.name.."_iter < m_vector_size(ARRAY("..param.name.."_ptr)); "..param.name.."_iter++) {")
    print("      M_Object "..param.name.."_ftl_obj;")
    print("      m_vector_get(ARRAY("..param.name.."_ptr), "..param.name.."_iter, &"..param.name.."_ftl_obj);")
    print("      "..param.name.."["..param.name.."_iter] = FTBL("..param.name.."_ftl_obj);\n  }")
    print("  ++"..param.name.."_ptr->ref;")
  elseif string.match(param.type, "&sp_ftbl%s*") then
    print("  M_Object "..param.name.."_obj = *(M_Object*)(shred->mem+"..offset..");")
    print("  sp_ftbl** "..param.name.." = &FTBL("..param.name.."_obj);")
  elseif string.match(param.type, "sp_ftbl%s*") then
    print("  const M_Object "..param.name.."_obj = *(M_Object*)(shred->mem+"..offset..");")
    print("  sp_ftbl* "..param.name.." = FTBL("..param.name.."_obj);")
    print("  ++"..param.name.."_obj->ref;")
  else
    print("unknown type:", param.type, ".")
    os.exit(1)
  end
end

function mkdoc(param) 
  if param.description then
    description = param.description:gmatch("([^\n]*)\n?")
    for desc in description do
      d = desc:gsub("\"", "`");
      if d:len() > 0
      then print("     gwinote(gwi, \""..d.."\");")
      end
    end
  end
end

function declare_gw_param(param)
  mkdoc(param)
  local pname = "_"..param.name
  if string.match(param.type, "int") then
    print("     gwi_func_arg(gwi, \"int\", \""..pname.."\");")
  elseif string.match(param.type, "SPFLOAT") then
    print("     gwi_func_arg(gwi, \"float\", \""..pname.."\");")
  elseif string.match(param.type, "char*") then
    print("     gwi_func_arg(gwi, \"string\", \""..pname.."\");")
  elseif string.match(param.type, "sp_ftbl%s%*%*") then
    print("     gwi_func_arg(gwi, \"ftbl[]\", \""..pname.."\");")
  elseif string.match(param.type, "sp_ftbl%*%*") then
    print("     gwi_func_arg(gwi, \"ftbl[]\", \""..pname.."\");")
  elseif string.match(param.type, "sp_ftbl%s%*") then
    print("     gwi_func_arg(gwi, \"ftbl\", \""..pname.."\");")
  elseif string.match(param.type, "sp_ftbl%*") then
    print("     gwi_func_arg(gwi, \"ftbl\", \""..pname.."\");")
  else print("unknown "..param.type)
    os.exit(1);
  end
end

function print_gen_func(name, func)
  print("static MFUN(ftbl_"..name..") {")
  print("  sp_ftbl* ftbl = FTBL(o);")
  print("  if(FTBL(o))\n    sp_ftbl_destroy(&ftbl);")
  print("  m_int size = *(m_int*)(shred->mem + SZ_INT);")
  local args = "";
  if(func.params ~= nil) then
    local offset = "SZ_INT*2"
    for i, v in pairs(func.params) do
      declare_c_param(v, offset)
      if string.match(v.type, "SPFLOAT") or string.match(v.type, "SP_FLOAT$") or string.match(v.type, "SP_FLOAT*") then
           offset = offset.."+SZ_FLOAT"
      else offset = offset.."+SZ_INT"
      end
      args =	string.format("%s, %s", args , v.name)
    end
  end
  print("  CHECK_SIZE(size);")
  print("  sp_data *sp = get_module(shred->info->vm->gwion, \"Soundpipe\");")
  print("  sp_ftbl_create(sp, &ftbl, size);")
  print("  sp_"..name.."(sp, ftbl"..args..");")
  print("  FTBL(o) = ftbl;")
  print("}\n")
end

function print_mod_func(name, mod)
  if string.match(name, "^foo$") then
    return
  end
  local ntrig = 0
  local ninputs = mod.ninputs
  local nmandatory = 0
  local tbl = mod.params.mandatory
  if tbl then
    for _, v in pairs(tbl) do
      nmandatory = nmandatory + 1
    end
  end
  for i = 1, ninputs  do
    if string.match(mod.inputs[i].name, "trig") then
      ntrig = ntrig + 1
      ninputs  = ninputs - 1
    end
  end
  print("typedef struct {\n  sp_data* sp;\n  sp_"..name.."* osc;")
  if(nmandatory > 0) then
    local tbl = mod.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("  sp_ftbl** "..v.name..";\n")
          print("  M_Object "..v.name.."_ptr;\n")
        else if string.match(v.type, "sp_ftbl%s%*") then
          print("  M_Object "..v.name.."_obj;\n")
          end
        end
      end
    end
  end
  print("} GW_"..name..";\n")
  print("static TICK("..name.."_tick) {")
  print("  const GW_"..name.."* ug = (GW_"..name.."*)u->module.gen.data;")
  local args = ""
  if ninputs > 1 or mod.noutputs > 1 then
    for i = 1, ninputs do
      args = string.format("%s, &UGEN(u->connect.multi->channel["..(i - 1).."])->in", args)
    end
  elseif ninputs == 1 then
    args = string.format("%s, &u->in", args)
  elseif ntrig == 0 and not string.match(name, "voc") then
    args = ", NULL"
  end
  if ntrig > 0 then
    args = string.format("%s, &u->module.gen.trig->in", args)
  end
  if mod.noutputs > 1 then
    for i = 1, mod.noutputs do
      args = string.format("%s, &UGEN(u->connect.multi->channel["..(i - 1).."])->out", args)
    end
  elseif mod.noutputs == 1 then
    args = string.format("%s, &u->out", args)
  end
  print("  sp_"..name.."_compute(ug->sp, ug->osc"..args..");")
  print("}\n")
  print("static DTOR("..name.."_dtor) {\n  GW_"..name.."* ug = UGEN(o)->module.gen.data;")
  if(nmandatory > 0) then
    local  arg = mod.params.mandatory
    if arg then
      for _, v in pairs(arg) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("  xfree(ug->osc->"..v.name..");\n")
          print("  release(ug->"..v.name.."_ptr, shred);\n")
        else if string.match(v.type, "sp_ftbl%s%*") then
          print("  release(ug->"..v.name.."_obj, shred);\n")
          end
        end
      end
      print("  sp_"..name.."_destroy(&ug->osc);")
    end
  else
    print("  sp_"..name.."_destroy(&ug->osc);")
  end
  print("  mp_free2(shred->info->mp, sizeof(GW_"..name.."), ug);");
  print("}\n")
    print("static MFUN("..name.."_init) {")
    print("  GW_"..name.."* ug = mp_malloc2(shred->info->mp, sizeof(GW_"..name.."));")
    print("  ugen_ini(shred->info->vm->gwion, UGEN(o), "..mod.ninputs..", "..mod.noutputs..");")
    print("  ugen_gen(shred->info->vm->gwion, UGEN(o), "..name.."_tick, ug, "..ntrig..");")
    local args = ""
    local tbl = mod.params.mandatory
    if tbl then
      args = " "
      local offset = "SZ_INT"
      for i, v in pairs(tbl) do
        declare_c_param(v, offset)
        if string.match(v.type, "SPFLOAT") or string.match(v.type, "SP_FLOAT$") or string.match(v.type, "SP_FLOAT*") then
             offset = offset.."+SZ_FLOAT"
        else offset = offset.."+SZ_INT"
        end
        if string.match(args, "^$") then
          args = v.name
        else
          args = string.format("%s, %s", args, v.name)
        end
      end
    end
    print("  *(M_Object*)RETURN = o;")
    local tbl = mod.params.mandatory
    print("  if(sp_"..name.."_create(&ug->osc) == SP_NOT_OK || sp_"..name.."_init(ug->sp, ug->osc"..args..") == SP_NOT_OK) {")
local offset = "SZ_INT"
    local tbl = mod.params.mandatory
if tbl then
  for _, v in pairs(tbl) do
        if string.match(v.type, "SPFLOAT$") then
offset = offset.."+SZ_FLOAT"
else offset = offset.."+SZ_INT"
end
  end
end
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("    xfree("..v.name.."); // LCOV_EXCL_LINE")
          print("    release("..v.name.."_ptr, shred); // LCOV_EXCL_LINE")
        else if string.match(v.type, "sp_ftbl%s%*") then
          print("    release("..v.name.."_obj, shred); // LCOV_EXCL_LINE")
          end
        end
      end
    end
    print("    sp_handle(shred, "..offset..", \"UgenCreateException\") // LCOV_EXCL_LINE\n  }")
    local tbl = mod.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("  ug->"..v.name.." = "..v.name..";")
          print("  ug->"..v.name.."_ptr = "..v.name.."_ptr;")
        else if string.match(v.type, "sp_ftbl%s%*") then
          print("  ug->"..v.name.."_obj = "..v.name.."_obj;")
          end
        end
      end
    end
  print("}\n")
  local opt = mod.params.optional
  if opt then
    for _, v in pairs(opt) do
      print("static MFUN("..name.."_get_"..v.name..") {")
      print("  const GW_"..name.."* ug = (GW_"..name.."*)UGEN(o)->module.gen.data;")
      if string.match(v.type, "int") then
        print("  *(m_uint*)RETURN = ug->osc->"..v.name..";")
      elseif string.match(v.type, "SPFLOAT$") then
        print("  *(m_float*)RETURN = ug->osc->"..v.name..";")
      elseif string.gmatch(v.type, "SPFLOAT*") then
        print("  *(m_float*)RETURN = *ug->osc->"..v.name..";")
      elseif string.gmatch(v.type, "char*") then
        print("  *(m_uint*)RETURN = ug->osc->"..v.name.."_obj;")
      elseif string.gmatch(v.type, "sp_ftbl*") then
        print("  *(m_uint*)RETURN = ug->osc->"..v.name.."_obj;")
      else
        print("unknown type "..v.type)
        os.exit(1);
      end
      print("}\n")
      print("static MFUN("..name.."_set_"..v.name..") {")
      print("  const GW_"..name.."* ug = (GW_"..name.."*)UGEN(o)->module.gen.data;")
      declare_c_param(v, "SZ_INT")
      if string.match(v.type, "int") then
        print("  *(m_uint*)RETURN = (ug->osc->"..v.name.." = "..v.name..");")
      elseif string.match(v.type, "SPFLOAT$") then
        print("  *(m_float*)RETURN = (ug->osc->"..v.name.." = "..v.name..");")
      elseif string.gmatch(v.type, "SPFLOAT*") then
        print("  *(m_float*)RETURN = (*ug->osc->"..v.name.." = "..v.name..");")
      elseif string.gmatch(v.type, "char*") then
        print("  *(m_uint*)RETURN = (ug->osc->"..v.name.."_obj = "..v.name..");")
      elseif string.gmatch(v.type, "sp_ftbl*") then
        print("  *(m_uint*)RETURN = (ug->osc->"..v.name.."_obj = "..v.name..");")
      end
      print("}\n")
    end
  end
end

sptbl = {}

if not arg[1] then
  print("this script takes a valid soudpipe data directory as argument")
  os.exit(1)
end

local dir = io.popen("ls "..arg[1])
if dir then
  for filename in string.gmatch(dir:read("*a"), '([^%s]+)') do
    if filename:gmatch(".lua$") then
      dofile(arg[1]..'/'..filename)
    end
  end
else
  print("failed to read soundpipe data directory.")
  os.exit(1);
end

a = {}
for n in pairs(sptbl) do table.insert(a, n) end
table.sort(a)

print('#include <stdlib.h>\
#include <soundpipe.h>\
#include "gwion_util.h"\
#include "gwion_ast.h"\
#include "gwion_env.h"\
#include "vm.h"\
#include "err_msg.h"\
#include "instr.h"\
#include "gwion.h"\
#include "object.h"\
#include "operator.h"\
#include "import.h"\
#include "ugen.h"\
#include "gwi.h"\
#include "array.h"')

print("#define FTBL(o) *((sp_ftbl**)((M_Object)o)->data)")
print("#define CHECK_SIZE(size)  if(size <= 0){fprintf(stderr, \"'gen_ftbl' size argument must be more than 0\");return;}")
print("#define sp_handle(a,b,c) { xfun_handle(a,b,c); return; }")

print("\nstatic DTOR(ftbl_dtor) {")
print("  if(FTBL(o))\n    sp_ftbl_destroy(&FTBL(o));")
print("}\n")
for n in ipairs(a) do
  local name = a[n]
  local object = sptbl[name]
  if string.match(object.modtype, "gen") then
    print_gen_func(name, object)
  elseif not string.match(name, "foo") then
    print_mod_func(name, object)
  end
end

print("static TICK(sp_tick) {\n  ++((sp_data*)u->module.gen.data)->pos; }")
print("")
print("GWMODINI(Soundpipe) {")
print("  sp_data *sp;")
print("  sp_createn(&sp, gwion->vm->bbq->si->out);")
print("  sp->sr = gwion->vm->bbq->si->sr;")
print("  return sp;")
print("}")
print("")
print("GWMODEND(Soundpipe) {")
print("  sp_destroy((sp_data**)&self);")
print("}")
print("GWION_IMPORT(Soundpipe) {\n")
print("  VM* vm = gwi_vm(gwi);")
print("  sp_data *sp = get_module(vm->gwion, \"Soundpipe\");")
print("  if(!sp) {")
print("    sp_createn(&sp, gwi->gwion->vm->bbq->si->out); ")
print("    sp->sr = gwi->gwion->vm->bbq->si->sr;")
print("    CHECK_BB(set_module(vm->gwion, \"Soundpipe\", sp));") 
print("  }")
print("  const M_Object o = new_M_UGen(gwi->gwion);")
print("  ugen_ini(gwi->gwion, UGEN(o), 1, 1);")
print("  ugen_gen(gwi->gwion, UGEN(o), sp_tick, sp, 0);")
print("  vector_add(&vm->ugen, (vtype)UGEN(o));")
print("  gwi_item_ini(gwi, \"UGen\", \"@soundpipe main ugen\");")
print("  gwi_item_end(gwi, ae_flag_late | ae_flag_const, obj, o);")

print("  ugen_connect(UGEN(o), (UGen)vector_front(&vm->ugen));")
print("  DECL_OB(const Type, t_ftbl, = gwi_class_ini(gwi, \"ftbl\", NULL));")
print("  gwi_class_xtor(gwi, NULL, ftbl_dtor);")
print("  t_ftbl->nspc->offset += SZ_INT;")
for n in ipairs(a) do
  local gen_name = a[n]
  local object = sptbl[gen_name]
  if string.match(object.modtype, "gen") then
    print("  gwi_func_ini(gwi, \"void\", \"_"..gen_name.."\");")
    local i = 1; -- why this 'i' ?
    print("  gwi_func_arg(gwi, \"int\", \"Size\");")
    if(object.params ~= nil) then
      while object.params[i]  do
        declare_gw_param(object.params[i])
        i = i+1
      end
    end
    print("  GWI_BB(gwi_func_end(gwi, ftbl_"..gen_name..", ae_flag_none))")
  end
end
print("  GWI_BB(gwi_class_end(gwi))\n")

for n in ipairs(a) do
  local mod_name = a[n]
  local object = sptbl[mod_name]
  mkdoc(object)
  if not string.match(object.modtype, "gen") and not string.match(mod_name, "foo")then
    local title = string.format("%s%s", string.upper(mod_name:sub(1, 1)), string.sub(mod_name, 2))
    print("  DECL_OB(const Type, t_"..mod_name..", = gwi_class_ini(gwi, \""..mod_name:gsub("^%l", string.upper).."\", \"UGen\"));")
    print("  SET_FLAG(t_"..mod_name..", final);")
    print("  gwi_class_xtor(gwi, NULL, "..mod_name.."_dtor);")
    local nmandatory = 0
    local tbl = object.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        nmandatory = nmandatory + 1
      end
    end
    print("  gwi_func_ini(gwi, \"auto\", \"new\");")
    local tbl = object.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        declare_gw_param(v)
      end
    end
    print("  GWI_BB(gwi_func_end(gwi, "..mod_name.."_init, ae_flag_none))")
    local tbl = object.params.optional
    if tbl then
      for _, v in pairs(tbl) do
        local fname = v.name
        if string.match(v.type, "int") then
          print("  gwi_func_ini(gwi, \"int\", \""..fname.."\");")
        elseif string.match(v.type, "SPFLOAT") then
          print("  gwi_func_ini(gwi, \"float\", \""..fname.."\");")
        elseif string.match(v.type, "char") then
          print("  gwi_func_ini(gwi, \"string\", \""..fname.."\");")
        elseif string.match(v.type, "sp_ftbl%s%*%*") then
          print("  gwi_func_ini(gwi, \"ftbl[]\", \""..fname.."\");")
        elseif string.match(v.type, "sp_ftbl%s%*") then
          print("  gwi_func_ini(gwi, \"ftbl\", \""..fname.."\");")
        end
        print("  GWI_BB(gwi_func_end(gwi, "..mod_name.."_get_"..v.name..", ae_flag_none))")
        if string.match(v.type, "int") then
          print("  gwi_func_ini(gwi, \"int\", \""..fname.."\");")
        elseif string.match(v.type, "SPFLOAT") then
          print("  gwi_func_ini(gwi, \"float\", \""..fname.."\");")
        elseif string.match(v.type, "char") then
          print("  gwi_func_ini(gwi, \"string\", \""..fname.."\");")
        elseif string.match(v.type, "sp_ftbl%s%*%*") then
          print("  gwi_func_ini(gwi, \"ftbl[]\", \""..fname.."\");")
        elseif string.match(v.type, "sp_ftbl%s%*") then
          print("  gwi_func_ini(gwi, \"ftbl\", \""..fname.."\");")
        end
        declare_gw_param(v)
        print("  GWI_BB(gwi_func_end(gwi, "..mod_name.."_set_"..v.name..", ae_flag_none))")
      end
    end
    print("  GWI_BB(gwi_class_end(gwi))\n")
  end
end
print("  return GW_OK;\n}")
