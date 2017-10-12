// TODO
//    enum
//    check
//    test


#include <fann.h>
//#include <doublefann.h>

#include <fann_data.h>

#include "type.h"
#include "instr.h"
#include "import.h"
#include "err_msg.h"
#include "lang.h"

extern m_int o_fileio_file;

static struct Type_ t_fann_connect = { "FANN_connect", sizeof(m_uint), &t_object };
static struct Type_ t_fann_base =    { "FANN_base",    sizeof(m_uint), &t_object };
static struct Type_ t_fann =         { "FANN",         sizeof(m_uint), &t_fann_base };
static struct Type_ t_fann_data =    { "FANN_data",    sizeof(m_uint), &t_fann_base };

static m_int o_fann_error, o_fann_from, o_fann_to, o_fann_weight;

#define ERROR(o) *(struct fann_error**)(o->data + o_fann_error)
#define FANN(o)  *(struct fann **)(o->data + o_fann_error)
#define DATA(o)  *(struct fann_train_data **)(o->data + o_fann_error)

static DTOR(fann_dtor)
{
  if(FANN(o))
    fann_destroy(FANN(o));
}

static DTOR(data_dtor)
{
  if(DATA(o))
    fann_destroy_train(DATA(o));
}

static MFUN(error_log)
{
  FILE* file = IO_FILE(*(M_Object*)MEM(SZ_INT));
  fann_set_error_log(ERROR(o), file);
}

static MFUN(errno)
{
  *(m_uint*)RETURN = fann_get_errno(ERROR(o));
}

static MFUN(reset_errno)
{
  fann_reset_errno(ERROR(o));
}

static MFUN(errstr)
{
  m_str str = fann_get_errstr(ERROR(o));
  *(m_uint*)RETURN = (m_uint)new_String(shred, str ? str : "no error");
}

static MFUN(reset_errstr)
{
  fann_reset_errstr(ERROR(o));
}

static MFUN(print_error)
{
  fann_print_error(ERROR(o));
}

static MFUN(standard)
{
  m_uint i, size;
  m_uint num_layers = *(m_uint*)MEM(SZ_INT);
  M_Object array    = *(M_Object*)MEM(SZ_INT*2);
  if(FANN(o))
    fann_destroy(FANN(o));
  size = m_vector_size(ARRAY(array));
/*  unsigned int ptr[size];*/
  unsigned int ptr[size];
  for(i = 0; i < size; i++)
    ptr[i] = i_vector_at(ARRAY(array), i);
  FANN(o) = fann_create_standard_array(num_layers, ptr);
}

static MFUN(shortcut)
{
  m_uint i, size;
  m_uint num_layers = *(m_uint*)MEM(SZ_INT);
  M_Object array    = *(M_Object*)MEM(SZ_INT*2);
  if(FANN(o))
    fann_destroy(FANN(o));
  size = m_vector_size(ARRAY(array));
  unsigned int ptr[size];
  for(i = 0; i < size; i++)
    ptr[i] = i_vector_at(ARRAY(array), i);
  FANN(o) = fann_create_shortcut_array(num_layers, ptr);
}

static MFUN(sparse)
{
  m_uint i, size;
  m_float rate      = *(m_float*)MEM(SZ_INT);
  m_uint num_layers = *(m_uint*)MEM(SZ_INT + SZ_FLOAT);
  M_Object array    = *(M_Object*)MEM(SZ_FLOAT + SZ_INT*2);
  if(FANN(o))
    fann_destroy(FANN(o));
  size = m_vector_size(ARRAY(array));
  unsigned int ptr[size];
  for(i = 0; i < size; i++)
    ptr[i] = i_vector_at(ARRAY(array), i);
  FANN(o) = fann_create_sparse_array(rate, num_layers, ptr);
}

static MFUN(randomize)
{
  if(!FANN(o))
    return;
  fann_randomize_weights(FANN(o), *(m_float*)MEM(SZ_INT), *(m_float*)MEM(SZ_INT + SZ_FLOAT));
}

static MFUN(init)
{
  if(!FANN(o))
    return;
  fann_init_weights(FANN(o), DATA(o));
}

static MFUN(print_connections)
{
  if(!FANN(o))
    return;
  fann_print_connections(FANN(o));
}

// parameters
static MFUN(parameters)
{
  if(!FANN(o))
    return;
  fann_print_parameters(FANN(o));
}

static MFUN(input)
{
  *(m_uint*)RETURN = FANN(o) ? fann_get_num_input(FANN(o)) : - 1;
}

static MFUN(output)
{
  *(m_uint*)RETURN = FANN(o) ? fann_get_num_output(FANN(o)) : -1;
}

static MFUN(neurons)
{
  *(m_uint*)RETURN = FANN(o) ? fann_get_total_neurons(FANN(o)) : -1;
}

static MFUN(connections)
{
  *(m_uint*)RETURN = FANN(o) ? fann_get_total_connections(FANN(o)) : -1;
}

static MFUN(type)
{
  *(m_uint*)RETURN = FANN(o) ? fann_get_network_type(FANN(o)) : -1;
}

static MFUN(rate)
{
  *(m_float*)RETURN = FANN(o) ? fann_get_connection_rate(FANN(o)) : -1;
}

static MFUN(layer)
{
  *(m_uint*)RETURN = FANN(o) ? fann_get_num_layers(FANN(o)) : -1;
}

static MFUN(layers)
{
  if(!FANN(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_num_layers(FANN(o));
  M_Object ret = new_M_Array(SZ_INT, size, 1);
  unsigned int j[size];
  fann_get_layer_array(FANN(o), j);
  for(i = 0; i < size; i++)
    i_vector_set(ARRAY(ret), i, j[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}
static MFUN(bias)
{
  if(!FANN(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_num_layers(FANN(o));
  M_Object ret = new_M_Array(SZ_INT, size, 1);
  unsigned int j[size];
  fann_get_bias_array(FANN(o), j);
  for(i = 0; i < size; i++)
    i_vector_set(ARRAY(ret), i, j[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static struct fann_connection to_fann(M_Object o)
{
  struct fann_connection c;
  c.from_neuron = *(m_uint*)(o->data + o_fann_from);
  c.to_neuron   = *(m_uint*)(o->data + o_fann_to);
  c.weight      = *(m_float*)(o->data + o_fann_weight);
  return c;
}
static M_Object from_fann(struct fann_connection c)
{
  M_Object o= new_M_Object(NULL);
  initialize_object(o, &t_fann_connect);
  *(m_uint*)(o->data + o_fann_from)    = c.from_neuron;
  *(m_uint*)(o->data + o_fann_to)      = c.to_neuron;
  *(m_float*)(o->data + o_fann_weight) = c.weight;
  return o;
}
static MFUN(connection_array)
{
  if(!FANN(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_total_connections(FANN(o));
  M_Object ret = new_M_Array(SZ_INT, size, 1);
  struct fann_connection c[size];
  fann_get_connection_array(FANN(o), c);
  for(i= 0; i < size; i++)
    i_vector_set(ARRAY(ret), i, (m_uint)from_fann(c[i]));
  *(m_uint*)RETURN = (m_uint)ret;
}
static MFUN(weigth_array)
{
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  struct fann_connection c[size];
  for(i = 0; i < size; i++)
    c[i] = to_fann((M_Object)i_vector_at(ARRAY(obj), i));
  fann_set_weight_array(FANN(o), c, size);
}

static MFUN(weigth)
{
  m_int   from   = *(m_int*)MEM(SZ_INT);
  m_int   to = *(m_int*)MEM(SZ_INT*2);
  m_float f    = *(m_float*)MEM(SZ_INT*3);
  fann_set_weight(FANN(o), from, to, f);
}

static MFUN(get_weigths)
{
  if(!FANN(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_total_connections(FANN(o));
  M_Object ret = new_M_Array(SZ_FLOAT, size, 1);
  m_float f[size];
  fann_get_weights(FANN(o), f);
  for(i = 0; i < size; i++)
    f_vector_set(ARRAY(ret), i, f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(set_weigths)
{
  if(!FANN(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_total_connections(FANN(o));
  M_Object ret = *(M_Object*)MEM(SZ_INT);
  if(m_vector_size(ARRAY(ret)) < size)
  if(!FANN(o))
  {
    err_msg(INSTR_, 0, "invalid array size for weights (%i). should be %i", m_vector_size(ARRAY(ret)), size);
    *(m_uint*)RETURN = 0;
    return;
  }
  m_float f[size];
  for(i = 0; i < size; i++)
    f[i] = f_vector_at(ARRAY(ret), i);
  fann_set_weights(FANN(o), f);
  *(m_uint*)RETURN = (m_uint)ret;
}
// get/set user data
static SFUN(disable_seed)
{
  fann_disable_seed_rand();
}

static SFUN(enable_seed)
{
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
static SFUN(type_str)
{
  m_int i = *(m_int*)MEM(SZ_INT);
  if(i < 0 || i > 1)
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  *(m_uint*)RETURN = (m_uint)new_String(shred, (m_str)FANN_NETTYPE_NAMES[i]);
}

static MFUN(load)
{
  FANN(o) = fann_create_from_file(STRING(MEM(SZ_INT)));
}

static MFUN(save)
{
  fann_save(FANN(o), STRING(MEM(SZ_INT)));
}
// training
static MFUN(train)
{
  m_uint i;
  M_Object o_in  = *(M_Object*)MEM(SZ_INT);
  M_Object o_out = *(M_Object*)MEM(SZ_INT*2);
  m_uint   s_in  = m_vector_size(ARRAY(o_in));
  m_uint   s_out  = m_vector_size(ARRAY(o_out));
  m_float in[s_in], out[s_out];
  for(i = 0; i < s_in; i++)
    in[i] = f_vector_at(ARRAY(o_in), i);
  for(i = 0; i < s_out; i++)
    in[i] = f_vector_at(ARRAY(o_out), i);
  fann_train(FANN(o), in, out);
}

static MFUN(test)
{
  m_uint i;
  M_Object o_in  = *(M_Object*)MEM(SZ_INT);
  M_Object o_out = *(M_Object*)MEM(SZ_INT*2);
  m_uint   s_in  = m_vector_size(ARRAY(o_in));
  m_uint   s_out = m_vector_size(ARRAY(o_out));
  m_uint s_ret   = fann_get_num_output(FANN(o));
  m_float in[s_in], out[s_out];
  for(i = 0; i < s_in; i++)
    in[i] = f_vector_at(ARRAY(o_in), i);
  for(i = 0; i < s_out; i++)
    in[i] = f_vector_at(ARRAY(o_out), i);
  m_float* f = fann_test(FANN(o), in, out);
  M_Object ret = new_M_Array(SZ_FLOAT, s_ret, 1);
  for(i = 0; i < s_ret; i++)
    f_vector_set(ARRAY(o_out), i, f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(get_MSE)
{
  *(m_float*)RETURN = fann_get_MSE(FANN(o));
}

static MFUN(get_bit_fail)
{
  *(m_uint*)RETURN = fann_get_bit_fail(FANN(o));
}

static MFUN(reset_MSE)
{
  fann_reset_MSE(FANN(o));
}

static MFUN(run)
{
  m_uint i, size;
  if(!FANN(o) || !DATA(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  M_Object array    = *(M_Object*)MEM(SZ_INT);
  size = m_vector_size(ARRAY(array));
  m_float  ptr[size];
  for(i = 0; i < size; i++)
    ptr[i] = i_vector_at(ARRAY(array), i);
  M_Object ret = new_M_Array(SZ_FLOAT, fann_get_num_output(FANN(o)), 1);
  m_float *f = fann_run(FANN(o), ptr);
  for(i = 0; i < fann_get_num_output(FANN(o)); i++)
    f_vector_set(ARRAY(ret), i, f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

// Training Data Manipulation
static MFUN(scale_train)
{
  M_Object data   = *(M_Object*)MEM(SZ_INT);
  fann_scale_train(FANN(o),DATA(data));
}

static MFUN(descale_train)
{
  M_Object data   = *(M_Object*)MEM(SZ_INT);
  fann_descale_train(FANN(o),DATA(data));
}

static MFUN(input_scaling_params)
{
  M_Object data  = *(M_Object*)MEM(SZ_INT);
  m_float min    = *(m_float*)MEM(SZ_INT*2);
  m_float max    = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT);
  *(m_uint*)RETURN = fann_set_input_scaling_params(FANN(o), DATA(data), min, max);
}

static MFUN(output_scaling_params)
{
  M_Object data  = *(M_Object*)MEM(SZ_INT);
  m_float min    = *(m_float*)MEM(SZ_INT*2);
  m_float max    = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT);
  *(m_uint*)RETURN = fann_set_output_scaling_params(FANN(o), DATA(data), min, max);
}

static MFUN(scaling_params)
{
  M_Object data  = *(M_Object*)MEM(SZ_INT);
  m_float imin   = *(m_float*)MEM(SZ_INT*2);
  m_float imax   = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT);
  m_float omin   = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT*2);
  m_float omax   = *(m_float*)MEM(SZ_INT*2 + SZ_FLOAT*3);
  *(m_uint*)RETURN = fann_set_scaling_params(FANN(o), DATA(data), imin, imax, omin, omax);
}
static MFUN(clear_scaling_params)
{
  *(m_uint*)RETURN = fann_clear_scaling_params(FANN(o));
}

static MFUN(scale_input)
{
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  m_float f[size];
  for(i = 0; i < size; i++)
    f[i] = f_vector_at(ARRAY(obj), i);
  fann_scale_input(FANN(o), f);
}

static MFUN(scale_output)
{
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  m_float f[size];
  for(i = 0; i < size; i++)
    f[i] = f_vector_at(ARRAY(obj), i);
  fann_scale_output(FANN(o), f);
}

static MFUN(descale_input)
{
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  m_float f[size];
  for(i = 0; i < size; i++)
    f[i] = f_vector_at(ARRAY(obj), i);
  fann_descale_input(FANN(o), f);
  for(i = 0; i < size; i++)
    f_vector_set(ARRAY(obj), i, f[i]);
}

static MFUN(descale_output)
{
  if(!FANN(o))
    return;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_uint i, size = m_vector_size(ARRAY(obj));
  m_float f[size];
  for(i = 0; i < size; i++)
    f[i] = f_vector_at(ARRAY(obj), i);
  fann_descale_output(FANN(o), f);
  for(i = 0; i < size; i++)
    f_vector_set(ARRAY(obj), i, f[i]);
}

// Training Data Training
static MFUN(train_on_data)
{
  M_Object data   = *(M_Object*)MEM(SZ_INT);
  m_uint   epoch  = *(m_uint*)MEM(SZ_INT*2);
  m_uint   report = *(m_uint*)MEM(SZ_INT*3);
  m_float  error  = *(m_uint*)MEM(SZ_INT*4);
  fann_train_on_data(FANN(o), DATA(data), epoch, report, error);
}

static MFUN(train_on_file)
{
  M_Object data   = *(M_Object*)MEM(SZ_INT);
  m_uint   epoch  = *(m_uint*)MEM(SZ_INT*2);
  m_uint   report = *(m_uint*)MEM(SZ_INT*3);
  m_float  error  = *(m_uint*)MEM(SZ_INT*4);
  fann_train_on_file(FANN(o), STRING(data), epoch, report, error);
}

static MFUN(train_epoch)
{
  M_Object data = *(M_Object*)MEM(SZ_INT);
  *(m_float*)RETURN = fann_train_epoch(FANN(o), DATA(data));
}

static MFUN(test_data)
{
  M_Object data = *(M_Object*)MEM(SZ_INT);
  *(m_float*)RETURN = fann_test_data(FANN(o), DATA(data));
}
// Training Data Manipulation
static MFUN(train_from_file)
{
  if(DATA(o))
    fann_destroy_train(DATA(o));
  fann_read_train_from_file(STRING(*(M_Object*)MEM(SZ_INT)));
}

static MFUN(train_from_array)
{
  if(DATA(o))
    fann_destroy_train(DATA(o));
  m_uint i, j, y, in_size, out_size;
  m_uint  num_data = *(m_uint*)MEM(SZ_INT);
  M_Object obj;
  M_Object  in_obj = *(M_Object*)MEM(SZ_INT*2);
  M_Object out_obj = *(M_Object*)MEM(SZ_INT*3);
  in_size  = m_vector_size(ARRAY(in_obj));
  m_float* in[in_size];
  for(i = 0; i < in_size; i++)
  {
    obj = (M_Object)i_vector_at(ARRAY(in_obj), i);
    y = m_vector_size(ARRAY(obj));
    m_float f[y];
    in[i] = f;
    for(j = 0; j < y; j++)
      f[j] = f_vector_at(ARRAY(obj), j);
  }
  out_size = m_vector_size(ARRAY(out_obj));
  m_float* out[out_size];
  for(i = 0; i < out_size; i++)
  {
    obj = (M_Object)i_vector_at(ARRAY(out_obj), i);
    y = m_vector_size(ARRAY(obj));
    m_float f[y];
    out[i] = f;
    for(j = 0; j < y; j++)
      f[j] = f_vector_at(ARRAY(obj), j);
  }
  DATA(o) = fann_create_train_pointer_array(num_data, in_size, in, out_size, out);
}

static MFUN(train_create)
{
  if(DATA(o))
    fann_destroy_train(DATA(o));
  DATA(o) = fann_create_train(*(m_uint*)MEM(SZ_INT),  *(m_uint*)MEM(SZ_INT*2), *(m_uint*)MEM(SZ_INT*3));
}

static MFUN(train_input)
{
  if(!DATA(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_float* f = fann_get_train_input(DATA(o), *(m_uint*)MEM(SZ_INT));
  m_uint i, size = sizeof(f)/sizeof(m_float);
  M_Object ret = new_M_Array(SZ_FLOAT, size, 1);
  for(i = 0; i < size; i++)
    f_vector_set(ARRAY(ret), i, f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(train_output)
{
  if(!DATA(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_float* f = fann_get_train_output(DATA(o), *(m_uint*)MEM(SZ_INT));
  m_uint i, size = sizeof(f)/sizeof(m_float);
  M_Object ret = new_M_Array(SZ_FLOAT, size, 1);
  for(i = 0; i < size; i++)
    f_vector_set(ARRAY(ret), i, f[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(train_shuffle)
{
  if(!DATA(o))
    return;
  fann_shuffle_train_data(DATA(o));
}

static MFUN(train_min_in)
{
  if(!DATA(o))
  {
    *(m_float*)RETURN = NAN;
    return;
  }
  *(m_float*)RETURN = fann_get_min_train_input(DATA(o));
}

static MFUN(train_max_in)
{
  if(!DATA(o))
  {
    *(m_float*)RETURN = NAN;
    return;
  }
  *(m_float*)RETURN = fann_get_max_train_input(DATA(o));
}

static MFUN(train_min_out)
{
  if(!DATA(o))
  {
    *(m_float*)RETURN = NAN;
    return;
  }
  *(m_float*)RETURN = fann_get_min_train_output(DATA(o));
}

static MFUN(train_max_out)
{
  if(!DATA(o))
  {
    *(m_float*)RETURN = NAN;
    return;
  }
  *(m_float*)RETURN = fann_get_max_train_output(DATA(o));
}

static MFUN(train_input_scale)
{
  m_float min    = *(m_float*)MEM(SZ_INT);
  m_float max    = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  fann_scale_input_train_data(DATA(o), min, max);
}

static MFUN(train_output_scale)
{
  m_float min    = *(m_float*)MEM(SZ_INT);
  m_float max    = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  fann_scale_output_train_data(DATA(o), min, max);
}

static MFUN(train_scale)
{
  m_float min    = *(m_float*)MEM(SZ_INT);
  m_float max    = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
  fann_scale_train_data(DATA(o), min, max);
}

static MFUN(train_length)
{
  *(m_uint*)RETURN = fann_length_train_data(DATA(o));
}

static MFUN(train_num_input)
{
  *(m_uint*)RETURN = fann_num_input_train_data(DATA(o));
}

static MFUN(train_num_output)
{
  *(m_uint*)RETURN = fann_num_input_train_data(DATA(o));
}

static MFUN(train_save)
{
  M_Object file = *(M_Object*)MEM(SZ_INT);
  *(m_uint*)RETURN = fann_save_train(DATA(o), STRING(file));
}

static SFUN(train_merge)
{
  M_Object ret = new_M_Object(shred);
  M_Object l = *(M_Object*)MEM(SZ_INT);
  M_Object r = *(M_Object*)MEM(SZ_INT*2);
  initialize_object(ret, &t_fann_data);
  DATA(ret) = fann_merge_train_data(DATA(l), DATA(r));
  *(m_uint*)RETURN = (m_uint)ret;
}

static SFUN(train_duplicate)
{
  M_Object ret = new_M_Object(shred);
  M_Object l = *(M_Object*)MEM(SZ_INT);
  M_Object r = *(M_Object*)MEM(SZ_INT*2);
  initialize_object(ret, &t_fann_data);
  DATA(ret) = fann_duplicate_train_data(DATA(l));
  *(m_uint*)RETURN = (m_uint)ret;
}

static MFUN(train_do_subset)
{
  M_Object ret = new_M_Object(shred);
  m_uint pos = *(m_uint*)MEM(SZ_INT);
  m_uint len = *(m_uint*)MEM(SZ_INT*2);
  initialize_object(ret, &t_fann_data);
  DATA(ret) = fann_subset_train_data(DATA(o), pos, len);
  *(m_uint*)RETURN = (m_uint)ret;
}

// fann parameters
MFUN(get_training_algorithm)
{
  *(m_uint*)RETURN = fann_get_training_algorithm(FANN(o));
}

MFUN(set_training_algorithm)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_training_algorithm(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_learning_rate)
{
  *(m_float*)RETURN = fann_get_learning_rate(FANN(o));
}

MFUN(set_learning_rate)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_learning_rate(FANN(o), *(m_float*)RETURN);
}


MFUN(get_learning_momentum)
{
  *(m_float*)RETURN = fann_get_learning_momentum(FANN(o));
}

MFUN(set_learning_momentum)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_learning_momentum(FANN(o), *(m_float*)RETURN);
}

MFUN(get_activation_function)
{
  *(m_uint*)RETURN = fann_get_activation_function(FANN(o),
    *(m_int*)MEM(SZ_INT), *(m_int*)MEM(SZ_INT*2));
}

MFUN(set_activation_function)
{
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_function(FANN(o), *(m_uint*)RETURN,
    *(m_int*)MEM(SZ_INT*2), *(m_int*)MEM(SZ_INT*3));
}
MFUN(set_activation_function_layer)
{
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_function_layer(FANN(o), *(m_uint*)RETURN,
    *(m_int*)MEM(SZ_INT*2));
}

MFUN(set_activation_function_hidden)
{
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_function_hidden(FANN(o), *(m_uint*)RETURN);
}

MFUN(set_activation_function_output)
{
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_function_output(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_activation_steepness)
{
  *(m_uint*)RETURN = fann_get_activation_steepness(FANN(o),
    *(m_int*)MEM(SZ_INT), *(m_int*)MEM(SZ_INT*2));
}

MFUN(set_activation_steepness)
{
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_steepness(FANN(o), *(m_uint*)RETURN,
    *(m_int*)MEM(SZ_INT*2), *(m_int*)MEM(SZ_INT*3));
}
MFUN(set_activation_steepness_layer)
{
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_steepness_layer(FANN(o), *(m_uint*)RETURN,
    *(m_int*)MEM(SZ_INT*2));
}

MFUN(set_activation_steepness_hidden)
{
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_steepness_hidden(FANN(o), *(m_uint*)RETURN);
}

MFUN(set_activation_steepness_output)
{
  *(m_uint*)RETURN = *(m_int*)MEM(SZ_INT);
  fann_set_activation_steepness_output(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_train_error_function)
{
  *(m_uint*)RETURN = fann_get_train_error_function(FANN(o));
}

MFUN(set_train_error_function)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_train_error_function(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_train_stop_function)
{
  *(m_uint*)RETURN = fann_get_train_stop_function(FANN(o));
}

MFUN(set_train_stop_function)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_train_stop_function(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_bit_fail_limit)
{
  *(m_float*)RETURN = fann_get_bit_fail_limit(FANN(o));
}

MFUN(set_bit_fail_limit)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_bit_fail_limit(FANN(o), *(m_uint*)RETURN);
}
// callback

MFUN(get_quickprop_decay)
{
  *(m_float*)RETURN = fann_get_quickprop_decay(FANN(o));
}

MFUN(set_quickprop_decay)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_quickprop_decay(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_quickprop_mu)
{
  *(m_float*)RETURN = fann_get_quickprop_mu(FANN(o));
}

MFUN(set_quickprop_mu)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_quickprop_mu(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_increase_factor)
{
  *(m_float*)RETURN = fann_get_rprop_increase_factor(FANN(o));
}

MFUN(set_rprop_increase_factor)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_increase_factor(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_decrease_factor)
{
  *(m_float*)RETURN = fann_get_rprop_decrease_factor(FANN(o));
}

MFUN(set_rprop_decrease_factor)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_decrease_factor(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_delta_min)
{
  *(m_float*)RETURN = fann_get_rprop_delta_min(FANN(o));
}

MFUN(set_rprop_delta_min)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_delta_min(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_delta_max)
{
  *(m_float*)RETURN = fann_get_rprop_delta_max(FANN(o));
}

MFUN(set_rprop_delta_max)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_delta_max(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_rprop_delta_zero)
{
  *(m_float*)RETURN = fann_get_rprop_delta_zero(FANN(o));
}

MFUN(set_rprop_delta_zero)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_rprop_delta_zero(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_sarprop_weight_decay_shift)
{
  *(m_float*)RETURN = fann_get_sarprop_weight_decay_shift(FANN(o));
}

MFUN(set_sarprop_weight_decay_shift)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_sarprop_weight_decay_shift(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_sarprop_step_error_threshold_factor)
{
  *(m_float*)RETURN = fann_get_sarprop_step_error_threshold_factor(FANN(o));
}

MFUN(set_sarprop_step_error_threshold_factor)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_sarprop_step_error_threshold_factor(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_sarprop_step_error_shift)
{
  *(m_float*)RETURN = fann_get_sarprop_step_error_shift(FANN(o));
}

MFUN(set_sarprop_step_error_shift)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_sarprop_step_error_shift(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_sarprop_temperature)
{
  *(m_float*)RETURN = fann_get_sarprop_temperature(FANN(o));
}

MFUN(set_sarprop_temperature)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_sarprop_temperature(FANN(o), *(m_uint*)RETURN);
}

// cascade parameters
MFUN(get_cascade_output_change_fraction)
{
  *(m_float*)RETURN = fann_get_cascade_output_change_fraction(FANN(o));
}

MFUN(set_cascade_output_change_fraction)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_cascade_output_change_fraction(FANN(o), *(m_uint*)RETURN);
}


MFUN(get_cascade_output_stagnation_epochs)
{
  *(m_uint*)RETURN = fann_get_cascade_output_stagnation_epochs(FANN(o));
}

MFUN(set_cascade_output_stagnation_epochs)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_output_stagnation_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_candidate_change_fraction)
{
  *(m_float*)RETURN = fann_get_cascade_candidate_change_fraction(FANN(o));
}

MFUN(set_cascade_candidate_change_fraction)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_cascade_candidate_change_fraction(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_candidate_stagnation_epochs)
{
  *(m_uint*)RETURN = fann_get_cascade_candidate_stagnation_epochs(FANN(o));
}

MFUN(set_cascade_candidate_stagnation_epochs)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_candidate_stagnation_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_weight_multiplier)
{
  *(m_float*)RETURN = fann_get_cascade_weight_multiplier(FANN(o));
}

MFUN(set_cascade_weight_multiplier)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_cascade_weight_multiplier(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_candidate_limit)
{
  *(m_float*)RETURN = fann_get_cascade_candidate_limit(FANN(o));
}

MFUN(set_cascade_candidate_limit)
{
  *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  fann_set_cascade_candidate_limit(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_max_out_epochs)
{
  *(m_uint*)RETURN = fann_get_cascade_max_out_epochs(FANN(o));
}

MFUN(set_cascade_max_out_epochs)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_max_out_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_min_out_epochs)
{
  *(m_uint*)RETURN = fann_get_cascade_min_out_epochs(FANN(o));
}

MFUN(set_cascade_min_out_epochs)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_min_out_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_max_cand_epochs)
{
  *(m_uint*)RETURN = fann_get_cascade_max_cand_epochs(FANN(o));
}

MFUN(set_cascade_max_cand_epochs)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_max_cand_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_min_cand_epochs)
{
  *(m_uint*)RETURN = fann_get_cascade_min_cand_epochs(FANN(o));
}

MFUN(set_cascade_min_cand_epochs)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_min_cand_epochs(FANN(o), *(m_uint*)RETURN);
}

MFUN(get_cascade_activation_functions_count)
{
  *(m_uint*)RETURN = fann_get_cascade_activation_functions_count(FANN(o));
}

MFUN(get_cascade_activation_functions)
{
  if(!FANN(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_cascade_activation_functions_count(FANN(o));
  enum fann_activationfunc_enum * tmp = fann_get_cascade_activation_functions(FANN(o));
  M_Object ret = new_M_Array(SZ_INT, size, 1);
  for(i=0; i < size; i++)
    i_vector_set(ARRAY(ret), i, tmp[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

MFUN(set_cascade_activation_functions)
{
  if(!FANN(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  M_Object ret = *(M_Object*)MEM(SZ_INT);
  m_uint index = *(m_uint*)MEM(SZ_INT*2);
  m_uint i, size = m_vector_size(ARRAY(ret));
  enum fann_activationfunc_enum tmp[size];
  for(i=0; i < size; i++)
    tmp[i] = i_vector_at(ARRAY(ret), i);
  fann_set_cascade_activation_functions(FANN(o), tmp, size);
  *(m_uint*)RETURN = (m_uint)ret;
}

MFUN(get_cascade_activation_steepnesses_count)
{
  *(m_uint*)RETURN = fann_get_cascade_activation_steepnesses_count(FANN(o));
}

MFUN(get_cascade_activation_steepnesses)
{
  if(!FANN(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_uint i, size = fann_get_cascade_activation_steepnesses_count(FANN(o));
  m_float* tmp = fann_get_cascade_activation_steepnesses(FANN(o));
  M_Object ret = new_M_Array(SZ_INT, size, 1);
  for(i=0; i < size; i++)
    f_vector_set(ARRAY(ret), i, tmp[i]);
  *(m_uint*)RETURN = (m_uint)ret;
}

MFUN(set_cascade_activation_steepnesses)
{
  if(!FANN(o))
  {
    *(m_uint*)RETURN = 0;
    return;
  }
  M_Object ret = *(M_Object*)MEM(SZ_INT);
  m_uint index = *(m_uint*)MEM(SZ_INT*2);
  m_uint i, size = m_vector_size(ARRAY(ret));
  m_float tmp[size];
  for(i=0; i < size; i++)
    tmp[i] = f_vector_at(ARRAY(ret), i);
  fann_set_cascade_activation_steepnesses(FANN(o), tmp, size);
  *(m_uint*)RETURN = (m_uint)ret;
}

MFUN(get_cascade_num_candidate_groups)
{
  *(m_uint*)RETURN = fann_get_cascade_num_candidate_groups(FANN(o));
}

MFUN(set_cascade_num_candidate_groups)
{
  *(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT);
  fann_set_cascade_num_candidate_groups(FANN(o), *(m_uint*)RETURN);
}

m_bool import(Importer importer) {
  CHECK_BB(importer_class_ini(importer, &t_fann_connect, NULL, NULL))
	importer_item_ini(importer,"int",  "from");
  o_fann_from = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_fann_from)
	importer_item_ini(importer,"int",  "to");
  o_fann_to = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_fann_to)
	importer_item_ini(importer,"int",  "weight");
  o_fann_weight = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_fann_weight)
  CHECK_BB(importer_class_end(importer))

  // this is for error handling
  CHECK_BB(importer_class_ini(importer, &t_fann_base, NULL, NULL))
	importer_item_ini(importer,"int",  "@data");
  o_fann_error = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_fann_error)
  importer_func_ini(importer, "void", "log",  (m_uint)error_log);
    importer_func_arg(importer, "FileIO", "f");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "errno",  (m_uint)errno);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "reset_errno",  (m_uint)reset_errno);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "reset_errstr",  (m_uint)reset_errstr);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "errstr",  (m_uint)errstr);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "print_error",  (m_uint)print_error);
  CHECK_BB(importer_func_end(importer, 0))
  CHECK_BB(importer_class_end(importer))

//  Training Data Manipulation
  CHECK_BB(importer_class_ini(importer, &t_fann_data, NULL, data_dtor))
  importer_func_ini(importer, "void", "from_data",  (m_uint)train_from_array);
    importer_func_arg(importer, "int", "num");
    importer_func_arg(importer, "float[][]", "in");
    importer_func_arg(importer, "float[][]", "out");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "load",  (m_uint)train_from_file);
    importer_func_arg(importer, "string", "filename");
    importer_func_arg(importer, "int", "epochs");
    importer_func_arg(importer, "int", "report");
    importer_func_arg(importer, "float", "error");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "save",  (m_uint)train_save);
    importer_func_arg(importer, "string", "filename");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "create",  (m_uint)train_create);
    importer_func_arg(importer, "int", "num_data");
    importer_func_arg(importer, "int", "num_in");
    importer_func_arg(importer, "int", "num_out");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float[]", "input",  (m_uint)train_input);
    importer_func_arg(importer, "int", "position");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float[]", "output",  (m_uint)train_output);
    importer_func_arg(importer, "int", "position");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "shuffle",  (m_uint)train_shuffle);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "min_in",  (m_uint)train_min_in);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "max_in",  (m_uint)train_max_in);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "max_in",  (m_uint)train_max_in);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "max_out",  (m_uint)train_max_out);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "length",  (m_uint)train_length);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "num_input",  (m_uint)train_num_input);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "num_ouput",  (m_uint)train_num_output);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "FANN_data", "subset",  (m_uint)train_do_subset);
    importer_func_arg(importer, "int", "pos");
    importer_func_arg(importer, "int", "len");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "FANN_data", "merge",  (m_uint)train_merge);
    importer_func_arg(importer, "FANN_data", "l");
    importer_func_arg(importer, "FANN_data", "r");
  CHECK_BB(importer_func_end(importer, ae_flag_static))
  importer_func_ini(importer, "FANN_data", "duplicate",  (m_uint)train_duplicate);
    importer_func_arg(importer, "FANN_data", "l");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  CHECK_BB(importer_class_end(importer))

  CHECK_BB(importer_class_ini(importer, &t_fann, NULL, fann_dtor))
  importer_func_ini(importer, "string", "type_str", (m_uint)type_str);
    importer_func_arg(importer, "int", "layer");
  CHECK_BB(importer_func_end(importer, ae_flag_static))
  importer_func_ini(importer, "void", "standard", (m_uint)standard);
    importer_func_arg(importer, "int", "layer");
    importer_func_arg(importer, "int[]", "outputs");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "shortcut",  (m_uint)shortcut);
    importer_func_arg(importer, "int", "layer");
    importer_func_arg(importer, "int[]", "outputs");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "sparse",   (m_uint)sparse);
    importer_func_arg(importer, "float", "rate");
    importer_func_arg(importer, "int", "layer");
    importer_func_arg(importer, "int[]", "outputs");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "randomize",  (m_uint)randomize);
    importer_func_arg(importer, "float", "min");
    importer_func_arg(importer, "float", "max");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "init",  (m_uint)init);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "print",  (m_uint)print_connections);
  CHECK_BB(importer_func_end(importer, 0))
  // paramters
  importer_func_ini(importer, "void", "parameters",  (m_uint)parameters);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "input",  (m_uint)input);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "output",  (m_uint)output);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "neurons",  (m_uint)neurons);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "connections",  (m_uint)connections);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "type",  (m_uint)type);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rate",  (m_uint)rate);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "layer",  (m_uint)layer);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int[]", "layers",  (m_uint)layers);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int[]", "bias",  (m_uint)bias);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "FANN_connect[]", "connection_array",  (m_uint)connection_array);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "weigth_array",  (m_uint)weigth_array);
    importer_func_arg(importer, "FANN_connect[]", "array");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float[]", "weigth",  (m_uint)weigth);
    importer_func_arg(importer, "int", "from");
    importer_func_arg(importer, "int", "to");
    importer_func_arg(importer, "float", "weigth");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float[]", "weigths",  (m_uint)get_weigths);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float[]", "weigths",  (m_uint)set_weigths);
    importer_func_arg(importer, "float[]", "f");
  CHECK_BB(importer_func_end(importer, 0))
// get/set user data
  importer_func_ini(importer, "void", "disable_seed",  (m_uint)disable_seed);
  CHECK_BB(importer_func_end(importer, ae_flag_static))
  importer_func_ini(importer, "void", "enable_seed",  (m_uint)enable_seed);
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "void", "load",  (m_uint)load);
  importer_func_arg(importer, "string", "file");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "save",  (m_uint)save);
    importer_func_arg(importer, "string", "file");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float[]", "run",  (m_uint)run);
    importer_func_arg(importer, "float[]", "in");
  CHECK_BB(importer_func_end(importer, 0))
  // FIXED POINT only
/*  importer_func_ini(importer, "void", "decimal_point",  (m_uint)decimal_point);*/
/*  CHECK_BB(importer_func_end(importer, 0))*/
/*  importer_func_ini(importer, "void", "multiplier",  (m_uint)multiplier);*/
/*  CHECK_BB(importer_func_end(importer, 0))*/

// training
  importer_func_ini(importer, "void", "train",  (m_uint)train);
    importer_func_arg(importer, "float[]", "in");
    importer_func_arg(importer, "float[]", "out");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float[]", "test",  (m_uint)test);
    importer_func_arg(importer, "float[]", "in");
    importer_func_arg(importer, "float[]", "out");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "mse",  (m_uint)get_MSE);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "bit_fail",  (m_uint)get_bit_fail);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "reset_mse",  (m_uint)reset_MSE);
  CHECK_BB(importer_func_end(importer, 0))

// Training Data Training
  importer_func_ini(importer, "void", "on_data",  (m_uint)train_on_data);
    importer_func_arg(importer, "FANN_data", "data");
    importer_func_arg(importer, "int",   "epoch");
    importer_func_arg(importer, "int",   "report");
    importer_func_arg(importer, "float", "error");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "on_file",  (m_uint)train_on_file);
    importer_func_arg(importer, "string", "filename");
    importer_func_arg(importer, "int",   "epoch");
    importer_func_arg(importer, "int",   "report");
    importer_func_arg(importer, "float", "error");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "epoch",  (m_uint)train_epoch);
    importer_func_arg(importer, "FANN_data", "data");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "epoch",  (m_uint)test_data);
    importer_func_arg(importer, "FANN_data", "data");
  CHECK_BB(importer_func_end(importer, 0))
/*  importer_func_ini(importer, "void", "test",  (m_uint)train_test);*/
/*    importer_func_arg(importer, "FANN_data", "data");*/
/*  CHECK_BB(importer_func_end(importer, 0))*/

// Training Data Manipulation
  importer_func_ini(importer, "void", "scale",  (m_uint)scale_train);
    importer_func_arg(importer, "FANN_data", "data");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "descale",  (m_uint)descale_train);
    importer_func_arg(importer, "FANN_data", "data");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "in_scaling_params",  (m_uint)input_scaling_params);
    importer_func_arg(importer, "FANN_data", "data");
    importer_func_arg(importer, "float", "min");
    importer_func_arg(importer, "float", "max");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "out_scaling_params",  (m_uint)output_scaling_params);
    importer_func_arg(importer, "FANN_data", "data");
    importer_func_arg(importer, "float", "min");
    importer_func_arg(importer, "float", "max");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "scaling_params",  (m_uint)scaling_params);
    importer_func_arg(importer, "FANN_data", "data");
    importer_func_arg(importer, "float", "imin");
    importer_func_arg(importer, "float", "imax");
    importer_func_arg(importer, "float", "omin");
    importer_func_arg(importer, "float", "omax");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "clear_scaling_params",  (m_uint)scaling_params);
  CHECK_BB(importer_func_end(importer, 0))
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "scale_input",  (m_uint)scale_input);
    importer_func_arg(importer, "float[]", "in");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "scale_output",  (m_uint)scale_output);
    importer_func_arg(importer, "float[]", "out");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "descale_input",  (m_uint)descale_input);
    importer_func_arg(importer, "float[]", "in");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "descale_output",  (m_uint)descale_output);
    importer_func_arg(importer, "float[]", "out");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "scale_in",  (m_uint)train_input_scale);
    importer_func_arg(importer, "FANN_data", "data");
    importer_func_arg(importer, "float", "min");
    importer_func_arg(importer, "float", "max");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "scale_out",  (m_uint)train_output_scale);
    importer_func_arg(importer, "FANN_data", "data");
    importer_func_arg(importer, "float", "min");
    importer_func_arg(importer, "float", "max");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "void", "scale",  (m_uint)train_scale);
    importer_func_arg(importer, "FANN_data", "data");
    importer_func_arg(importer, "float", "min");
    importer_func_arg(importer, "float", "max");
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "int", "training_algorithm",  (m_uint)get_training_algorithm);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "training_algorithm",  (m_uint)set_training_algorithm);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "learning_rate",  (m_uint)get_learning_rate);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "learning_rate",  (m_uint)set_learning_rate);
    importer_func_arg(importer, "float", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "learning_momentum",  (m_uint)get_learning_momentum);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "learning_momentum",  (m_uint)set_learning_momentum);
    importer_func_arg(importer, "float", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "activation_function",  (m_uint)get_activation_function);
    importer_func_arg(importer, "int", "layer");
    importer_func_arg(importer, "int", "neuron");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "activation_function",  (m_uint)set_activation_function);
    importer_func_arg(importer, "int", "arg");
    importer_func_arg(importer, "int", "layer");
    importer_func_arg(importer, "int", "neuron");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "activation_function_layer",  (m_uint)set_activation_function_layer);
    importer_func_arg(importer, "int", "layer");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "activation_function_output",  (m_uint)set_activation_function_output);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "activation_steepness",  (m_uint)get_activation_steepness);
    importer_func_arg(importer, "int", "layer");
    importer_func_arg(importer, "int", "neuron");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "activation_steepness",  (m_uint)set_activation_steepness);
    importer_func_arg(importer, "int", "arg");
    importer_func_arg(importer, "int", "layer");
    importer_func_arg(importer, "int", "neuron");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "activation_steepness_layer",  (m_uint)set_activation_steepness_layer);
    importer_func_arg(importer, "int", "layer");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "activation_steepness_output",  (m_uint)set_activation_steepness_output);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "train_error_function",  (m_uint)get_train_error_function);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "train_error_function",  (m_uint)set_train_error_function);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "train_stop_function",  (m_uint)get_train_stop_function);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "train_stop_function",  (m_uint)set_train_stop_function);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "bit_fail_limit",  (m_uint)get_bit_fail_limit);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "bit_fail_limit",  (m_uint)set_bit_fail_limit);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "quickprop_decay",  (m_uint)get_quickprop_decay);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "quickprop_decay",  (m_uint)set_quickprop_decay);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "quickprop_mu",  (m_uint)get_quickprop_mu);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "quickprop_mu",  (m_uint)set_quickprop_mu);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_increase_factor",  (m_uint)get_rprop_increase_factor);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_increase_factor",  (m_uint)set_rprop_increase_factor);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_decrease_factor",  (m_uint)get_rprop_decrease_factor);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_decrease_factor",  (m_uint)set_rprop_decrease_factor);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_delta_min",  (m_uint)get_rprop_delta_min);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_delta_min",  (m_uint)set_rprop_delta_min);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_delta_max",  (m_uint)get_rprop_delta_max);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_delta_max",  (m_uint)set_rprop_delta_max);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_delta_zero",  (m_uint)get_rprop_delta_zero);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "rprop_delta_zero",  (m_uint)set_rprop_delta_zero);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "sarprop_weight_decay_shift",  (m_uint)get_sarprop_weight_decay_shift);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "sarprop_weight_decay_shift",  (m_uint)set_sarprop_weight_decay_shift);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "sarprop_step_error_threshold_factor",  (m_uint)get_sarprop_step_error_threshold_factor);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "sarprop_step_error_threshold_factor",  (m_uint)set_sarprop_step_error_threshold_factor);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
   importer_func_ini(importer, "float", "sarprop_step_error_shift",  (m_uint)get_sarprop_step_error_shift);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "sarprop_step_error_shift",  (m_uint)set_sarprop_step_error_shift);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "sarprop_temperature",  (m_uint)get_sarprop_temperature);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "sarprop_temperature",  (m_uint)set_sarprop_temperature);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "cascade_output_change_fraction",  (m_uint)get_cascade_output_change_fraction);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "cascade_output_change_fraction",  (m_uint)set_cascade_output_change_fraction);
    importer_func_arg(importer, "float", "arg");
  CHECK_BB(importer_func_end(importer, 0))
   importer_func_ini(importer, "int", "cascade_output_stagnation_epochs",  (m_uint)get_cascade_output_stagnation_epochs);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_output_stagnation_epochs",  (m_uint)set_cascade_output_stagnation_epochs);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "cascade_candidate_change_fraction",  (m_uint)get_cascade_candidate_change_fraction);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "cascade_candidate_change_fraction",  (m_uint)set_cascade_candidate_change_fraction);
    importer_func_arg(importer, "float", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_candidate_stagnation_epochs",  (m_uint)get_cascade_candidate_stagnation_epochs);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_candidate_stagnation_epochs",  (m_uint)set_cascade_candidate_stagnation_epochs);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "cascade_weight_multiplier",  (m_uint)get_cascade_weight_multiplier);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "cascade_weight_multiplier",  (m_uint)set_cascade_weight_multiplier);
    importer_func_arg(importer, "float", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "cascade_candidate_limit",  (m_uint)get_cascade_candidate_limit);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float", "cascade_candidate_limit",  (m_uint)set_cascade_candidate_limit);
    importer_func_arg(importer, "float", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_max_out_epochs",  (m_uint)get_cascade_max_out_epochs);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_max_out_epochs",  (m_uint)set_cascade_max_out_epochs);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_min_out_epochs",  (m_uint)get_cascade_min_out_epochs);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_min_out_epochs",  (m_uint)set_cascade_min_out_epochs);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
   importer_func_ini(importer, "int", "cascade_max_cand_epochs",  (m_uint)get_cascade_max_cand_epochs);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_max_cand_epochs",  (m_uint)set_cascade_max_cand_epochs);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_min_cand_epochs",  (m_uint)get_cascade_min_cand_epochs);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_min_cand_epochs",  (m_uint)set_cascade_min_cand_epochs);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_activation_functions_count",  (m_uint)get_cascade_activation_functions_count);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int[]", "cascade_activation_functions",  (m_uint)get_cascade_activation_functions);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int[]", "cascade_activation_functions",  (m_uint)set_cascade_activation_functions);
    importer_func_arg(importer, "int[]", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_activation_steepnesses_count",  (m_uint)get_cascade_activation_steepnesses_count);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float[]", "cascade_activation_steepnesses",  (m_uint)get_cascade_activation_steepnesses);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "float[]", "cascade_activation_steepnesses",  (m_uint)set_cascade_activation_steepnesses);
    importer_func_arg(importer, "float[]", "arg");
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_num_candidate_groups",  (m_uint)get_cascade_num_candidate_groups);
  CHECK_BB(importer_func_end(importer, 0))
  importer_func_ini(importer, "int", "cascade_num_candidate_groups",  (m_uint)set_cascade_num_candidate_groups);
    importer_func_arg(importer, "int", "arg");
  CHECK_BB(importer_func_end(importer, 0))

  CHECK_BB(importer_class_end(importer))
  return 1;
}
