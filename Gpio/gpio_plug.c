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

#include "periphery/gpio.h"


#include <string.h>

#define GW_gpio_config_t(o) *(void**)(o->data)
static MFUN(gw_gpio_config_t_direction_set) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  gpio_direction_t arg2 = *(gpio_direction_t*)MEM(SZ_INT);
  arg1->direction = arg2;
}

static MFUN(gw_gpio_config_t_direction_get) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  gpio_direction_t result = (gpio_direction_t) ((arg1)->direction);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_config_t_edge_set) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  gpio_edge_t arg2 = *(gpio_edge_t*)MEM(SZ_INT);
  arg1->edge = arg2;
}

static MFUN(gw_gpio_config_t_edge_get) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  gpio_edge_t result = (gpio_edge_t) ((arg1)->edge);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_config_t_bias_set) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  gpio_bias_t arg2 = *(gpio_bias_t*)MEM(SZ_INT);
  arg1->bias = arg2;
}

static MFUN(gw_gpio_config_t_bias_get) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  gpio_bias_t result = (gpio_bias_t) ((arg1)->bias);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_config_t_drive_set) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  gpio_drive_t arg2 = *(gpio_drive_t*)MEM(SZ_INT);
  arg1->drive = arg2;
}

static MFUN(gw_gpio_config_t_drive_get) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  gpio_drive_t result = (gpio_drive_t) ((arg1)->drive);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_config_t_inverted_set) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  bool arg2 = (bool)*(m_int*)MEM(SZ_INT);
  arg1->inverted = arg2;
}

static MFUN(gw_gpio_config_t_inverted_get) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  bool result = (bool) ((arg1)->inverted);
  *(m_int*)RETURN = (m_int)result;
}

// unset function?
static MFUN(gw_gpio_config_t_label_set) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  {
    if (arg2) {
      arg1->label = (char const *) malloc(strlen((const char *)arg2)+1);
      strcpy((char *)arg1->label, (const char *)arg2);
    } else {
      arg1->label = 0;
    }
  }
}

static MFUN(gw_gpio_config_t_label_get) {
  struct gpio_config * arg1 = *(struct gpio_config **)(o->data);
  char const * result = (char *) ((arg1)->label);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, (m_str)result ?: (m_str)"");
}

static MFUN(gw_gpio_new) {
  gpio_t * result = (gpio_t *)gpio_new();
  // VM_Code code = *(VM_Code*)REG(0);
  // M_Object o = new_object(shred->info->mp, code->ret_type);
  *(gpio_t **)o->data = result; // TODO: handle offset
  *(M_Object*)RETURN = o;
}

static MFUN(gw_gpio_open) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(SZ_INT*2);
  gpio_direction_t arg4 = *(gpio_direction_t*)MEM(SZ_INT*3);
  if(gpio_open(arg1,(char const *)arg2,arg3,arg4) < 0)
    handle(shred, "GpioError");
  *(M_Object*)RETURN = o;
}

static MFUN(gw_gpio_open_name) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  M_Object temp3 = *(M_Object*)MEM(SZ_INT*2);
  char * arg3 = (char *)STRING(temp3);
  gpio_direction_t arg4 = *(gpio_direction_t*)MEM(SZ_INT*3);
  if(gpio_open_name(arg1,(char const *)arg2,(char const *)arg3,arg4) < 0)
    handle(shred, "GpioError");
  *(M_Object*)RETURN = o;
}

static MFUN(gw_gpio_open_advanced) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  unsigned int arg3 = (unsigned int)*(m_int*)MEM(SZ_INT*2);
  gpio_config_t * arg4 = *(gpio_config_t **)MEM(SZ_INT*3);
  if(gpio_open_advanced(arg1,(char const *)arg2,arg3,(struct gpio_config const *)arg4) < 0)
    handle(shred, "GpioError");
  *(M_Object*)RETURN = o;
}

static MFUN(gw_gpio_open_name_advanced) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  M_Object temp3 = *(M_Object*)MEM(SZ_INT*2);
  char * arg3 = (char *)STRING(temp3);
  gpio_config_t * arg4 = *(gpio_config_t **)MEM(SZ_INT*3);
  if(gpio_open_name_advanced(arg1,(char const *)arg2,(char const *)arg3,(struct gpio_config const *)arg4) < 0)
    handle(shred, "GpioError");
  *(M_Object*)RETURN = o;
}

static MFUN(gw_gpio_open_sysfs) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  unsigned int arg2 = (unsigned int)*(m_int*)MEM(SZ_INT);
  gpio_direction_t arg3 = *(gpio_direction_t*)MEM(SZ_INT*2);
  if(gpio_open_sysfs(arg1,arg2,arg3) < 0)
    handle(shred, "GpioError");
  *(M_Object*)RETURN = o;
}

static MFUN(gw_gpio_read) {
  gpio_t * gpio = *(gpio_t **)(o->data);
  bool value;
  if(gpio_read(gpio, &value) < 0)
    handle(shred, "GpioError");
  *(m_int*)RETURN = value;
}

static MFUN(gw_gpio_write) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  bool arg2 = (bool)*(m_int*)MEM(SZ_INT);
  if(gpio_write(arg1,arg2) < 0)
    handle(shred, "GpioError");
}

#include <threads.h>
struct poll_data {
  VM_Shred shred;
  gpio_t *gpio;
  int timeout_ms;
};

static int threaded_gpio_poll(void *data) {
  struct poll_data *pd = (struct poll_data*)data;
  const VM_Shred shred = pd->shred;
  gpio_t *gpio = pd->gpio;
  const int timeout_ms = pd->timeout_ms;
  mp_free2(shred->info->mp, sizeof(struct poll_data), data);
  const m_bool ret = gpio_poll(gpio, timeout_ms);
  if(ret >= 0) shredule(shred->tick->shreduler, shred, GWION_EPSILON);
  else handle(shred, "GpioError");
  return ret;
}

static MFUN(gw_gpio_poll) {
  struct poll_data *pd = mp_calloc2(shred->info->mp, sizeof(struct poll_data));
  pd->shred = shred;
  pd->gpio = *(gpio_t **)(o->data);
  pd->timeout_ms = (int)*(m_int*)MEM(SZ_INT);
  thrd_t thrd;
  thrd_create(&thrd, threaded_gpio_poll, pd);
  thrd_detach(thrd);
}

static MFUN(gw_gpio_close) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  int result = (int)gpio_close(arg1);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_free) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  gpio_free(arg1);
}

static MFUN(gw_gpio_read_event) {
  gpio_t *gpio = *(gpio_t **)(o->data);
  gpio_edge_t edge;
  uint64_t * timeout_ms = *(uint64_t **)(MEM(SZ_INT));
  if(gpio_read_event(gpio, &edge, timeout_ms) < 0)
    handle(shred, "GpioError");
  *(m_int*)RETURN = edge;
}
/*
static MFUN(gw_gpio_poll_multiple) {
  gpio_t ** arg1 = ARRAY_PTR(o);
  size_t arg2 = (size_t)*(m_int*)MEM(SZ_INT);
  int arg3 = (int)*(m_int*)MEM(SZ_INT*2);
//  bool * arg4 = *(bool **)MEM(SZ_INT*3);
  if(gpio_poll_multiple(arg1,arg2,arg3,arg4) < 0)
    handle(shred, "GpioError");
}
*/
static MFUN(gw_gpio_get_direction) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  gpio_direction_t arg2;
  if(gpio_get_direction(arg1, &arg2) < 0)
    handle(shred, "GpioError");
  *(m_int*)RETURN = arg2;
}

static MFUN(gw_gpio_get_edge) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  gpio_edge_t arg2;
  if(gpio_get_edge(arg1, &arg2) < 0)
    handle(shred, "GpioError");
  *(m_int*)RETURN = arg2;
}

static MFUN(gw_gpio_get_bias) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  gpio_bias_t arg2;
  if(gpio_get_bias(arg1, &arg2) < 0)
    handle(shred, "GpioError");
  *(m_int*)RETURN = arg2;
}

static MFUN(gw_gpio_get_drive) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  gpio_drive_t arg2;
  if(gpio_get_drive(arg1, &arg2) < 0)
    handle(shred, "GpioError");
  *(m_int*)RETURN = arg2;
}

static MFUN(gw_gpio_get_inverted) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  bool arg2;
  if(gpio_get_inverted(arg1, &arg2) < 0)
    handle(shred, "GpioError");
  *(m_int*)RETURN = arg2;
}

static MFUN(gw_gpio_set_direction) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  gpio_direction_t arg2 = *(gpio_direction_t*)MEM(SZ_INT);
  if(gpio_set_direction(arg1,arg2) < 0)
    handle(shred, "GpioError");
}

static MFUN(gw_gpio_set_edge) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  gpio_edge_t arg2 = *(gpio_edge_t*)MEM(SZ_INT);
  if(gpio_set_edge(arg1,arg2) < 0)
    handle(shred, "GpioError");
}

static MFUN(gw_gpio_set_bias) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  gpio_bias_t arg2 = *(gpio_bias_t*)MEM(SZ_INT);
  if(gpio_set_bias(arg1,arg2) < 0)
    handle(shred, "GpioError");
}

static MFUN(gw_gpio_set_drive) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  gpio_drive_t arg2 = *(gpio_drive_t*)MEM(SZ_INT);
  if(gpio_set_drive(arg1,arg2) < 0)
    handle(shred, "GpioError");
}

static MFUN(gw_gpio_set_inverted) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  bool arg2 = (bool)*(m_int*)MEM(SZ_INT);
  if(gpio_set_inverted(arg1,arg2) < 0)
    handle(shred, "GpioError");
}

static MFUN(gw_gpio_line) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  unsigned int result = (unsigned int)gpio_line(arg1);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_fd) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  int result = (int)gpio_fd(arg1);
  *(m_int*)RETURN = (m_int)result;
}
/*
static MFUN(gw_gpio_name) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  size_t arg3 = (size_t)*(m_int*)MEM(SZ_INT*2);
  int result = (int)gpio_name(arg1,arg2,arg3);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_label) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  size_t arg3 = (size_t)*(m_int*)MEM(SZ_INT*2);
  int result = (int)gpio_label(arg1,arg2,arg3);
  *(m_int*)RETURN = (m_int)result;
}
*/
static MFUN(gw_gpio_chip_fd) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  int result = (int)gpio_chip_fd(arg1);
  *(m_int*)RETURN = (m_int)result;
}
/*
static MFUN(gw_gpio_chip_name) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  size_t arg3 = (size_t)*(m_int*)MEM(SZ_INT*2);
  int result = (int)gpio_chip_name(arg1,arg2,arg3);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_chip_label) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  size_t arg3 = (size_t)*(m_int*)MEM(SZ_INT*2);
  int result = (int)gpio_chip_label(arg1,arg2,arg3);
  *(m_int*)RETURN = (m_int)result;
}
*/
static MFUN(gw_gpio_tostring) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  M_Object temp2 = *(M_Object*)MEM(SZ_INT);
  char * arg2 = (char *)STRING(temp2);
  size_t arg3 = (size_t)*(m_int*)MEM(SZ_INT*2);
  int result = (int)gpio_tostring(arg1,arg2,arg3);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_errno) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  int result = (int)gpio_errno(arg1);
  *(m_int*)RETURN = (m_int)result;
}

static MFUN(gw_gpio_errmsg) {
  gpio_t * arg1 = *(gpio_t **)(o->data);
  char const * result = (char *)gpio_errmsg(arg1);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, (m_str)result ?: (m_str)"");
}

GWION_IMPORT(Gpio) {
  gwi_class_ini(gwi, (m_str)"Gpio", (m_str)"Object");
  CHECK_BB(gwi_enum_ini(gwi, (m_str)"Error"));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"ARG", (m_uint)GPIO_ERROR_ARG));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"OPEN", (m_uint)GPIO_ERROR_OPEN));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"NOT_FOUND", (m_uint)GPIO_ERROR_NOT_FOUND));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"QUERY", (m_uint)GPIO_ERROR_QUERY));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"CONFIGURE", (m_uint)GPIO_ERROR_CONFIGURE));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"UNSUPPORTED", (m_uint)GPIO_ERROR_UNSUPPORTED));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"INVALID_OPERATION", (m_uint)GPIO_ERROR_INVALID_OPERATION));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"IO", (m_uint)GPIO_ERROR_IO));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"CLOSE", (m_uint)GPIO_ERROR_CLOSE));
  CHECK_OB(gwi_enum_end(gwi));
  CHECK_BB(gwi_enum_ini(gwi, (m_str)"Direction"));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"IN", (m_uint)GPIO_DIR_IN));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"OUT", (m_uint)GPIO_DIR_OUT));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"OUT_LOW", (m_uint)GPIO_DIR_OUT_LOW));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"OUT_HIGH", (m_uint)GPIO_DIR_OUT_HIGH));
  CHECK_OB(gwi_enum_end(gwi));
  CHECK_BB(gwi_enum_ini(gwi, (m_str)"Edge"));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"NONE", (m_uint)GPIO_EDGE_NONE));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"RISING", (m_uint)GPIO_EDGE_RISING));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"FALLING", (m_uint)GPIO_EDGE_FALLING));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"BOTH", (m_uint)GPIO_EDGE_BOTH));
  CHECK_OB(gwi_enum_end(gwi));
  CHECK_BB(gwi_enum_ini(gwi, (m_str)"Bias"));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"DEFAULT", (m_uint)GPIO_BIAS_DEFAULT));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"PULL_UP", (m_uint)GPIO_BIAS_PULL_UP));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"PULL_DOWN", (m_uint)GPIO_BIAS_PULL_DOWN));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"DISABLE", (m_uint)GPIO_BIAS_DISABLE));
  CHECK_OB(gwi_enum_end(gwi));
  CHECK_BB(gwi_enum_ini(gwi, (m_str)"Drive"));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"DEFAULT", (m_uint)GPIO_DRIVE_DEFAULT));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"OPEN_DRAIN", (m_uint)GPIO_DRIVE_OPEN_DRAIN));
  CHECK_BB(gwi_enum_add(gwi, (m_str)"OPEN_SOURCE", (m_uint)GPIO_DRIVE_OPEN_SOURCE));
  CHECK_OB(gwi_enum_end(gwi));
  const Type t_gpio_config_t = gwi_class_ini(gwi, "Config", "Object");
  t_gpio_config_t->nspc->offset += SZ_INT;
  CHECK_BB(gwi_func_ini(gwi, "void", "direction"));
  CHECK_BB(gwi_func_arg(gwi, "Direction", "direction"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_direction_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "Direction", "direction"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_direction_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "edge"));
  CHECK_BB(gwi_func_arg(gwi, "Edge", "edge"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_edge_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "Edge", "edge"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_edge_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "bias"));
  CHECK_BB(gwi_func_arg(gwi, "Bias", "bias"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_bias_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "Bias", "bias"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_bias_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "drive"));
  CHECK_BB(gwi_func_arg(gwi, "Drive", "drive"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_drive_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "Drive", "drive"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_drive_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "inverted"));
  CHECK_BB(gwi_func_arg(gwi, "bool", "inverted"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_inverted_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "bool", "inverted"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_inverted_get, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "label"));
  CHECK_BB(gwi_func_arg(gwi, "string", "label"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_label_set, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "string", "label"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_config_t_label_get, ae_flag_none));
  CHECK_BB(gwi_class_end(gwi)); // gpio_config_t

// should be ctor?
//  CHECK_BB(gwi_func_ini(gwi, "auto", "new"));
//  CHECK_BB(gwi_func_end(gwi, gw_gpio_new, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "auto", "new"));
  CHECK_BB(gwi_func_arg(gwi, "string", "path"));
  CHECK_BB(gwi_func_arg(gwi, "int", "line"));
  CHECK_BB(gwi_func_arg(gwi, "Direction", "direction"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_open, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "auto", "new"));
  CHECK_BB(gwi_func_arg(gwi, "string", "path"));
  CHECK_BB(gwi_func_arg(gwi, "string", "name"));
  CHECK_BB(gwi_func_arg(gwi, "Direction", "direction"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_open_name, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "auto", "new"));
  CHECK_BB(gwi_func_arg(gwi, "string", "path"));
  CHECK_BB(gwi_func_arg(gwi, "int", "line"));
  CHECK_BB(gwi_func_arg(gwi, "Config", "config"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_open_advanced, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "auto", "new"));
  CHECK_BB(gwi_func_arg(gwi, "string", "path"));
  CHECK_BB(gwi_func_arg(gwi, "string", "name"));
  CHECK_BB(gwi_func_arg(gwi, "Config", "config"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_open_name_advanced, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "auto", "new"));
  CHECK_BB(gwi_func_arg(gwi, "int", "line"));
  CHECK_BB(gwi_func_arg(gwi, "Direction", "direction"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_open_sysfs, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "bool", "read"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_read, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "write"));
  CHECK_BB(gwi_func_arg(gwi, "bool", "value"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_write, ae_flag_none));
  gwidoc(gwi, "this function suspends the shred");
  CHECK_BB(gwi_func_ini(gwi, "void", "poll"));
  CHECK_BB(gwi_func_arg(gwi, "int", "timeout_ms"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_poll, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "close"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_close, ae_flag_none));

// this works only if sizeof(void*) == sizeof(uint64_t);
  CHECK_BB(gwi_func_ini(gwi, "Edge", "read_event"));
  CHECK_BB(gwi_func_arg(gwi, "&int", "timestamp"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_read_event, ae_flag_none));
/*
  CHECK_BB(gwi_func_ini(gwi, "int", "poll_multiple"));
  CHECK_BB(gwi_func_arg(gwi, "int", "count"));
  CHECK_BB(gwi_func_arg(gwi, "int", "timeout_ms"));
  CHECK_BB(gwi_func_arg(gwi, "&bool", "gpios_ready"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_poll_multiple, ae_flag_none));
*/
  CHECK_BB(gwi_func_ini(gwi, "Direction", "get_direction"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_get_direction, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "Edge", "get_edge"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_get_edge, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "Bias", "get_bias"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_get_bias, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "Drive", "get_drive"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_get_drive, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "bool", "get_inverted"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_get_inverted, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "set_direction"));
  CHECK_BB(gwi_func_arg(gwi, "Direction", "direction"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_set_direction, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "set_edge"));
  CHECK_BB(gwi_func_arg(gwi, "Edge", "edge"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_set_edge, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "set_bias"));
  CHECK_BB(gwi_func_arg(gwi, "Bias", "bias"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_set_bias, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "set_drive"));
  CHECK_BB(gwi_func_arg(gwi, "Drive", "drive"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_set_drive, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "void", "set_inverted"));
  CHECK_BB(gwi_func_arg(gwi, "bool", "inverted"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_set_inverted, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "line"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_line, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "fd"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_fd, ae_flag_none));
/*
  CHECK_BB(gwi_func_ini(gwi, "int", "name"));
  CHECK_BB(gwi_func_arg(gwi, "string", "str"));
  CHECK_BB(gwi_func_arg(gwi, "int", "len"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_name, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "label"));
  CHECK_BB(gwi_func_arg(gwi, "string", "str"));
  CHECK_BB(gwi_func_arg(gwi, "int", "len"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_label, ae_flag_none));
*/
  CHECK_BB(gwi_func_ini(gwi, "int", "chip_fd"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_chip_fd, ae_flag_none));
/*
  CHECK_BB(gwi_func_ini(gwi, "int", "chip_name"));
  CHECK_BB(gwi_func_arg(gwi, "string", "str"));
  CHECK_BB(gwi_func_arg(gwi, "int", "len"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_chip_name, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "chip_label"));
  CHECK_BB(gwi_func_arg(gwi, "string", "str"));
  CHECK_BB(gwi_func_arg(gwi, "int", "len"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_chip_label, ae_flag_none));
*/
  CHECK_BB(gwi_func_ini(gwi, "int", "tostring"));
  CHECK_BB(gwi_func_arg(gwi, "string", "str"));
  CHECK_BB(gwi_func_arg(gwi, "int", "len"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_tostring, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "int", "errno"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_errno, ae_flag_none));
  CHECK_BB(gwi_func_ini(gwi, "string", "errmsg"));
  CHECK_BB(gwi_func_end(gwi, gw_gpio_errmsg, ae_flag_none));
  gwi_class_xtor(gwi, gw_gpio_new, gw_gpio_free);
  gwi_class_end(gwi);
  return GW_OK;
}
