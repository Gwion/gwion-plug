#include <stdlib.h>
#include <string.h>
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
#include "array.h"
#include "k.h"
// TODO: thread

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

static SFUN(gw_knn) {
  M_Object data_obj = *(M_Object*)MEM(0);
  M_Object labl_obj = *(M_Object*)MEM(SZ_INT);
  M_Object inst_obj = *(M_Object*)MEM(SZ_INT*2);
  m_uint k          = *(m_uint*)  MEM(SZ_INT*3);

  m_uint data_x, data_y;
  m_uint inst_x, inst_y;
 
  m_float** data = gw2c(ARRAY(data_obj), &data_x, &data_y);
  m_float** inst = gw2c(ARRAY(inst_obj), &inst_x, &inst_y);
  m_uint n_labl = m_vector_size(ARRAY(labl_obj));
  m_uint* labl = (m_uint*)ARRAY(labl_obj)->ptr;
  m_uint* ret = knn_classify_multi(data_x, data_y, data, n_labl, labl, inst_x, inst, k);

  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_int], 1);
  M_Object ret_obj = new_array(shred->info->mp, t, inst_x);
  *(m_uint*)RETURN = (m_uint)ret_obj;
  matrix_release(data);
  matrix_release(inst);
  free(ret);
}

static SFUN(gw_kmeans) {
  M_Object data_obj = *(M_Object*)MEM(0);
  M_Object cent_obj = *(M_Object*)MEM(SZ_INT);
  m_uint   k        = *(m_uint*)  MEM(SZ_INT*2);
  m_uint   initial  = *(m_uint*)  MEM(SZ_INT*3);
  m_float  theta    = *(m_float*) MEM(SZ_INT*4);

  m_uint data_x, data_y;
  m_uint cent_x, cent_y;
  m_float** data = gw2c(ARRAY(data_obj), &data_x, &data_y);
  m_float** cent = gw2c(ARRAY(cent_obj), &cent_x, &cent_y);
  m_uint* ret = kmeans(data_x, data_y, data, k, theta, cent, initial);
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_int], 1);
  M_Object ret_obj = new_array(shred->info->mp, t, data_x);
  *(m_uint*)RETURN = (m_uint)ret_obj;
  memcpy(ARRAY(ret_obj)->ptr, ret, data_y * sizeof(m_uint));
  matrix_release(data);
  matrix_release(cent);
  free(ret);
}

static SFUN(gw_kmeans_refine) {
  m_uint i;
  M_Object data_obj = *(M_Object*)MEM(0);
  m_uint   iter     = *(m_uint*)  MEM(SZ_INT);
  m_uint   n_points = *(m_uint*)  MEM(SZ_INT*2);
  m_uint   n_label  = *(m_uint*)  MEM(SZ_INT*3);

  m_uint data_x, data_y;
  m_float** data = gw2c(ARRAY(data_obj), &data_x, &data_y);
  m_float** ret = kmeans_refine(data_x, data_y, data, iter, n_points, n_label);
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_int], 1);
  M_Object ret_obj = new_array(shred->info->mp, t, data_x);
  *(m_uint*)RETURN = (m_uint)ret_obj;
  for(i = 0; i < n_label; i++) {
  Type t = array_type(shred->info->vm->gwion->env, shred->info->vm->gwion->type[et_float], 1);
    M_Object obj = new_array(shred->info->mp, t, data_y);
    memcpy(ARRAY(obj)->ptr, ret[i], data_y * sizeof(m_float));
    m_vector_set(ARRAY(ret_obj), i, (char*)&obj);
  }
  matrix_release(ret);
  matrix_release(data);
}

GWION_IMPORT(K) {
  GWI_OB(gwi_class_ini(gwi, "K", NULL))

  gwi_func_ini(gwi, "int[]", "nn");
    gwi_func_arg(gwi, "float", "data[][]");
    gwi_func_arg(gwi, "int", "labels[]");
    gwi_func_arg(gwi, "float", "instances[][]");
    gwi_func_arg(gwi, "int", "k");
  GWI_BB(gwi_func_end(gwi, gw_knn, ae_flag_static))

  gwi_func_ini(gwi, "int[]", "means");
    gwi_func_arg(gwi, "float", "data[][]");
    gwi_func_arg(gwi, "float", "centroid[][]");
    gwi_func_arg(gwi, "int", "k");
    gwi_func_arg(gwi, "int", "initial_centroid");
    gwi_func_arg(gwi, "float", "theta");
  GWI_BB(gwi_func_end(gwi, gw_kmeans, ae_flag_static))
  
  gwi_func_ini(gwi, "int[]", "fine_means");
    gwi_func_arg(gwi, "float", "data[][]");
    gwi_func_arg(gwi, "int", "iter");
    gwi_func_arg(gwi, "int", "n_points");
    gwi_func_arg(gwi, "int", "k");
  GWI_BB(gwi_func_end(gwi, gw_kmeans_refine, ae_flag_static))
  
  GWI_BB(gwi_class_end(gwi))
  return GW_OK;
}
