#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"
#include "k.h"
// TODO: thread

struct M_Vector_ {
  char*  ptr;   // data
  m_uint len;   // number of elements * size
  m_uint size;  // size of objects
  m_uint depth;
};

static m_float** gw2c(M_Vector vec, m_uint* x, m_uint* y) {
	m_uint i, j;
	*x = m_vector_size(vec);
    M_Object a;
    m_vector_get(vec, 0, (char*)&a);
	*y = m_vector_size(ARRAY(a));
	m_float** ret = matrix_alloc(*x, *y);
    for(i = 0; i < *x; i++) {
      m_vector_get(vec, i, (char*)&a);
      for(j = 0; j < *y; j++)
        m_vector_get(ARRAY(a), j, (char*)&ret[i][j]);
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

  Type t = array_type(t_int, 1);
  M_Object ret_obj = new_array(t, SZ_INT, inst_x, 1);
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
  Type t = array_type(t_int, 1);
  M_Object ret_obj = new_array(t, SZ_INT, data_x, 1);
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
  Type t = array_type(t_int, 1);
  M_Object ret_obj = new_array(t, SZ_INT, data_x, 2);
  *(m_uint*)RETURN = (m_uint)ret_obj;
  vector_add(&shred->gc, (vtype)ret_obj);
  for(i = 0; i < n_label; i++) {
  Type t = array_type(t_float, 1);
    M_Object obj = new_array(t, SZ_FLOAT, data_y, 1);
    memcpy(ARRAY(obj)->ptr, ret[i], data_y * sizeof(m_float));
    m_vector_set(ARRAY(ret_obj), i, (char*)&obj);
    vector_add(&shred->gc, (vtype)obj);
  }
  release(data_obj, shred);
  matrix_release(ret);
  matrix_release(data);
}

GWION_IMPORT(gwk) {
  const Type t_k = gwi_mk_type(gwi, "K", 0, NULL );
  CHECK_BB(gwi_class_ini(gwi, t_k, NULL, NULL))

  gwi_func_ini(gwi, "int[]", "nn", gw_knn);
    gwi_func_arg(gwi, "float", "data[][]");
    gwi_func_arg(gwi, "int", "labels[]");
    gwi_func_arg(gwi, "float", "instances[][]");
    gwi_func_arg(gwi, "int", "k");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "int[]", "means", gw_kmeans);
    gwi_func_arg(gwi, "float", "data[][]");
    gwi_func_arg(gwi, "float", "centroid[][]");
    gwi_func_arg(gwi, "int", "k");
    gwi_func_arg(gwi, "int", "initial_centroid");
    gwi_func_arg(gwi, "float", "theta");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))
  
  gwi_func_ini(gwi, "int[]", "fine_means", gw_kmeans_refine);
    gwi_func_arg(gwi, "float", "data[][]");
    gwi_func_arg(gwi, "int", "iter");
    gwi_func_arg(gwi, "int", "n_points");
    gwi_func_arg(gwi, "int", "k");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))
  
  CHECK_BB(gwi_class_end(gwi))
  return 1;
}
