#ifdef __cplusplus
#include <utility>
#define SWIG_STD_MOVE(i) std::move(i)
#define private public
#define protected public
#endif
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

#include "libi2c/include/i2c/i2c.h"

#define GW_I2CDevice(o) ((I2CDevice*const)(o->data))

static MFUN(gw_I2CDevice_bus_set) {
  int arg2 = (int)*(m_int*)MEM(SZ_INT);
  GW_I2CDevice(o)->bus = arg2;
}

static MFUN(gw_I2CDevice_bus_get) {
  *(m_int*)RETURN = GW_I2CDevice(o)->bus;
}

static MFUN(gw_I2CDevice_addr_set) {
  GW_I2CDevice(o)->addr = *(m_int*)(MEM(SZ_INT));
}

static MFUN(gw_I2CDevice_addr_get) {
  *(m_int*)RETURN = GW_I2CDevice(o)->addr;
}

static MFUN(gw_I2CDevice_tenbit_set) {
  GW_I2CDevice(o)->tenbit = *(m_int*)MEM(SZ_INT);
}

static MFUN(gw_I2CDevice_tenbit_get) {
  *(m_int*)RETURN = GW_I2CDevice(o)->tenbit;
}

static MFUN(gw_I2CDevice_delay_set) {
  GW_I2CDevice(o)->delay = *(m_int*)MEM(SZ_INT);
}

static MFUN(gw_I2CDevice_delay_get) {
  *(m_int*)RETURN = GW_I2CDevice(o)->delay;
}

static MFUN(gw_I2CDevice_flags_set) {
  GW_I2CDevice(o)->flags = *(m_int*)MEM(SZ_INT);
}

static MFUN(gw_I2CDevice_flags_get) {
  *(m_int*)RETURN = GW_I2CDevice(o)->flags;
}

static MFUN(gw_I2CDevice_page_bytes_set) {
  GW_I2CDevice(o)->page_bytes = *(m_int*)MEM(SZ_INT);
}

static MFUN(gw_I2CDevice_page_bytes_get) {
  *(m_int*)RETURN = GW_I2CDevice(o)->page_bytes;
}

static MFUN(gw_I2CDevice_iaddr_bytes_set) {
  GW_I2CDevice(o)->iaddr_bytes = *(m_int*)MEM(SZ_INT);
}

static MFUN(gw_I2CDevice_iaddr_bytes_get) {
  *(m_int*)RETURN = GW_I2CDevice(o)->iaddr_bytes;
}

static DTOR(gw_i2c_dtor) {
  i2c_close(GW_I2CDevice(o)->bus);
}

static MFUN(gw_i2c_ctor) {
  const M_Object bus_name = *(M_Object*)MEM(SZ_INT);
  int bus;
  if ((bus = i2c_open(STRING(bus_name))) == -1) {
    xfun_handle(shred, "I2CBusError");
    return;
  }
  I2CDevice *dev = GW_I2CDevice(o);
  i2c_init_device(dev); // optimize call out
  dev->bus = bus;
  dev->addr = (unsigned short) (*(m_int*)MEM(SZ_INT*2));
  dev->iaddr_bytes = (unsigned int)*(m_int*)MEM(SZ_INT*3);
  dev->page_bytes = (unsigned int)*(m_int*)MEM(SZ_INT*4);
  *(M_Object*)RETURN = o;
}
/*
static MFUN(gw_i2c_get_device_desc) {
  I2CDevice * arg1 = GW_I2CDevice(o);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  size_t arg3 = (size_t)*(m_int*)MEM(0+SZ_INT+SZ_INT);
  char * result = (char *)i2c_get_device_desc((struct i2c_device const *)arg1,arg2,arg3);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, (m_str)result ?: (m_str)"");
}

static MFUN(gw_i2c_select) {
  int arg1 = (int)*(m_int*)MEM(0);
  unsigned long arg2 = *(m_int*)MEM(SZ_INT);
  unsigned long arg3 = *(m_int*)MEM(SZ_INT*3);
  int result = (int)i2c_select(arg1,arg2,arg3);
  *(m_int*)RETURN = (m_int)result;
}
*/

static MFUN(gw_i2c_read1) {
  unsigned int iaddr = *(m_int*)MEM(SZ_INT);
  uint8_t value;
  if(i2c_read(GW_I2CDevice(o), iaddr, &value, 1) >= 0)
    *(m_int*)RETURN = value;
  else xfun_handle(shred, "I2CReadError");
}

static MFUN(gw_i2c_read) {
  const VM_Code code = *(VM_Code*)REG(SZ_INT*3);
  unsigned int iaddr = *(m_int*)MEM(SZ_INT);
  const size_t len = *(m_int*)MEM(SZ_INT*2);
  const M_Object array = new_array(shred->info->mp, code->ret_type, len);
  *(M_Object*)RETURN = array;
  void *buf = ARRAY(array);
  if(i2c_read(GW_I2CDevice(o), iaddr, &buf, len) < 0)
    xfun_handle(shred, "I2CReadError");
}

static MFUN(gw_i2c_write1) {
  unsigned int iaddr = (unsigned int)*(m_int*)MEM(SZ_INT);
  if(i2c_write(GW_I2CDevice(o), iaddr, (void const*)MEM(SZ_INT*2), 1) < 0)
    xfun_handle(shred, "I2CWriteError");
}

static MFUN(gw_i2c_write) {
  unsigned int iaddr = (unsigned int)*(m_int*)MEM(SZ_INT);
  const M_Vector array = ARRAY(*(M_Object*)MEM(SZ_INT*2));
  if(i2c_write(GW_I2CDevice(o), iaddr,(void const *)ARRAY_PTR(array), ARRAY_LEN(array)) < 0)
    xfun_handle(shred, "I2CWriteError");
}

static MFUN(gw_i2c_ioctl_read1) {
  unsigned int iaddr = *(m_int*)MEM(SZ_INT);
  uint8_t value;
  if(i2c_ioctl_read(GW_I2CDevice(o), iaddr, &value, 1) < 0)
    xfun_handle(shred, "I2CReadError");
  *(m_int*)RETURN = value;
}

static MFUN(gw_i2c_ioctl_read) {
  const VM_Code code = *(VM_Code*)REG(SZ_INT*3);
  unsigned int iaddr = *(m_int*)MEM(SZ_INT);
  const size_t len = *(m_int*)MEM(SZ_INT*2);
  const M_Object array = new_array(shred->info->mp, code->ret_type, len);
  *(M_Object*)RETURN = array;
  void *buf = ARRAY(array);
  if(i2c_ioctl_read(GW_I2CDevice(o), iaddr, &buf, len) < 0)
    xfun_handle(shred, "I2CReadError");
}

static MFUN(gw_i2c_ioctl_write1) {
  unsigned int iaddr = *(m_int*)MEM(SZ_INT);
  if(i2c_ioctl_write(GW_I2CDevice(o), iaddr,(void const*)MEM(SZ_INT*2), 1) < 0)
    xfun_handle(shred, "I2CWriteError");
}

static MFUN(gw_i2c_ioctl_write) {
  unsigned int iaddr = *(m_int*)MEM(SZ_INT);
  const M_Vector array = ARRAY(*(M_Object*)MEM(SZ_INT*2));
  if(i2c_ioctl_write(GW_I2CDevice(o), iaddr,(void const*)ARRAY_PTR(array), ARRAY_LEN(array)) < 0)
    xfun_handle(shred, "I2CWriteError");
}

GWION_IMPORT(I2C) {
  const Type t_I2CDevice = gwi_class_ini(gwi, "I2C", "Object");
  gwi_class_xtor(gwi, NULL, gw_i2c_dtor);
  t_I2CDevice->nspc->offset += sizeof(I2CDevice);
  CHECK_BB(gwi_func_ini(gwi, "void", "bus"));
  CHECK_BB(gwi_func_arg(gwi, "int", "bus"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_bus_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "bus"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_bus_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "addr"));
  CHECK_BB(gwi_func_arg(gwi, "int", "addr"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_addr_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "addr"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_addr_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "addr_tenbit"));
  CHECK_BB(gwi_func_arg(gwi, "int", "tenbit"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_tenbit_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "addr_tenbit"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_tenbit_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "delay"));
  CHECK_BB(gwi_func_arg(gwi, "int", "delay"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_delay_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "delay"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_delay_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "flags"));
  CHECK_BB(gwi_func_arg(gwi, "int", "flags"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_flags_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "flags"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_flags_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "page_bytes"));
  CHECK_BB(gwi_func_arg(gwi, "int", "page_bytes"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_page_bytes_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "page_bytes"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_page_bytes_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "iaddr_bytes"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr_bytes"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_iaddr_bytes_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "iaddr_bytes"));
  CHECK_BB(gwi_func_end(gwi, gw_I2CDevice_iaddr_bytes_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "auto", "new"));
  CHECK_BB(gwi_func_arg(gwi, "string", "bus"));
  CHECK_BB(gwi_func_arg(gwi, "int", "addr"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr_bytes"));
  CHECK_BB(gwi_func_arg(gwi, "int", "page_bytes"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_ctor, ae_flag_none));
//  CHECK_BB(gwi_func_ini(gwi, "string", "get_device_desc"));
//  CHECK_BB(gwi_func_arg(gwi, "string", "buf"));
//  CHECK_BB(gwi_func_arg(gwi, "int", "size"));
//  CHECK_BB(gwi_func_end(gwi, gw_i2c_get_device_desc, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "u8", "read"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_read1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "u8[]", "read"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr"));
  CHECK_BB(gwi_func_arg(gwi, "int", "len"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_read, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "write"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr"));
  CHECK_BB(gwi_func_arg(gwi, "u8", "value"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_write1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "write"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr"));
  CHECK_BB(gwi_func_arg(gwi, "u8[]", "buf"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_write, ae_flag_none));
/*
  CHECK_BB(gwi_func_ini(gwi, "int", "select"));
  CHECK_BB(gwi_func_arg(gwi, "int", "dev_addr"));
  CHECK_BB(gwi_func_arg(gwi, "int", "tenbit"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_select, ae_flag_static));
*/
  gwidoc(gwi, "for I2Cs using ioctl");
  GWI_OB(gwi_class_ini(gwi, "Ioctl", "I2C"));
  CHECK_BB(gwi_func_ini(gwi, "u8", "read"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_ioctl_read1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "u8[]", "read"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr"));
  CHECK_BB(gwi_func_arg(gwi, "int", "len"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_ioctl_read, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "write"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr"));
  CHECK_BB(gwi_func_arg(gwi, "u8", "value"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_ioctl_write1, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "write1"));
  CHECK_BB(gwi_func_arg(gwi, "int", "iaddr"));
  CHECK_BB(gwi_func_arg(gwi, "u8[]", "buf"));
  CHECK_BB(gwi_func_end(gwi, gw_i2c_ioctl_write, ae_flag_none));
  gwi_class_end(gwi);
  gwi_class_end(gwi);
  return GW_OK;
}
