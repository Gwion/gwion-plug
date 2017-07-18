#include <string.h>
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "instr.h"
#include "import.h"
#include "array.h"
#include "k.h"
// TODO: thread
static struct Type_ t_k = { "K", 0, NULL };

struct M_Vector_ {
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
  m_uint depth;
};

static m_float** gw2c(M_Vector vec, m_uint* x, m_uint* y) {
	m_uint i, j;
	*x = m_vector_size(vec);
  M_Object a = (M_Object)i_vector_at(vec, 0);
	*y = m_vector_size(ARRAY(a));
	m_float** ret = matrix_alloc(*x, *y);
  for(i = 0; i < *x; i++) {
    a = (M_Object)i_vector_at(vec, i);
    for(j = 0; j < *y; j++)
      ret[i][j] = f_vector_at(ARRAY(a), j);
  } 
	return ret;
}

static SFUN(gw_knn)
{
  M_Object data_obj = *(M_Object*)MEM(SZ_INT);
  M_Object labl_obj = *(M_Object*)MEM(SZ_INT*2);
  M_Object inst_obj = *(M_Object*)MEM(SZ_INT*3);
  m_uint k          = *(m_uint*)  MEM(SZ_INT*4);

  m_uint data_x, data_y;
  m_uint inst_x, inst_y;
 
  m_float** data = gw2c(ARRAY(data_obj), &data_x, &data_y);
  m_float** inst = gw2c(ARRAY(inst_obj), &inst_x, &inst_y);
  m_uint n_labl = m_vector_size(ARRAY(labl_obj));
  m_uint* labl = (m_uint*)ARRAY(labl_obj)->ptr;
  m_uint* ret = knn_classify_multi(data_x, data_y, data, n_labl, labl, inst_x, inst, k);


  M_Object ret_obj = new_M_Array(SZ_INT, inst_x, 1);
  *(m_uint*)RETURN = (m_uint)ret_obj;
  vector_add(&shred->gc, (vtype)ret_obj);
  matrix_release(data);
  matrix_release(inst);
  free(ret);
  release(data_obj, shred);
  release(labl_obj, shred);
  release(inst_obj, shred);
}

static SFUN(gw_kmeans)
{
  M_Object data_obj = *(M_Object*)MEM(SZ_INT);
  M_Object cent_obj = *(M_Object*)MEM(SZ_INT*2);
  m_uint   k        = *(m_uint*)  MEM(SZ_INT*3);
  m_uint   initial  = *(m_uint*)  MEM(SZ_INT*4);
  m_float  theta    = *(m_float*) MEM(SZ_INT*5);

  m_uint data_x, data_y;
  m_uint cent_x, cent_y;
  m_float** data = gw2c(ARRAY(data_obj), &data_x, &data_y);
  m_float** cent = gw2c(ARRAY(cent_obj), &cent_x, &cent_y);
  m_uint* ret = kmeans(data_x, data_y, data, k, theta, cent, initial);

  M_Object ret_obj = new_M_Array(SZ_INT, data_x, 1);
  *(m_uint*)RETURN = (m_uint)ret_obj;
  vector_add(&shred->gc, (vtype)ret_obj);
  memcpy(ARRAY(ret_obj)->ptr, ret, data_y * sizeof(m_uint));
  matrix_release(data);
  matrix_release(cent);
  free(ret);
  release(data_obj, shred);
  release(cent_obj, shred);
}

static SFUN(gw_kmeans_refine)
{
  m_uint i;
  M_Object data_obj = *(M_Object*)MEM(SZ_INT);
  m_uint   iter     = *(m_uint*)  MEM(SZ_INT*2);
  m_uint   n_points = *(m_uint*)  MEM(SZ_INT*3);
  m_uint   n_label  = *(m_uint*)  MEM(SZ_INT*4);

  m_uint data_x, data_y;
  m_float** data = gw2c(ARRAY(data_obj), &data_x, &data_y);
  m_float** ret = kmeans_refine(data_x, data_y, data, iter, n_points, n_label);

  M_Object ret_obj = new_M_Array(SZ_INT, data_x, 2);
  *(m_uint*)RETURN = (m_uint)ret_obj;
  vector_add(&shred->gc, (vtype)ret_obj);
  for(i = 0; i < n_label; i++) {
    M_Object obj = new_M_Array(SZ_FLOAT, data_y, 1);
    memcpy(ARRAY(obj)->ptr, ret[i], data_y * sizeof(m_float));
    i_vector_set(ARRAY(ret_obj), i, (m_uint)obj);
    vector_add(&shred->gc, (vtype)obj);
  }
  release(data_obj, shred);
  matrix_release(ret);
  matrix_release(data);
}

IMPORT
{
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_k, env->global_nspc, NULL, NULL))

  dl_func_init(&fun, "int[]", "nn", (m_uint)gw_knn);
    dl_func_add_arg(&fun, "float", "data[][]");
    dl_func_add_arg(&fun, "int", "labels[]");
    dl_func_add_arg(&fun, "float", "instances[][]");
    dl_func_add_arg(&fun, "int", "k");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "int[]", "means", (m_uint)gw_kmeans);
    dl_func_add_arg(&fun, "float", "data[][]");
    dl_func_add_arg(&fun, "float", "centroid[][]");
    dl_func_add_arg(&fun, "int", "k");
    dl_func_add_arg(&fun, "int", "initial_centroid");
    dl_func_add_arg(&fun, "float", "theta");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))
  
  dl_func_init(&fun, "int[]", "fine_means", (m_uint)gw_kmeans_refine);
    dl_func_add_arg(&fun, "float", "data[][]");
    dl_func_add_arg(&fun, "int", "iter");
    dl_func_add_arg(&fun, "int", "n_points");
    dl_func_add_arg(&fun, "int", "k");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))
  
  CHECK_BB(import_class_end(env))
  return 1;
}
