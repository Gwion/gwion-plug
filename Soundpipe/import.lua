function declare_c_param(param, offset)
  local increment = "SZ_INT"
  if string.match(param.type, "int") then
    print("  m_int "..param.name.." = *(m_int*)(shred->mem + gw_offset);")
  elseif string.match(param.type, "SPFLOAT$") then
    print("  m_float "..param.name.." = *(m_float*)(shred->mem + gw_offset);")
    increment = "SZ_FLOAT"
  elseif string.match(param.type, "SPFLOAT*") then
    print("  m_float "..param.name.." = *(m_float*)(shred->mem + gw_offset);")
    increment = "SZ_FLOAT"
  elseif string.match(param.type, "char%s*") then
    print("  M_Object "..param.name.."_obj = *(M_Object*)(shred->mem + gw_offset);")
    print("  m_str "..param.name.." = STRING("..param.name.."_obj);")
    print("  release("..param.name.."_obj, shred);")
  elseif string.match(param.type, "sp_ftbl%s%*%*") then
    print("  M_Object "..param.name.."_ptr = *(M_Object*)(shred->mem + gw_offset);")
    print("  m_uint "..param.name.."_iter;")
    print("  sp_ftbl** "..param.name.." = (sp_ftbl**)xmalloc(m_vector_size(ARRAY("..param.name.."_ptr)) * SZ_INT);")
    print("  for("..param.name.."_iter = 0; "..param.name.."_iter < m_vector_size(ARRAY("..param.name.."_ptr)); "..param.name.."_iter++) {")
    print("      M_Object "..param.name.."_ftl_obj;")
    print("      m_vector_get(ARRAY("..param.name.."_ptr), "..param.name.."_iter, &"..param.name.."_ftl_obj);")
    print("      "..param.name.."["..param.name.."_iter] = FTBL("..param.name.."_ftl_obj);\n  }")
--    print("  REM_REF("..param.name.."_ptr->type_ref);")
    print("  release("..param.name.."_ptr, shred);")
  elseif string.match(param.type, "&sp_ftbl%s*") then
    print("  M_Object "..param.name.."_obj = *(M_Object*)(shred->mem + gw_offset);")
    print("  sp_ftbl** "..param.name.." = &FTBL("..param.name.."_obj);")
    print("  release("..param.name.."_obj, shred);")
  elseif string.match(param.type, "sp_ftbl%s*") then
    print("  const M_Object "..param.name.."_obj = *(M_Object*)(shred->mem + gw_offset);")
    print("  sp_ftbl* "..param.name.." = FTBL("..param.name.."_obj);")
    print("  release("..param.name.."_obj, shred);")
  else
    print("unknown type:", param.type, ".")
    os.exit(1)
  end
  if offset == false then
    print("  gw_offset +="..increment..";")
  end
end

function declare_gw_param(param)
  if string.match(param.type, "int") then
    print("     gwi_func_arg(gwi, \"int\", \""..param.name.."\");")
  elseif string.match(param.type, "SPFLOAT") then
    print("     gwi_func_arg(gwi, \"float\", \""..param.name.."\");")
  elseif string.match(param.type, "char*") then
    print("     gwi_func_arg(gwi, \"string\", \""..param.name.."\");")
  elseif string.match(param.type, "sp_ftbl%s%*%*") then
    print("     gwi_func_arg(gwi, \"ftbl[]\", \""..param.name.."\");")
  elseif string.match(param.type, "sp_ftbl%*%*") then
    print("     gwi_func_arg(gwi, \"ftbl[]\", \""..param.name.."\");")
  elseif string.match(param.type, "sp_ftbl%s%*") then
    print("     gwi_func_arg(gwi, \"ftbl\", \""..param.name.."\");")
  elseif string.match(param.type, "sp_ftbl%*") then
    print("     gwi_func_arg(gwi, \"ftbl\", \""..param.name.."\");")
  else print("unknown "..param.type)
    os.exit(1);
  end
  -- make_doc("    arg", param)
end

function print_gen_func(name, func)
  print("static MFUN(ftbl_"..name..") {")
  print("  sp_ftbl* ftbl = FTBL(o);")
  if(func.params ~= nil) then
    print("  m_uint gw_offset = SZ_INT*2;")
  end
  print("  if(FTBL(o))\n    sp_ftbl_destroy(&ftbl);")
  print("  m_int size = *(m_int*)(shred->mem + SZ_INT);")
  local args = "";
  if(func.params ~= nil) then
    for i, v in pairs(func.params) do
      --		while func.params[i]  do
      declare_c_param(v, i == #func.params)
      args =	string.format("%s, %s", args , v.name)
    end
  end
  print("  CHECK_SIZE(size);")
  print("  sp_ftbl_create(sp, &ftbl, size);")
  print("  sp_"..name.."(sp, ftbl"..args..");")
  print("  FTBL(o) = ftbl;")
  --	print("error:\n  sp_ftbl_destroy(&ftbl);")
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
    print("  m_bool is_init;")
    local tbl = mod.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("  sp_ftbl** "..v.name..";\n")
        end
      end
    end
  end
  print("} GW_"..name..";\n")
  print("static TICK("..name.."_tick) {")
  print("  const GW_"..name.."* ug = (GW_"..name.."*)u->module.gen.data;")
  if(nmandatory > 0) then
    print("  if(!ug->is_init) { // LCOV_EXCL_START\n    u->out = 0;\n    return;\n  } // LCOV_EXCL_STOP")
  end
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
  print("\n}\n")
  print("static CTOR("..name.."_ctor) {\n  GW_"..name.."* ug = (GW_"..name.."*)xcalloc(1, sizeof(GW_"..name.."));")
  print("  ug->sp = sp;")
  if(nmandatory > 0) then
    print("  ug->is_init = 0;")
    print("  ug->osc = NULL;")
  else
    --[=====[
    -- check errors
    print("  if(sp_"..name.."_create(&ug->osc) == SP_NOT_OK) {\n    xfree(ug);\n    Except(shred);\n  }")
    print("  if(sp_"..name.."_init(ug->sp, ug->osc) == SP_NOT_OK) {\n    xfree(ug);\n    Except(shred);\n  }")
    --]=====]
    print("  sp_"..name.."_create(&ug->osc);")
    print("  sp_"..name.."_init(ug->sp, ug->osc);")
  end
  print("  ugen_ini(shred->info->vm->gwion, UGEN(o), "..mod.ninputs..", "..mod.noutputs..");")
  print("  ugen_gen(shred->info->vm->gwion, UGEN(o), "..name.."_tick, ug, "..ntrig..");")
  print("}\n")
  print("static DTOR("..name.."_dtor) {\n  GW_"..name.."* ug = UGEN(o)->module.gen.data;")
  if(nmandatory > 0) then
    print("  if(ug->is_init) {\n")
    local  arg = mod.params.mandatory
    if arg then
      for _, v in pairs(arg) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("    xfree(ug->osc->"..v.name..");\n")
        end
      end
      print("    sp_"..name.."_destroy(&ug->osc);\n  }")
    end
  else
    print("  sp_"..name.."_destroy(&ug->osc);")
  end
  print("  xfree(ug);");
  print("}\n")
  if nmandatory > 0 then
    print("static MFUN("..name.."_init) {")
  if nmandatory > 1 then
    print("  m_uint gw_offset = SZ_INT;")
  else
    print("  const m_uint gw_offset = SZ_INT;")
end
    print("  GW_"..name.."* ug = (GW_"..name.."*)UGEN(o)->module.gen.data;")
    local args = ""
    local tbl = mod.params.mandatory
    print("  if(ug->osc) {\n    sp_"..name.."_destroy(&ug->osc);")
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("    xfree(ug->"..v.name..");")
        end
      end
    end
    print("    ug->osc = NULL;\n  }");
    if tbl then
      for i, v in pairs(tbl) do
        declare_c_param(v, i == #tbl)
        if string.match(args, "^$") then
          args = v.name
        else
          args = string.format("%s, %s", args, v.name)
        end
      end
    end
    local tbl = mod.params.mandatory
    print("  if(sp_"..name.."_create(&ug->osc) == SP_NOT_OK || sp_"..name.."_init(ug->sp, ug->osc, "..args..") == SP_NOT_OK) {")
    local tbl = mod.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("    xfree("..v.name.."); // LCOV_EXCL_LINE")
        end
      end
    end
    print("    Except(shred, \"UgenCreateException\") // LCOV_EXCL_LINE\n  }")
    local tbl = mod.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "sp_ftbl%s%*%*") then
          print("  ug->"..v.name.." = "..v.name..";")
        end
      end
    end
    print("  ug->is_init = 1;\n}\n")
  end
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
      print("  const m_uint gw_offset = SZ_INT;")
      print("  const GW_"..name.."* ug = (GW_"..name.."*)UGEN(o)->module.gen.data;")
      declare_c_param(v, true)
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

--local dir = io.popen("dir "..arg[1])
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
#include "object.h"\
#include "gwion.h"\
#include "operator.h"\
#include "import.h"\
#include "ugen.h"\
#include "gwi.h"\
#include "array.h")

--print("m_uint o_ftbl_data;")
--print("#define FTBL(o) *((sp_ftbl**)((M_Object)o)->data + o_ftbl_data)")
print("#define FTBL(o) *((sp_ftbl**)((M_Object)o)->data)")
print("#define CHECK_SIZE(size)  if(size <= 0){fprintf(stderr, \"'gen_ftbl' size argument must be more than 0\");return;}")
print("\n/*static*/ sp_data* sp;")
print("__attribute__((destructor)) static void sp_end(void) {sp_destroy(&sp);}")
print("static DTOR(sp_dtor) {\n  sp_destroy(&sp);\n}")

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
print("GWION_IMPORT(soundpipe) {\n")
print("  VM* vm = gwi_vm(gwi);")
print("  const uint8_t nchan = vm->bbq->si->out;")
print("  sp_createn(&sp, nchan);")
print("  sp->sr = vm->bbq->si->sr;")
--print("  M_Object o = new_M_UGen(gwi->gwion->mp);")
--print("  ugen_ini(gwi->gwion->mp, UGEN(o), 1, 1);")
--print("  ugen_gen(gwi->gwion->mp, UGEN(o), sp_tick, sp, 0);")
--print("  vector_add(&vm->ugen, (vtype)UGEN(o));")
--print("  gwi_item_ini(gwi, \"UGen\", \"@soundpipe main ugen\");")
--print("  gwi_item_end(gwi, ae_flag_ref | ae_flag_const, o);")
--print("  gw_connect(UGEN(o), UGEN(vm->dac));")
--print("  ugen_connect(UGEN(o), UGEN(vector_at(&vm->ugen, 0)));")
print("  CHECK_BB(gwi_class_ini(gwi, \"ftbl\", NULL))")
print("  gwi_class_xtor(gwi, NULL, ftbl_dtor);")
print("  CHECK_BB(gwi_item_ini(gwi, \"@internal\", \"@ftbl\"))")
print("  gwi_item_end(gwi, 0, NULL);")
for n in ipairs(a) do
  local gen_name = a[n]
  local object = sptbl[gen_name]
  if string.match(object.modtype, "gen") then
    print("  gwi_func_ini(gwi, \"void\", \""..gen_name.."\");")
    local i = 1; -- why this 'i' ?
    print("  gwi_func_arg(gwi, \"int\", \"size\");")
    if(object.params ~= nil) then
      while object.params[i]  do
        declare_gw_param(object.params[i])
        i = i+1
      end
    end
    print("  CHECK_BB(gwi_func_end(gwi, ftbl_"..gen_name..", ae_flag_none))")
  end
end
print("  CHECK_BB(gwi_class_end(gwi))\n")

for n in ipairs(a) do
  local mod_name = a[n]
  local object = sptbl[mod_name]
  if not string.match(object.modtype, "gen") and not string.match(mod_name, "foo")then
    local title = string.format("%s%s", string.upper(mod_name:sub(1, 1)), string.sub(mod_name, 2))
    print("  CHECK_BB(gwi_class_ini(gwi, \""..mod_name.."\", \"UGen\"))")
    print("  gwi_class_xtor(gwi, "..mod_name.."_ctor, "..mod_name.."_dtor);")
    local nmandatory = 0
    local tbl = object.params.mandatory
    if tbl then
      for _, v in pairs(tbl) do
        nmandatory = nmandatory + 1
      end
    end
    if nmandatory > 0 then
      print("  gwi_func_ini(gwi, \"void\", \"init\");")
      local tbl = object.params.mandatory
      if tbl then
        for _, v in pairs(tbl) do
          declare_gw_param(v)
        end
      end
      print("  CHECK_BB(gwi_func_end(gwi, "..mod_name.."_init, ae_flag_none))")
    end
    local tbl = object.params.optional
    if tbl then
      for _, v in pairs(tbl) do
        if string.match(v.type, "int") then
          print("  gwi_func_ini(gwi, \"int\", \""..v.name.."\");")
        elseif string.match(v.type, "SPFLOAT") then
          print("  gwi_func_ini(gwi, \"float\", \""..v.name.."\");")
        elseif string.match(v.type, "char") then
          print("  gwi_func_ini(gwi, \"string\", \""..v.name.."\");")
        elseif string.match(v.type, "sp_ftbl%s%*%*") then
          print("  gwi_func_ini(gwi, \"ftbl[]\", \""..v.name.."\");")
        elseif string.match(v.type, "sp_ftbl%s%*") then
          print("  gwi_func_ini(gwi, \"ftbl\", \""..v.name.."\");")
        end
        print("  CHECK_BB(gwi_func_end(gwi, "..mod_name.."_get_"..v.name..", ae_flag_none))")
        if string.match(v.type, "int") then
          print("  gwi_func_ini(gwi, \"int\", \""..v.name.."\");")
        elseif string.match(v.type, "SPFLOAT") then
          print("  gwi_func_ini(gwi, \"float\", \""..v.name.."\");")
        elseif string.match(v.type, "char") then
          print("  gwi_func_ini(gwi, \"string\", \""..v.name.."\");")
        elseif string.match(v.type, "sp_ftbl%s%*%*") then
          print("  gwi_func_ini(gwi, \"ftbl[]\", \""..v.name.."\");")
        elseif string.match(v.type, "sp_ftbl%s%*") then
          print("  gwi_func_ini(gwi, \"ftbl\", \""..v.name.."\");")
        end
        declare_gw_param(v)
        print("  CHECK_BB(gwi_func_end(gwi, "..mod_name.."_set_"..v.name..", ae_flag_none))")
      end
    end
    print("  CHECK_BB(gwi_class_end(gwi))\n")
  end
end
--print("  CHECK_BB(import_modules(gwi))")
print("  return 1;\n}")
