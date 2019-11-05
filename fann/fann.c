// TODO
//    enum
//    check
//    test


#include <fann.h>
//#include <doublefann.h>

#include <fann_data.h>

#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "object.h"
#include "array.h"
#include "instr.h"
#include "gwion.h"
#include "plug.h"
#include "value.h"
#include "operator.h"
#include "import.h"

static Type t_fann_connect, t_fann_data;
extern m_int o_fileio_file;

static m_int o_fann_error, o_fann_from, o_fann_to, o_fann_weight;

#define ERROR(o) *(struct fann_error**)(o->data + o_fann_error)
#define FANN(o)  *(struct fann **)(o->data + o_fann_error)
#define DATA(o)  *(struct fann_train_data **)(o->data + o_fann_error)

static DTOR(fann_dtor) {
  if(FANN(o))
    fann_destroy(FANN(o));
}

static DTOR(data_dtor) {
  if(DATA(o))
    fann_destroy_train(DATA(o));
}

static MFUN(error_log) {
  FILE* file = IO_FILE(*(M_Object*)MEM(SZ_INT));
  fann_set_error_log(ERROR(o), file);
}

static MFUN(errno) {
  *(m_uint*)RETURN = fann_get_errno(ERROR(o));
}

static MFUN(reset_errno) {
  fann_reset_errno(ERROR(o));
}

static MFUN(errstr) {
  m_str str = fann_get_errstr(ERROR(o));
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->vm->gwion->mp, shred, str ? str : "no error");
}

static MFUN(reset_errstr) {
  fann_reset_errstr(ERROR(o));
}

static MFUN(print_error) {
  fann_print_error(ERROR(o));
}

static MFUN(standard) {
  m_uint i, size;
  m_uint num_layers = *(m_uint*)MEM(SZ_INT);
  M_Object array    = *(M_Object*)MEM(SZ_INT*2);
  if(FANN(o))
    fann_destroy(FANN(o));
  size = m_vector_size(ARRAY(array));
/*  unsigned int ptr[size];*/
  unsigned int ptr[size];
  for(i = 0; i < size; i++)
    m_vector_get(ARRAY(array), i, &ptr[i]);
  FANN(o) = fann_create_standard_array(num_layers, ptr);
}

static MFUN(shortcut) {
  m_uint i, size;
  m_uint num_layers = *(m_uint*)MEM(SZ_INT);
  M_Object array    = *(M_Object*)MEM(SZ_INT*2);
  if(FANN(o))
    fann_destroy(FANN(o));
  size = m_vector_size(ARRAY(array));
  unsigned int ptr[size];
  for(i = 0; i < size; i++)
    m_vector_get(ARRAY(array), i, &ptr[i]);
  FANN(o) = fann_create_shortcut_array(num_layers, ptr);
}

static MFUN(sparse) {
  m_uint i, size;
  m_float rate      = *(m_float*)MEM(SZ_INT);
  m_uint num_layers = *(m_uint*)MEM(SZ_INT + SZ_FLOAT);
  M_Object array    = *(M_Object*)MEM(SZ_FLOAT + SZ_INT*2);
  if(FANN(o))
    fann_destroy(FANN(o));
  size = m_vector_size(ARRAY(array));
  unsigned int ptr[size];
  for(i = 0; i < size; i++)
    m_vector_get(ARRAY(array), i, &ptr[i]);
  FANN(o) = fann_create_sparse_array(rate, num_layers, ptr);
}

static MFUN(randomize) {
  if(!FANN(o))
    return;
  fann_randomize_weights(FANN(o), *(m_float*)MEM(SZ_INT), *(m_float*)MEM(SZ_INT + SZ_FLOAT));
}

static MFUN(init) {
  if(!FANN(o))
    return;
  fann_init_weights(FANN(o), DATA(o));
}

static MFUN(print_connections) {
  if(!FANN(o))
    return;
  fann_print_connections(FANN(o));
}

// parameters
static MFUN(parameters) {
  if(!FANN(o))
    return;
  fann_print_parameters(FANN(o));
}

static MFUN(input) {
  *(m_uint*)RETURN = FANN(o) ? fann_get_num_input(FANN(o)) : -1;
}

static MFUN(output) {
  *(m_uint*)RETURN = FANN(o) ? fann_get_num_output(FANN(o)) : -1;
}

static MFUN(neurons) {
  *(m_uint*)RETURN = FANN(o) ? fann_get_total_neurons(FANN(o)) : -1;
}

static MFUN(connections) {
  *(m_uint*)RETURN = FANN(o) ? fann_get_total_connections(FANN(o)) : -1;
}

static MFUN(type) {
  *(m_uint*)RETURN = FANN(o) ? fann_get_network_type(FANN(o)) : -1;
}

static MFUN(rate) {
  *(m_float*)RETURN = FANN(o) ? fann_get_connection_rate(FANN(o)) : -1;
}

static MFUN(layer) {
  *(m_uint*)RETURN = FANN(o) ? fann_get_num_layers(FANN(o)) : -1;
}

static MFUN(layers) {
  if(!FANN(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_num_layers(FANN(o));
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_int], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, size);
  unsigned int j[size];
  fann_get_layer_array(FANN(o), j);
  for(i = 0; i < size; i++)
    m_vector_set(ARRAY(ret), i, (char*)&j[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}
static MFUN(bias) {
  if(!FANN(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_num_layers(FANN(o));
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_int], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, size);
  unsigned int j[size];
  fann_get_bias_array(FANN(o), j);
  for(i = 0; i < size; i++)
    m_vector_set(ARRAY(ret), i, (char*)&j[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static struct fann_connection to_fann(M_Object o) {
  struct fann_connection c;
  c.from_neuron = *(m_uint*)(o->data + o_fann_from);
  c.to_neuron   = *(m_uint*)(o->data + o_fann_to);
  c.weight      = *(m_float*)(o->data + o_fann_weight);
  return c;
}

static M_Object from_fann(const VM_Shred shred, struct fann_connection c) {
  M_Object o= new_object(shred->info->vm->gwion->mp, NULL, t_fann_connect);
  *(m_uint*)(o->data + o_fann_from)    = c.from_neuron;
  *(m_uint*)(o->data + o_fann_to)      = c.to_neuron;
  *(m_float*)(o->data + o_fann_weight) = c.weight;
  return o;
}

static MFUN(connection_array){
  if(!FANN(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_total_connections(FANN(o));
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_int], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, size);
  struct fann_connection c[size];
  fann_get_connection_array(FANN(o), c);
  for(i= 0; i < size; i++) {
   M_Object obj = from_fann(shred, c[i]);
   m_vector_set(ARRAY(ret), i, (char*)&obj);
  }
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(weigth_array) {
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  struct fann_connection c[size];
  for(i = 0; i < size; i++) {
    M_Object tmp;
    m_vector_get(ARRAY(obj), i, &tmp);
    c[i] = to_fann(tmp);
  }
  fann_set_weight_array(FANN(o), c, size);
}

static MFUN(weigth) {
  m_int   from   = *(m_int*)MEM(SZ_INT);
  m_int   to = *(m_int*)MEM(SZ_INT*2);
  m_float f    = *(m_float*)MEM(SZ_INT*3);
  fann_set_weight(FANN(o), from, to, f);
}

static MFUN(get_weigths) {
  if(!FANN(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_total_connections(FANN(o));
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_float], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, size);
  m_float f[size];
  fann_get_weights(FANN(o), f);
  for(i = 0; i < size; i++)
    m_vector_set(ARRAY(ret), i, (char*)&f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(set_weigths) {
  if(!FANN(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_total_connections(FANN(o));
  M_Object ret = *(M_Object*)MEM(SZ_INT);
  if(m_vector_size(ARRAY(ret)) < size)
  if(!FANN(o)) {
    gw_err("invalid array size for weights (%i). should be %i", m_vector_size(ARRAY(ret)), size);
    *(m_uint*)RETURN = 0;
    return;
  }
  m_float f[size];
  for(i = 0; i < size; i++)
    m_vector_get(ARRAY(ret), i, (char*)&f[i]);
  fann_set_weights(FANN(o), f);
  *(m_uint*)RETURN = (m_uint)ret;
}
// get/set user data
static SFUN(disable_seed) {
  fann_disable_seed_rand();
}

static SFUN(enable_seed) {
  fann_enable_seed_rand();
}

/*
// FIXED POINT only

MFUN(decimal_point)
{
  *(m_uint*)RETURN = fann_get_decimal_point(FANN(o));
}

MFUN(multiplier)
{
  *(m_uint*)RETURN = fann_get_multiplier(FANN(o));
}
*/
static SFUN(type_str) {
  m_int i = *(m_int*)MEM(0);
  if(i < 0 || i > 1) {
    *(m_uint*)RETURN = 0;
    return;
  }
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->vm->gwion->mp, shred, (m_str)FANN_NETTYPE_NAMES[i]);
}

static MFUN(load) {
  FANN(o) = fann_create_from_file(STRING(MEM(SZ_INT)));
}

static MFUN(save) {
  fann_save(FANN(o), STRING(MEM(SZ_INT)));
}

// training
static MFUN(train) {
  m_uint i;
  M_Object o_in  = *(M_Object*)MEM(SZ_INT);
  M_Object o_out = *(M_Object*)MEM(SZ_INT*2);
  m_uint   s_in  = m_vector_size(ARRAY(o_in));
  m_uint   s_out  = m_vector_size(ARRAY(o_out));
  m_float in[s_in], out[s_out];
  for(i = 0; i < s_in; i++)
    m_vector_get(ARRAY(o_in), i, (char*)&in[i]);
  for(i = 0; i < s_out; i++)
    m_vector_get(ARRAY(o_out), i, (char*)&out[i]);
  fann_train(FANN(o), in, out);
}

static MFUN(test) {
  m_uint i;
  M_Object o_in  = *(M_Object*)MEM(SZ_INT);
  M_Object o_out = *(M_Object*)MEM(SZ_INT*2);
  m_uint   s_in  = m_vector_size(ARRAY(o_in));
  m_uint   s_out = m_vector_size(ARRAY(o_out));
  m_uint s_ret   = fann_get_num_output(FANN(o));
  m_float in[s_in], out[s_out];
  for(i = 0; i < s_in; i++)
    m_vector_get(ARRAY(o_in), i, (char*)&in[i]);
  for(i = 0; i < s_out; i++)
    m_vector_get(ARRAY(o_out), i, (char*)&out[i]);
  m_float* f = fann_test(FANN(o), in, out);
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_float], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, s_ret);
  for(i = 0; i < s_ret; i++)
    m_vector_set(ARRAY(o_out), i, (char*)&f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(get_MSE) {
  *(m_float*)RETURN = fann_get_MSE(FANN(o));
}

static MFUN(get_bit_fail) {
  *(m_uint*)RETURN = fann_get_bit_fail(FANN(o));
}

static MFUN(reset_MSE) {
  fann_reset_MSE(FANN(o));
}

static MFUN(run) {
  m_uint i, size;
  if(!FANN(o) || !DATA(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  M_Object array    = *(M_Object*)MEM(SZ_INT);
  size = m_vector_size(ARRAY(array));
  m_float  ptr[size];
  for(i = 0; i < size; i++)
    m_vector_get(ARRAY(array), i, &ptr[i]);
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_float], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, fann_get_num_output(FANN(o)));
  m_float *f = fann_run(FANN(o), ptr);
  for(i = 0; i < fann_get_num_output(FANN(o)); i++)
    m_vector_set(ARRAY(ret), i, (char*)&f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

// Training Data Manipulation
static MFUN(scale_train) {
  M_Object data   = *(M_Object*)MEM(SZ_INT);
  fann_scale_train(FANN(o),DATA(data));
}

static MFUN(descale_train) {
  M_Object data   = *(M_Object*)MEM(SZ_INT);
  fann_descale_train(FANN(o),DATA(data));
}

static MFUN(input_scaling_params) {
  M_Object data  = *(M_Object*)MEM(SZ_INT);
  m_float min    = *(m_float*)MEM(SZ_INT*2);
  m_float max    = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT);
  *(m_uint*)RETURN = fann_set_input_scaling_params(FANN(o), DATA(data), min, max);
}

static MFUN(output_scaling_params) {
  M_Object data  = *(M_Object*)MEM(SZ_INT);
  m_float min    = *(m_float*)MEM(SZ_INT*2);
  m_float max    = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT);
  *(m_uint*)RETURN = fann_set_output_scaling_params(FANN(o), DATA(data), min, max);
}

static MFUN(scaling_params) {
  M_Object data  = *(M_Object*)MEM(SZ_INT);
  m_float imin   = *(m_float*)MEM(SZ_INT*2);
  m_float imax   = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT);
  m_float omin   = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT*2);
  m_float omax   = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT*3);
  *(m_uint*)RETURN = fann_set_scaling_params(FANN(o), DATA(data), imin, imax, omin, omax);
}

static MFUN(clear_scaling_params) {
  *(m_uint*)RETURN = fann_clear_scaling_params(FANN(o));
}

static MFUN(scale_input) {
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  m_float f[size];
  for(i = 0; i < size; i++)
    m_vector_get(ARRAY(obj), i, (char*)&f[i]);
  fann_scale_input(FANN(o), f);
}

static MFUN(scale_output) {
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  m_float f[size];
  for(i = 0; i < size; i++)
    m_vector_get(ARRAY(obj), i, (char*)&f[i]);
  fann_scale_output(FANN(o), f);
}

static MFUN(descale_input) {
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  m_float f[size];
  for(i = 0; i < size; i++)
    m_vector_get(ARRAY(obj), i, (char*)&f[i]);
  fann_descale_input(FANN(o), f);
  for(i = 0; i < size; i++)
    m_vector_set(ARRAY(obj), i, (char*)&f[i]);
}

static MFUN(descale_output) {
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  m_float f[size];
  for(i = 0; i < size; i++)
    m_vector_get(ARRAY(obj), i, (char*)&f[i]);
  fann_descale_output(FANN(o), f);
  for(i = 0; i < size; i++)
    m_vector_set(ARRAY(obj), i, (char*)&f[i]);
}

// Training Data Training
static MFUN(train_on_data) {
  M_Object data   = *(M_Object*)MEM(SZ_INT);
  m_uint   epoch  = *(m_uint*)MEM(SZ_INT*2);
  m_uint   report = *(m_uint*)MEM(SZ_INT*3);
  m_float  error  = *(m_uint*)MEM(SZ_INT*4);
  fann_train_on_data(FANN(o), DATA(data), epoch, report, error);
}

static MFUN(train_on_file) {
  M_Object data   = *(M_Object*)MEM(SZ_INT);
  m_uint   epoch  = *(m_uint*)MEM(SZ_INT*2);
  m_uint   report = *(m_uint*)MEM(SZ_INT*3);
  m_float  error  = *(m_uint*)MEM(SZ_INT*4);
  fann_train_on_file(FANN(o), STRING(data), epoch, report, error);
}

static MFUN(train_epoch) {
  M_Object data = *(M_Object*)MEM(SZ_INT);
  *(m_float*)RETURN = fann_train_epoch(FANN(o), DATA(data));
}

static MFUN(test_data) {
  M_Object data = *(M_Object*)MEM(SZ_INT);
  *(m_float*)RETURN = fann_test_data(FANN(o), DATA(data));
}

// Training Data Manipulation
static MFUN(train_from_file) {
  if(DATA(o))
    fann_destroy_train(DATA(o));
  fann_read_train_from_file(STRING(*(M_Object*)MEM(SZ_INT)));
}

static MFUN(train_from_array) {
  if(DATA(o))
    fann_destroy_train(DATA(o));
  m_uint i, j, y, in_size, out_size;
  m_uint  num_data = *(m_uint*)MEM(SZ_INT);
  M_Object obj;
  M_Object  in_obj = *(M_Object*)MEM(SZ_INT*2);
  M_Object out_obj = *(M_Object*)MEM(SZ_INT*3);
  in_size  = m_vector_size(ARRAY(in_obj));
  m_float* in[in_size];
  for(i = 0; i < in_size; i++) {
    m_vector_get(ARRAY(in_obj), i, &obj);
    y = m_vector_size(ARRAY(obj));
    m_float f[y];
    in[i] = f;
    for(j = 0; j < y; j++)
      m_vector_get(ARRAY(obj), j, (char*)&f[j]);
  }
  out_size = m_vector_size(ARRAY(out_obj));
  m_float* out[out_size];
  for(i = 0; i < out_size; i++) {
    m_vector_get(ARRAY(out_obj), i, &obj);
    y = m_vector_size(ARRAY(obj));
    m_float f[y];
    out[i] = f;
    for(j = 0; j < y; j++)
      m_vector_get(ARRAY(obj), j, (char*)&f[j]);
  }
  DATA(o) = fann_create_train_pointer_array(num_data, in_size, in, out_size, out);
}

static MFUN(train_create) {
  if(DATA(o))
    fann_destroy_train(DATA(o));
  DATA(o) = fann_create_train(*(m_uint*)MEM(SZ_INT),  *(m_uint*)MEM(SZ_INT*2), *(m_uint*)MEM(SZ_INT*3));
}

static MFUN(train_input) {
  if(!DATA(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_float* f = fann_get_train_input(DATA(o), *(m_uint*)MEM(SZ_INT));
  m_uint i, size = sizeof(f)/sizeof(m_float);
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_float], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, size);
  for(i = 0; i < size; i++)
    m_vector_set(ARRAY(ret), i, (char*)&f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(train_output) {
  if(!DATA(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_float* f = fann_get_train_output(DATA(o), *(m_uint*)MEM(SZ_INT));
  m_uint i, size = sizeof(f)/sizeof(m_float);
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_float], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, size);
  for(i = 0; i < size; i++)
    m_vector_set(ARRAY(ret), i, (char*)&f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(train_shuffle) {
  if(!DATA(o))
    return;
  fann_shuffle_train_data(DATA(o));
}

static MFUN(train_min_in) {
  if(!DATA(o)) {
    *(m_float*)RETURN = NAN;
    return;
  }
  *(m_float*)RETURN = fann_get_min_train_input(DATA(o));
}

static MFUN(train_max_in) {
  if(!DATA(o)) {
    *(m_float*)RETURN = NAN;
    return;
  }
  *(m_float*)RETURN = fann_get_max_train_input(DATA(o));
}

static MFUN(train_min_out) {
  if(!DATA(o)) {
    *(m_float*)RETURN = NAN;
    return;
  }
  *(m_float*)RETURN = fann_get_min_train_output(DATA(o));
}

static MFUN(train_max_out) {
  if(!DATA(o)) {
    *(m_float*)RETURN = NAN;
    return;
  }
  *(m_float*)RETURN = fann_get_max_train_output(DATA(o));
}

static MFUN(train_input_scale) {
  m_float min    = *(m_float*)MEM(SZ_INT);
  m_float max    = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  fann_scale_input_train_data(DATA(o), min, max);
}

static MFUN(train_output_scale) {
  m_float min    = *(m_float*)MEM(SZ_INT);
  m_float max    = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  fann_scale_output_train_data(DATA(o), min, max);
}

static MFUN(train_scale) {
  m_float min    = *(m_float*)MEM(SZ_INT);
  m_float max    = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  fann_scale_train_data(DATA(o), min, max);
}

static MFUN(train_length) {
  *(m_uint*)RETURN = fann_length_train_data(DATA(o));
}

static MFUN(train_num_input) {
  *(m_uint*)RETURN = fann_num_input_train_data(DATA(o));
}

static MFUN(train_num_output) {
  *(m_uint*)RETURN = fann_num_input_train_data(DATA(o));
}

static MFUN(train_save) {
  M_Object file = *(M_Object*)MEM(SZ_INT);
  *(m_uint*)RETURN = fann_save_train(DATA(o), STRING(file));
}

static SFUN(train_merge) {
  M_Object ret = new_object(shred->info->vm->gwion->mp, shred, t_fann_data);
  M_Object l = *(M_Object*)MEM(0);
  M_Object r = *(M_Object*)MEM(SZ_INT);
  DATA(ret) = fann_merge_train_data(DATA(l), DATA(r));
  *(m_uint*)RETURN = (m_uint)ret;
}

static SFUN(train_duplicate) {
  M_Object ret = new_object(shred->info->vm->gwion->mp, shred, t_fann_data);
  M_Object l = *(M_Object*)MEM(0);
  M_Object r = *(M_Object*)MEM(SZ_INT);
  DATA(ret) = fann_duplicate_train_data(DATA(l));
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(train_do_subset) {
  M_Object ret = new_object(shred->info->vm->gwion->mp, shred, t_fann_data);
  m_uint pos = *(m_uint*)MEM(SZ_INT);
  m_uint len = *(m_uint*)MEM(SZ_INT*2);
  DATA(ret) = fann_subset_train_data(DATA(o), pos, len);
  *(m_uint*)RETURN = (m_uint)ret;
}

// fann parameters
MFUN(get_training_algorithm) {
  *(m_uint*)RETURN = fann_get_training_algorithm(FANN(o));
}

MFUN(set_training_algorithm) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_training_algorithm(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_learning_rate) {
  *(m_float*)RETURN = fann_get_learning_rate(FANN(o));
}

MFUN(set_learning_rate) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_learning_rate(FANN(o), *(m_float*)RETURN);
}


MFUN(get_learning_momentum) {
  *(m_float*)RETURN = fann_get_learning_momentum(FANN(o));
}

MFUN(set_learning_momentum) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_learning_momentum(FANN(o), *(m_float*)RETURN);
}

MFUN(get_activation_function) {
  *(m_uint*)RETURN = fann_get_activation_function(FANN(o),
    *(m_int*)MEM(SZ_INT), *(m_int*)MEM(SZ_INT*2));
}

MFUN(set_activation_function) {
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_function(FANN(o), *(m_uint*)RETURN,
    *(m_int*)MEM(SZ_INT*2), *(m_int*)MEM(SZ_INT*3));
}

MFUN(set_activation_function_layer) {
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_function_layer(FANN(o), *(m_uint*)RETURN,
    *(m_int*)MEM(SZ_INT*2));
}

MFUN(set_activation_function_hidden) {
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_function_hidden(FANN(o), *(m_uint*)RETURN);
}

MFUN(set_activation_function_output) {
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_function_output(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_activation_steepness) {
  *(m_uint*)RETURN = fann_get_activation_steepness(FANN(o),
    *(m_int*)MEM(SZ_INT), *(m_int*)MEM(SZ_INT*2));
}

MFUN(set_activation_steepness) {
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_steepness(FANN(o), *(m_uint*)RETURN,
    *(m_int*)MEM(SZ_INT*2), *(m_int*)MEM(SZ_INT*3));
}

MFUN(set_activation_steepness_layer) {
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_steepness_layer(FANN(o), *(m_uint*)RETURN,
    *(m_int*)MEM(SZ_INT*2));
}

MFUN(set_activation_steepness_hidden) {
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_steepness_hidden(FANN(o), *(m_uint*)RETURN);
}

MFUN(set_activation_steepness_output) {
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_steepness_output(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_train_error_function) {
  *(m_uint*)RETURN = fann_get_train_error_function(FANN(o));
}

MFUN(set_train_error_function) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_train_error_function(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_train_stop_function) {
  *(m_uint*)RETURN = fann_get_train_stop_function(FANN(o));
}

MFUN(set_train_stop_function) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_train_stop_function(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_bit_fail_limit) {
  *(m_float*)RETURN = fann_get_bit_fail_limit(FANN(o));
}

MFUN(set_bit_fail_limit) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_bit_fail_limit(FANN(o), *(m_uint*)RETURN);
}
// callback

MFUN(get_quickprop_decay) {
  *(m_float*)RETURN = fann_get_quickprop_decay(FANN(o));
}

MFUN(set_quickprop_decay) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_quickprop_decay(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_quickprop_mu) {
  *(m_float*)RETURN = fann_get_quickprop_mu(FANN(o));
}

MFUN(set_quickprop_mu) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_quickprop_mu(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_increase_factor) {
  *(m_float*)RETURN = fann_get_rprop_increase_factor(FANN(o));
}

MFUN(set_rprop_increase_factor) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_increase_factor(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_decrease_factor) {
  *(m_float*)RETURN = fann_get_rprop_decrease_factor(FANN(o));
}

MFUN(set_rprop_decrease_factor) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_decrease_factor(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_delta_min) {
  *(m_float*)RETURN = fann_get_rprop_delta_min(FANN(o));
}

MFUN(set_rprop_delta_min) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_delta_min(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_delta_max) {
  *(m_float*)RETURN = fann_get_rprop_delta_max(FANN(o));
}

MFUN(set_rprop_delta_max) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_delta_max(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_delta_zero) {
  *(m_float*)RETURN = fann_get_rprop_delta_zero(FANN(o));
}

MFUN(set_rprop_delta_zero) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_delta_zero(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_sarprop_weight_decay_shift) {
  *(m_float*)RETURN = fann_get_sarprop_weight_decay_shift(FANN(o));
}

MFUN(set_sarprop_weight_decay_shift) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_sarprop_weight_decay_shift(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_sarprop_step_error_threshold_factor) {
  *(m_float*)RETURN = fann_get_sarprop_step_error_threshold_factor(FANN(o));
}

MFUN(set_sarprop_step_error_threshold_factor) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_sarprop_step_error_threshold_factor(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_sarprop_step_error_shift) {
  *(m_float*)RETURN = fann_get_sarprop_step_error_shift(FANN(o));
}

MFUN(set_sarprop_step_error_shift) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_sarprop_step_error_shift(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_sarprop_temperature) {
  *(m_float*)RETURN = fann_get_sarprop_temperature(FANN(o));
}

MFUN(set_sarprop_temperature) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_sarprop_temperature(FANN(o), *(m_uint*)RETURN);
}

// cascade parameters
MFUN(get_cascade_output_change_fraction) {
  *(m_float*)RETURN = fann_get_cascade_output_change_fraction(FANN(o));
}

MFUN(set_cascade_output_change_fraction) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_cascade_output_change_fraction(FANN(o), *(m_uint*)RETURN);
}


MFUN(get_cascade_output_stagnation_epochs) {
  *(m_uint*)RETURN = fann_get_cascade_output_stagnation_epochs(FANN(o));
}

MFUN(set_cascade_output_stagnation_epochs) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_output_stagnation_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_candidate_change_fraction) {
  *(m_float*)RETURN = fann_get_cascade_candidate_change_fraction(FANN(o));
}

MFUN(set_cascade_candidate_change_fraction) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_cascade_candidate_change_fraction(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_candidate_stagnation_epochs) {
  *(m_uint*)RETURN = fann_get_cascade_candidate_stagnation_epochs(FANN(o));
}

MFUN(set_cascade_candidate_stagnation_epochs) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_candidate_stagnation_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_weight_multiplier) {
  *(m_float*)RETURN = fann_get_cascade_weight_multiplier(FANN(o));
}

MFUN(set_cascade_weight_multiplier) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_cascade_weight_multiplier(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_candidate_limit) {
  *(m_float*)RETURN = fann_get_cascade_candidate_limit(FANN(o));
}

MFUN(set_cascade_candidate_limit) {
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_cascade_candidate_limit(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_max_out_epochs) {
  *(m_uint*)RETURN = fann_get_cascade_max_out_epochs(FANN(o));
}

MFUN(set_cascade_max_out_epochs) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_max_out_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_min_out_epochs) {
  *(m_uint*)RETURN = fann_get_cascade_min_out_epochs(FANN(o));
}

MFUN(set_cascade_min_out_epochs) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_min_out_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_max_cand_epochs) {
  *(m_uint*)RETURN = fann_get_cascade_max_cand_epochs(FANN(o));
}

MFUN(set_cascade_max_cand_epochs) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_max_cand_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_min_cand_epochs) {
  *(m_uint*)RETURN = fann_get_cascade_min_cand_epochs(FANN(o));
}

MFUN(set_cascade_min_cand_epochs) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_min_cand_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_activation_functions_count) {
  *(m_uint*)RETURN = fann_get_cascade_activation_functions_count(FANN(o));
}

MFUN(get_cascade_activation_functions) {
  if(!FANN(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_cascade_activation_functions_count(FANN(o));
  enum fann_activationfunc_enum * tmp = fann_get_cascade_activation_functions(FANN(o));
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_int], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, size);
  for(i=0; i < size; i++)
    m_vector_set(ARRAY(ret), i, (char*)&tmp[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

MFUN(set_cascade_activation_functions) {
  if(!FANN(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  M_Object ret = *(M_Object*)MEM(SZ_INT);
  m_uint index = *(m_uint*)MEM(SZ_INT*2);
  m_uint i, size = m_vector_size(ARRAY(ret));
  enum fann_activationfunc_enum tmp[size];
  for(i=0; i < size; i++)
    m_vector_get(ARRAY(ret), i, &tmp[i]);
  fann_set_cascade_activation_functions(FANN(o), tmp, size);
  *(m_uint*)RETURN = (m_uint)ret;
}

MFUN(get_cascade_activation_steepnesses_count) {
  *(m_uint*)RETURN = fann_get_cascade_activation_steepnesses_count(FANN(o));
}

MFUN(get_cascade_activation_steepnesses) {
  if(!FANN(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_cascade_activation_steepnesses_count(FANN(o));
  m_float* tmp = fann_get_cascade_activation_steepnesses(FANN(o));
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_float], 1);
  M_Object ret = new_array(shred->info->vm->gwion->mp, t, size);
  for(i=0; i < size; i++)
    m_vector_set(ARRAY(ret), i, (char*)&tmp[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

MFUN(set_cascade_activation_steepnesses) {
  if(!FANN(o)) {
    *(m_uint*)RETURN = 0;
    return;
  }
  M_Object ret = *(M_Object*)MEM(SZ_INT);
  m_uint index = *(m_uint*)MEM(SZ_INT*2);
  m_uint i, size = m_vector_size(ARRAY(ret));
  m_float tmp[size];
  for(i=0; i < size; i++)
    m_vector_get(ARRAY(ret), i, (char*)&tmp[i]);
  fann_set_cascade_activation_steepnesses(FANN(o), tmp, size);
  *(m_uint*)RETURN = (m_uint)ret;
}

MFUN(get_cascade_num_candidate_groups) {
  *(m_uint*)RETURN = fann_get_cascade_num_candidate_groups(FANN(o));
}

MFUN(set_cascade_num_candidate_groups) {
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_num_candidate_groups(FANN(o), *(m_uint*)RETURN);
}

GWION_IMPORT(fann) {
  GWI_BB(gwi_class_ini(gwi, "FANN_connect", NULL))
  gwi_item_ini(gwi,"int",  "from");
  o_fann_from = gwi_item_end(gwi, ae_flag_member, NULL);
  GWI_BB(o_fann_from)
  gwi_item_ini(gwi,"int",  "to");
  o_fann_to = gwi_item_end(gwi, ae_flag_member, NULL);
  GWI_BB(o_fann_to)
  gwi_item_ini(gwi,"int",  "weight");
  o_fann_weight = gwi_item_end(gwi, ae_flag_member, NULL);
  GWI_BB(o_fann_weight)
  GWI_BB(gwi_class_end(gwi))

  // this is for error handling
  GWI_BB(gwi_class_ini(gwi, "FANN_base", NULL))
  gwi_item_ini(gwi, "@internal", "@data");
  o_fann_error = gwi_item_end(gwi, ae_flag_member, NULL);
  GWI_BB(o_fann_error)
  gwi_func_ini(gwi, "void", "log");
    gwi_func_arg(gwi, "FileIO", "f");
  GWI_BB(gwi_func_end(gwi, error_log, ae_flag_none))
  gwi_func_ini(gwi, "int", "errno");
  GWI_BB(gwi_func_end(gwi, errno, ae_flag_none))
  gwi_func_ini(gwi, "void", "reset_errno");
  GWI_BB(gwi_func_end(gwi, reset_errno, ae_flag_none))
  gwi_func_ini(gwi, "void", "reset_errstr");
  GWI_BB(gwi_func_end(gwi, reset_errstr, ae_flag_none))
  gwi_func_ini(gwi, "void", "errstr");
  GWI_BB(gwi_func_end(gwi, errstr, ae_flag_none))
  gwi_func_ini(gwi, "void", "print_error");
  GWI_BB(gwi_func_end(gwi, print_error, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

//  Training Data Manipulation
  GWI_BB(gwi_class_ini(gwi, "Fann_data", "FANN_base"))
  gwi_class_xtor(gwi, NULL, data_dtor);
  gwi_func_ini(gwi, "void", "from_data");
    gwi_func_arg(gwi, "int", "num");
    gwi_func_arg(gwi, "float[][]", "in");
    gwi_func_arg(gwi, "float[][]", "out");
  GWI_BB(gwi_func_end(gwi, train_from_array, ae_flag_none))
  gwi_func_ini(gwi, "void", "load");
    gwi_func_arg(gwi, "string", "filename");
    gwi_func_arg(gwi, "int", "epochs");
    gwi_func_arg(gwi, "int", "report");
    gwi_func_arg(gwi, "float", "error");
  GWI_BB(gwi_func_end(gwi, train_from_file, ae_flag_none))
  gwi_func_ini(gwi, "void", "save");
    gwi_func_arg(gwi, "string", "filename");
  GWI_BB(gwi_func_end(gwi, train_save, ae_flag_none))
  gwi_func_ini(gwi, "void", "create");
    gwi_func_arg(gwi, "int", "num_data");
    gwi_func_arg(gwi, "int", "num_in");
    gwi_func_arg(gwi, "int", "num_out");
  GWI_BB(gwi_func_end(gwi, train_create, ae_flag_none))
  gwi_func_ini(gwi, "float[]", "input");
    gwi_func_arg(gwi, "int", "position");
  GWI_BB(gwi_func_end(gwi, train_input, ae_flag_none))
  gwi_func_ini(gwi, "float[]", "output");
    gwi_func_arg(gwi, "int", "position");
  GWI_BB(gwi_func_end(gwi, train_output, ae_flag_none))
  gwi_func_ini(gwi, "void", "shuffle");
  GWI_BB(gwi_func_end(gwi, train_shuffle, ae_flag_none))
  gwi_func_ini(gwi, "void", "min_in");
  GWI_BB(gwi_func_end(gwi, train_min_in, ae_flag_none))
  gwi_func_ini(gwi, "void", "max_in");
  GWI_BB(gwi_func_end(gwi, train_max_in, ae_flag_none))
  gwi_func_ini(gwi, "void", "max_in");
  GWI_BB(gwi_func_end(gwi, train_max_in, ae_flag_none))
  gwi_func_ini(gwi, "void", "max_out");
  GWI_BB(gwi_func_end(gwi, train_max_out, ae_flag_none))
  gwi_func_ini(gwi, "int", "length");
  GWI_BB(gwi_func_end(gwi, train_length, ae_flag_none))
  gwi_func_ini(gwi, "int", "num_input");
  GWI_BB(gwi_func_end(gwi, train_num_input, ae_flag_none))
  gwi_func_ini(gwi, "int", "num_ouput");
  GWI_BB(gwi_func_end(gwi, train_num_output, ae_flag_none))
  gwi_func_ini(gwi, "FANN_data", "subset");
    gwi_func_arg(gwi, "int", "pos");
    gwi_func_arg(gwi, "int", "len");
  GWI_BB(gwi_func_end(gwi, train_do_subset, ae_flag_none))
  gwi_func_ini(gwi, "FANN_data", "merge");
    gwi_func_arg(gwi, "FANN_data", "l");
    gwi_func_arg(gwi, "FANN_data", "r");
  GWI_BB(gwi_func_end(gwi, train_merge, ae_flag_static))
  gwi_func_ini(gwi, "FANN_data", "duplicate");
    gwi_func_arg(gwi, "FANN_data", "l");
  GWI_BB(gwi_func_end(gwi, train_duplicate, ae_flag_static))

  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_class_ini(gwi, "FANN", "FANN_base"))
  gwi_class_xtor(gwi, NULL, fann_dtor);
  gwi_func_ini(gwi, "string", "type_str");
    gwi_func_arg(gwi, "int", "layer");
  GWI_BB(gwi_func_end(gwi, type_str, ae_flag_static))
  gwi_func_ini(gwi, "void", "standard");
    gwi_func_arg(gwi, "int", "layer");
    gwi_func_arg(gwi, "int[]", "outputs");
  GWI_BB(gwi_func_end(gwi, standard, ae_flag_none))
  gwi_func_ini(gwi, "void", "shortcut");
    gwi_func_arg(gwi, "int", "layer");
    gwi_func_arg(gwi, "int[]", "outputs");
  GWI_BB(gwi_func_end(gwi, shortcut, ae_flag_none))
  gwi_func_ini(gwi, "void", "sparse");
    gwi_func_arg(gwi, "float", "rate");
    gwi_func_arg(gwi, "int", "layer");
    gwi_func_arg(gwi, "int[]", "outputs");
  GWI_BB(gwi_func_end(gwi, sparse, ae_flag_none))
  gwi_func_ini(gwi, "void", "randomize");
    gwi_func_arg(gwi, "float", "min");
    gwi_func_arg(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, randomize, ae_flag_none))
  gwi_func_ini(gwi, "void", "init");
  GWI_BB(gwi_func_end(gwi, init, ae_flag_none))
  gwi_func_ini(gwi, "void", "print");
  GWI_BB(gwi_func_end(gwi, print_connections, ae_flag_none))
  // parameters
  gwi_func_ini(gwi, "void", "parameters");
  GWI_BB(gwi_func_end(gwi, parameters, ae_flag_none))
  gwi_func_ini(gwi, "int", "input");
  GWI_BB(gwi_func_end(gwi,  input, ae_flag_none))
  gwi_func_ini(gwi, "int", "output");
  GWI_BB(gwi_func_end(gwi, output, ae_flag_none))
  gwi_func_ini(gwi, "int", "neurons");
  GWI_BB(gwi_func_end(gwi, neurons, ae_flag_none))
  gwi_func_ini(gwi, "int", "connections");
  GWI_BB(gwi_func_end(gwi, connections, ae_flag_none))
  gwi_func_ini(gwi, "int", "type");
  GWI_BB(gwi_func_end(gwi, type, ae_flag_none))
  gwi_func_ini(gwi, "float", "rate");
  GWI_BB(gwi_func_end(gwi, rate, ae_flag_none))
  gwi_func_ini(gwi, "int", "layer");
  GWI_BB(gwi_func_end(gwi, layer, ae_flag_none))
  gwi_func_ini(gwi, "int[]", "layers");
  GWI_BB(gwi_func_end(gwi, layers, ae_flag_none))
  gwi_func_ini(gwi, "int[]", "bias");
  GWI_BB(gwi_func_end(gwi,  bias, ae_flag_none))
  gwi_func_ini(gwi, "FANN_connect[]", "connection_array");
  GWI_BB(gwi_func_end(gwi, connection_array, ae_flag_none))
  gwi_func_ini(gwi, "void", "weigth_array");
    gwi_func_arg(gwi, "FANN_connect[]", "array");
  GWI_BB(gwi_func_end(gwi, weigth_array, ae_flag_none))
  gwi_func_ini(gwi, "float[]", "weigth");
    gwi_func_arg(gwi, "int", "from");
    gwi_func_arg(gwi, "int", "to");
    gwi_func_arg(gwi, "float", "weigth");
  GWI_BB(gwi_func_end(gwi, weigth, ae_flag_none))
  gwi_func_ini(gwi, "float[]", "weigths");
  GWI_BB(gwi_func_end(gwi, get_weigths, ae_flag_none))
  gwi_func_ini(gwi, "float[]", "weigths");
    gwi_func_arg(gwi, "float[]", "f");
  GWI_BB(gwi_func_end(gwi, set_weigths, ae_flag_none))
// get/set user data
  gwi_func_ini(gwi, "void", "disable_seed");
  GWI_BB(gwi_func_end(gwi, disable_seed, ae_flag_static))
  gwi_func_ini(gwi, "void", "enable_seed");
  GWI_BB(gwi_func_end(gwi, enable_seed, ae_flag_static))

  gwi_func_ini(gwi, "void", "load");
  gwi_func_arg(gwi, "string", "file");
  GWI_BB(gwi_func_end(gwi,  load, ae_flag_none))
  gwi_func_ini(gwi, "void", "save");
    gwi_func_arg(gwi, "string", "file");
  GWI_BB(gwi_func_end(gwi, save, ae_flag_none))
  gwi_func_ini(gwi, "float[]", "run");
    gwi_func_arg(gwi, "float[]", "in");
  GWI_BB(gwi_func_end(gwi, run, ae_flag_none))
  // FIXED POINT only
/*  gwi_func_ini(gwi, "void", "decimal_point");
/*  gwi_func_ini(gwi, "void", "multiplier");

// training
  gwi_func_ini(gwi, "void", "train");
    gwi_func_arg(gwi, "float[]", "in");
    gwi_func_arg(gwi, "float[]", "out");
  GWI_BB(gwi_func_end(gwi, train, ae_flag_none))
  gwi_func_ini(gwi, "float[]", "test");
    gwi_func_arg(gwi, "float[]", "in");
    gwi_func_arg(gwi, "float[]", "out");
  GWI_BB(gwi_func_end(gwi, test, ae_flag_none))
  gwi_func_ini(gwi, "float", "mse");
  GWI_BB(gwi_func_end(gwi, get_MSE, ae_flag_none))
  gwi_func_ini(gwi, "int", "bit_fail");
  GWI_BB(gwi_func_end(gwi, get_bit_fail, ae_flag_none))
  gwi_func_ini(gwi, "void", "reset_mse");
  GWI_BB(gwi_func_end(gwi, reset_MSE, ae_flag_none))

// Training Data Training
  gwi_func_ini(gwi, "void", "on_data");
    gwi_func_arg(gwi, "FANN_data", "data");
    gwi_func_arg(gwi, "int",   "epoch");
    gwi_func_arg(gwi, "int",   "report");
    gwi_func_arg(gwi, "float", "error");
  GWI_BB(gwi_func_end(gwi, train_on_data, ae_flag_none))
  gwi_func_ini(gwi, "void", "on_file");
    gwi_func_arg(gwi, "string", "filename");
    gwi_func_arg(gwi, "int",   "epoch");
    gwi_func_arg(gwi, "int",   "report");
    gwi_func_arg(gwi, "float", "error");
  GWI_BB(gwi_func_end(gwi, train_on_file, ae_flag_none))
  gwi_func_ini(gwi, "void", "epoch");
    gwi_func_arg(gwi, "FANN_data", "data");
  GWI_BB(gwi_func_end(gwi, train_epoch, ae_flag_none))
  gwi_func_ini(gwi, "void", "epoch");
    gwi_func_arg(gwi, "FANN_data", "data");
  GWI_BB(gwi_func_end(gwi, test_data, ae_flag_none))
/*  gwi_func_ini(gwi, "void", "test");
/*    gwi_func_arg(gwi, "FANN_data", "data");*/

// Training Data Manipulation
  gwi_func_ini(gwi, "void", "scale");
    gwi_func_arg(gwi, "FANN_data", "data");
  GWI_BB(gwi_func_end(gwi, scale_train, ae_flag_none))
  gwi_func_ini(gwi, "void", "descale");
    gwi_func_arg(gwi, "FANN_data", "data");
  GWI_BB(gwi_func_end(gwi, descale_train, ae_flag_none))
  gwi_func_ini(gwi, "void", "in_scaling_params");
    gwi_func_arg(gwi, "FANN_data", "data");
    gwi_func_arg(gwi, "float", "min");
    gwi_func_arg(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, input_scaling_params, ae_flag_none))
  gwi_func_ini(gwi, "void", "out_scaling_params");
    gwi_func_arg(gwi, "FANN_data", "data");
    gwi_func_arg(gwi, "float", "min");
    gwi_func_arg(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, output_scaling_params, ae_flag_none))
  gwi_func_ini(gwi, "void", "scaling_params");
    gwi_func_arg(gwi, "FANN_data", "data");
    gwi_func_arg(gwi, "float", "imin");
    gwi_func_arg(gwi, "float", "imax");
    gwi_func_arg(gwi, "float", "omin");
    gwi_func_arg(gwi, "float", "omax");
  GWI_BB(gwi_func_end(gwi, scaling_params, ae_flag_none))
  gwi_func_ini(gwi, "void", "clear_scaling_params");
  GWI_BB(gwi_func_end(gwi, scaling_params, ae_flag_none))
  GWI_BB(gwi_func_end(gwi, scaling_params, ae_flag_none))
  gwi_func_ini(gwi, "void", "scale_input");
    gwi_func_arg(gwi, "float[]", "in");
  GWI_BB(gwi_func_end(gwi, scale_input, ae_flag_none))
  gwi_func_ini(gwi, "void", "scale_output");
    gwi_func_arg(gwi, "float[]", "out");
  GWI_BB(gwi_func_end(gwi, scale_output, ae_flag_none))
  gwi_func_ini(gwi, "void", "descale_input");
    gwi_func_arg(gwi, "float[]", "in");
  GWI_BB(gwi_func_end(gwi, descale_input, ae_flag_none))
  gwi_func_ini(gwi, "void", "descale_output");
    gwi_func_arg(gwi, "float[]", "out");
  GWI_BB(gwi_func_end(gwi, descale_output, ae_flag_none))
  gwi_func_ini(gwi, "void", "scale_in");
    gwi_func_arg(gwi, "FANN_data", "data");
    gwi_func_arg(gwi, "float", "min");
    gwi_func_arg(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, train_input_scale, ae_flag_none))
  gwi_func_ini(gwi, "void", "scale_out");
    gwi_func_arg(gwi, "FANN_data", "data");
    gwi_func_arg(gwi, "float", "min");
    gwi_func_arg(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, train_output_scale, ae_flag_none))
  gwi_func_ini(gwi, "void", "scale");
    gwi_func_arg(gwi, "FANN_data", "data");
    gwi_func_arg(gwi, "float", "min");
    gwi_func_arg(gwi, "float", "max");
  GWI_BB(gwi_func_end(gwi, train_scale, ae_flag_none))

  gwi_func_ini(gwi, "int", "training_algorithm");
  GWI_BB(gwi_func_end(gwi, get_training_algorithm, ae_flag_none))
  gwi_func_ini(gwi, "int", "training_algorithm");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_training_algorithm, ae_flag_none))
  gwi_func_ini(gwi, "float", "learning_rate");
  GWI_BB(gwi_func_end(gwi, get_learning_rate, ae_flag_none))
  gwi_func_ini(gwi, "float", "learning_rate");
    gwi_func_arg(gwi, "float", "arg");
  GWI_BB(gwi_func_end(gwi, set_learning_rate, ae_flag_none))
  gwi_func_ini(gwi, "float", "learning_momentum");
  GWI_BB(gwi_func_end(gwi, get_learning_momentum, ae_flag_none))
  gwi_func_ini(gwi, "float", "learning_momentum");
    gwi_func_arg(gwi, "float", "arg");
  GWI_BB(gwi_func_end(gwi, set_learning_momentum, ae_flag_none))
  gwi_func_ini(gwi, "int", "activation_function");
    gwi_func_arg(gwi, "int", "layer");
    gwi_func_arg(gwi, "int", "neuron");
  GWI_BB(gwi_func_end(gwi, get_activation_function, ae_flag_none))
  gwi_func_ini(gwi, "int", "activation_function");
    gwi_func_arg(gwi, "int", "arg");
    gwi_func_arg(gwi, "int", "layer");
    gwi_func_arg(gwi, "int", "neuron");
  GWI_BB(gwi_func_end(gwi, set_activation_function, ae_flag_none))
  gwi_func_ini(gwi, "int", "activation_function_layer");
    gwi_func_arg(gwi, "int", "layer");
  GWI_BB(gwi_func_end(gwi, set_activation_function_layer, ae_flag_none))
  gwi_func_ini(gwi, "int", "activation_function_output");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_activation_function_output, ae_flag_none))
  gwi_func_ini(gwi, "float", "activation_steepness");
    gwi_func_arg(gwi, "int", "layer");
    gwi_func_arg(gwi, "int", "neuron");
  GWI_BB(gwi_func_end(gwi, get_activation_steepness, ae_flag_none))
  gwi_func_ini(gwi, "float", "activation_steepness");
    gwi_func_arg(gwi, "int", "arg");
    gwi_func_arg(gwi, "int", "layer");
    gwi_func_arg(gwi, "int", "neuron");
  GWI_BB(gwi_func_end(gwi, set_activation_steepness, ae_flag_none))
  gwi_func_ini(gwi, "float", "activation_steepness_layer");
    gwi_func_arg(gwi, "int", "layer");
  GWI_BB(gwi_func_end(gwi, set_activation_steepness_layer, ae_flag_none))
  gwi_func_ini(gwi, "float", "activation_steepness_output");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_activation_steepness_output, ae_flag_none))
  gwi_func_ini(gwi, "int", "train_error_function");
  GWI_BB(gwi_func_end(gwi, get_train_error_function, ae_flag_none))
  gwi_func_ini(gwi, "int", "train_error_function");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_train_error_function, ae_flag_none))
  gwi_func_ini(gwi, "int", "train_stop_function");
  GWI_BB(gwi_func_end(gwi, get_train_stop_function, ae_flag_none))
  gwi_func_ini(gwi, "int", "train_stop_function");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_train_stop_function, ae_flag_none))
  gwi_func_ini(gwi, "float", "bit_fail_limit");
  GWI_BB(gwi_func_end(gwi, get_bit_fail_limit, ae_flag_none))
  gwi_func_ini(gwi, "float", "bit_fail_limit");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_bit_fail_limit, ae_flag_none))
  gwi_func_ini(gwi, "float", "quickprop_decay");
  GWI_BB(gwi_func_end(gwi, get_quickprop_decay, ae_flag_none))
  gwi_func_ini(gwi, "float", "quickprop_decay");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_quickprop_decay, ae_flag_none))
  gwi_func_ini(gwi, "float", "quickprop_mu");
  GWI_BB(gwi_func_end(gwi, get_quickprop_mu, ae_flag_none))
  gwi_func_ini(gwi, "float", "quickprop_mu");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_quickprop_mu, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_increase_factor");
  GWI_BB(gwi_func_end(gwi, get_rprop_increase_factor, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_increase_factor");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_rprop_increase_factor, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_decrease_factor");
  GWI_BB(gwi_func_end(gwi, get_rprop_decrease_factor, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_decrease_factor");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_rprop_decrease_factor, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_delta_min");
  GWI_BB(gwi_func_end(gwi, get_rprop_delta_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_delta_min");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_rprop_delta_min, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_delta_max");
  GWI_BB(gwi_func_end(gwi, get_rprop_delta_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_delta_max");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_rprop_delta_max, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_delta_zero");
  GWI_BB(gwi_func_end(gwi, get_rprop_delta_zero, ae_flag_none))
  gwi_func_ini(gwi, "float", "rprop_delta_zero");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_rprop_delta_zero, ae_flag_none))
  gwi_func_ini(gwi, "float", "sarprop_weight_decay_shift");
  GWI_BB(gwi_func_end(gwi, get_sarprop_weight_decay_shift, ae_flag_none))
  gwi_func_ini(gwi, "float", "sarprop_weight_decay_shift");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_sarprop_weight_decay_shift, ae_flag_none))
  gwi_func_ini(gwi, "float", "sarprop_step_error_threshold_factor");
  GWI_BB(gwi_func_end(gwi, get_sarprop_step_error_threshold_factor, ae_flag_none))
  gwi_func_ini(gwi, "float", "sarprop_step_error_threshold_factor");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_sarprop_step_error_threshold_factor, ae_flag_none))
   gwi_func_ini(gwi, "float", "sarprop_step_error_shift");
  GWI_BB(gwi_func_end(gwi, get_sarprop_step_error_shift, ae_flag_none))
  gwi_func_ini(gwi, "float", "sarprop_step_error_shift");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_sarprop_step_error_shift, ae_flag_none))
  gwi_func_ini(gwi, "float", "sarprop_temperature");
  GWI_BB(gwi_func_end(gwi, get_sarprop_temperature, ae_flag_none))
  gwi_func_ini(gwi, "float", "sarprop_temperature");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi, set_sarprop_temperature, ae_flag_none))
  gwi_func_ini(gwi, "float", "cascade_output_change_fraction");
  GWI_BB(gwi_func_end(gwi, get_cascade_output_change_fraction, ae_flag_none))
  gwi_func_ini(gwi, "float", "cascade_output_change_fraction");
    gwi_func_arg(gwi, "float", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_output_change_fraction, ae_flag_none))
   gwi_func_ini(gwi, "int", "cascade_output_stagnation_epochs");
  GWI_BB(gwi_func_end(gwi,  get_cascade_output_stagnation_epochs, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_output_stagnation_epochs");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_output_stagnation_epochs, ae_flag_none))
  gwi_func_ini(gwi, "float", "cascade_candidate_change_fraction");
  GWI_BB(gwi_func_end(gwi,  get_cascade_candidate_change_fraction, ae_flag_none))
  gwi_func_ini(gwi, "float", "cascade_candidate_change_fraction");
    gwi_func_arg(gwi, "float", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_candidate_change_fraction, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_candidate_stagnation_epochs");
  GWI_BB(gwi_func_end(gwi,  get_cascade_candidate_stagnation_epochs, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_candidate_stagnation_epochs");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_candidate_stagnation_epochs, ae_flag_none))
  gwi_func_ini(gwi, "float", "cascade_weight_multiplier");
  GWI_BB(gwi_func_end(gwi,  get_cascade_weight_multiplier, ae_flag_none))
  gwi_func_ini(gwi, "float", "cascade_weight_multiplier");
    gwi_func_arg(gwi, "float", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_weight_multiplier, ae_flag_none))
  gwi_func_ini(gwi, "float", "cascade_candidate_limit");
  GWI_BB(gwi_func_end(gwi,  get_cascade_candidate_limit, ae_flag_none))
  gwi_func_ini(gwi, "float", "cascade_candidate_limit");
    gwi_func_arg(gwi, "float", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_candidate_limit, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_max_out_epochs");
  GWI_BB(gwi_func_end(gwi,  get_cascade_max_out_epochs, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_max_out_epochs");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_max_out_epochs, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_min_out_epochs");
  GWI_BB(gwi_func_end(gwi,  get_cascade_min_out_epochs, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_min_out_epochs");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_min_out_epochs, ae_flag_none))
   gwi_func_ini(gwi, "int", "cascade_max_cand_epochs");
  GWI_BB(gwi_func_end(gwi,  get_cascade_max_cand_epochs, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_max_cand_epochs");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_max_cand_epochs, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_min_cand_epochs");
  GWI_BB(gwi_func_end(gwi,  get_cascade_min_cand_epochs, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_min_cand_epochs");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_min_cand_epochs, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_activation_functions_count");
  GWI_BB(gwi_func_end(gwi,  get_cascade_activation_functions_count, ae_flag_none))
  gwi_func_ini(gwi, "int[]", "cascade_activation_functions");
  GWI_BB(gwi_func_end(gwi,  get_cascade_activation_functions, ae_flag_none))
  gwi_func_ini(gwi, "int[]", "cascade_activation_functions");
    gwi_func_arg(gwi, "int[]", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_activation_functions, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_activation_steepnesses_count");
  GWI_BB(gwi_func_end(gwi,  get_cascade_activation_steepnesses_count, ae_flag_none))
  gwi_func_ini(gwi, "float[]", "cascade_activation_steepnesses");
  GWI_BB(gwi_func_end(gwi,  get_cascade_activation_steepnesses, ae_flag_none))
  gwi_func_ini(gwi, "float[]", "cascade_activation_steepnesses");
    gwi_func_arg(gwi, "float[]", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_activation_steepnesses, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_num_candidate_groups");
  GWI_BB(gwi_func_end(gwi,  get_cascade_num_candidate_groups, ae_flag_none))
  gwi_func_ini(gwi, "int", "cascade_num_candidate_groups");
    gwi_func_arg(gwi, "int", "arg");
  GWI_BB(gwi_func_end(gwi,  set_cascade_num_candidate_groups, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
