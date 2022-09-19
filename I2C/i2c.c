#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
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
#include "gwi.h"
#include "gwion.h"

#define I2CMSG(o) (*(M_Vector*)(o->data))
#define I2C(o) (*(int*)(o->data + SZ_INT))

static MFUN(i2c_new) {
  unsigned long supported_funcs;
  const m_str path = STRING(*(M_Object*)MEM(SZ_INT));
  if ((I2C(o) = open(path, O_RDWR)) < 0)
    handle(shred, "I2C_ERROR_OPEN");
  else if (ioctl(I2C(o), I2C_FUNCS, &supported_funcs) < 0)
    handle(shred, "I2C_NO_FUNCTIONS");
  else if (!(supported_funcs & I2C_FUNC_I2C))
    handle(shred, "I2C_UNSUPPORTED");
  I2CMSG(o) = new_m_vector(shred->info->mp, sizeof(struct i2c_msg), 0);
  *(M_Object*)RETURN = o;
}

static INSTR(i2c_add) {
  M_Object tmp = *(M_Object*)MEM(SZ_INT*3);
  M_Vector array = ARRAY(tmp);
  const m_uint len = ARRAY_LEN(array);
  uint8_t *buf = mp_calloc2(shred->info->mp, len);
  memcpy(buf, ARRAY_PTR(array), len);
  struct i2c_msg msg = {
    .addr  = (uint16_t)*(m_int*)MEM(SZ_INT),
    .flags = (uint16_t)*(m_int*)MEM(SZ_INT*2),
    .len   = len,
    .buf   = buf
  };
  m_vector_add(array, &msg);
}

ANN static void clean_vector(MemPool mp, M_Vector array) {
  for(m_uint i = 0; i < ARRAY_LEN(array); i++) {
    struct i2c_msg *msg = (struct i2c_msg*)ARRAY_PTR(array) + i * sizeof(struct i2c_msg);
    mp_free2(mp, msg->len, msg->buf);
  }
  ARRAY_LEN(array) = 0;
}

static MFUN(i2c_transfer) {
  M_Vector array = I2CMSG(o);
  struct i2c_rdwr_ioctl_data i2c_rdwr_data = { .msgs = (struct i2c_msg*)ARRAY_PTR(array), .nmsgs = ARRAY_LEN(array) };
  if (ioctl(I2C(o), I2C_RDWR, &i2c_rdwr_data) < 0)
    handle(shred, "I2C_TRANSFER");
  else clean_vector(shred->info->mp, array);
}

static DTOR(i2c_dtor) {
  if (I2C(o)) (void)close(I2C(o)); // ignoring errors on close
  clean_vector(shred->info->mp, I2CMSG(o));
  free_m_vector(shred->info->mp, I2CMSG(o));
}

GWION_IMPORT(import_i2c) {

  gwidoc(gwi, "A class to handle i2C communications");
  DECL_OB(const Type, t_i2c, = gwi_class_ini(gwi, "I2C", "Object"));
  t_i2c->nspc->offset += SZ_INT + sizeof(int);
  SET_FLAG(t_i2c, final);
  gwi_class_xtor(gwi, NULL, i2c_dtor);

    gwidoc(gwi, "constructor for the class, requires a path to the i2c file");
    GWI_BB(gwi_func_ini(gwi, "auto", "new"));
    GWI_BB(gwi_func_arg(gwi, "string", "path"));
    GWI_BB(gwi_func_end(gwi, i2c_new, ae_flag_none));

    gwidoc(gwi, "add a message to the queue");
    GWI_BB(gwi_func_ini(gwi, "void", "add"));
    GWI_BB(gwi_func_arg(gwi, "int",  "addr"));
    GWI_BB(gwi_func_arg(gwi, "int",  "flag"));
    GWI_BB(gwi_func_arg(gwi, "u8[]", "data"));
    GWI_BB(gwi_func_end(gwi, i2c_add, ae_flag_none));

    gwidoc(gwi, "send messages in the queue to the file");
    gwinote(gwi, "the internal queue is cleared after sending");
    GWI_BB(gwi_func_ini(gwi, "void", "transfer"));
    GWI_BB(gwi_func_end(gwi, i2c_transfer, ae_flag_none));

  CHECK_BB(gwi_class_end(gwi));

  return GW_OK;
}
