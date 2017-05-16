#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "time.h"

#include "../../include/defs.h"
m_float** matrix_alloc(m_uint x, m_uint y) {
  m_uint i;
  m_float** a = malloc(x* sizeof(m_float*));
  a[0] = malloc(x * y * sizeof(m_float));
  for(i = 0; i < x; i++)
    a[i] = *a + y * i;
  return a;
}

void matrix_release(m_float** d) {
  free(d[0]);
  free(d);
}

m_uint* kmeans(m_uint x, m_uint y, m_float **data, m_uint k, m_float t,
    m_float **c, m_uint initial_centroids)
{
  m_uint *labels = (m_uint*)calloc(x, sizeof(m_uint));
  m_uint h, i, j;
  m_uint counts[k];
  m_float old_error, error = DBL_MAX; /* sum of squared euclidean distance */
  m_float c1[k][y];

  /* pick k points as initial centroids */
  for (h = i = 0; !initial_centroids, i < k; h += x / k, i++)
    memcpy(c[i], data[h], y * sizeof(m_float));

  do {
    /* save error from last step */
    old_error = error, error = 0;
    /* clear old counts and temp centroids */
    for (i = 0; i < k; counts[i++] = 0)
      memset(c1[i], 0, y * sizeof(m_float));
    for (h = 0; h < x; h++) {
      /* identify the closest cluster */
      m_float min_distance = DBL_MAX;
      for (i = 0; i <  k; i++) {
        m_float distance = 0;
        for (j = y; j-- > 0; distance += pow(data[h][j] - c[i][j], 2));
        if (distance < min_distance) {
          labels[h] = i;
          min_distance = distance;
        }
      }
      /* update size and temp centroid of the destination cluster */
      for (j = y; j-- > 0; c1[labels[h]][j] += data[h][j]);
      counts[labels[h]]++;
      /* update standard error */
      error += min_distance;
    }
    for (i = 0; i < k; i++) /* update all centroids */
      for (j = 0; j < y; j++)
        c[i][j] = counts[i] ? c1[i][j] / counts[i] : c1[i][j];
  } while (fabs(error - old_error) > t);

  return labels;
}

m_float** kmeans_refine(m_uint x, m_uint y, m_float** d, m_uint iter, m_uint n_points, m_uint n_label)
{
  m_uint i, j, k, l, index;
  m_float min = INFINITY;
  m_float dist[iter];
  m_float** centroid[iter];
  m_float** ret;
  srand(time(NULL));
  /* try with a subsample iter times */
  for(i = 0; i < iter; i++)
  {
    m_uint* labels;
    m_float sub[n_points][y];
    centroid[i] = matrix_alloc(n_label, y);
    /* make new subsample */
    /*m_float** sub =	matrix_alloc(n_points, y);*/
    for(j = 0; j < n_points; j++)
      memcpy(sub[j], d[(int) round(rand()/(m_float)RAND_MAX * (y-1))], y * sizeof(m_float));
    /* run kmeans for subsample */
    labels = kmeans(n_points, y, d, n_label, 1e-4, centroid[i], 0);
    /* compute distance */
    dist[i] = 0;
    /* for each point in the subsample */
    for(k = 0; k < n_points; k++)
      /* 	and in each dimension, */
      for(l = 0; l < y; l++)
        /* sum squares of distances */
        dist[i] += (sub[k][l] - centroid[i][labels[k]][l])  *
          (sub[k][l] - centroid[i][labels[k]][l]);
    free(labels);
  }
  /* get the best centroids */
  /* aka the one with min distance */
  for(i = iter; --i;)
  {
    j = i -1;
    if(dist[j] < min)
    {
      min = dist[j];
      index = j;
    }
  }
  for(i = 0; i < iter; i++)
    if(i != index)
      matrix_release(centroid[i]);
  return centroid[index];
}

typedef struct
{
  m_uint id;
  m_float dist;
} id_dist;

m_float id_dist_min(id_dist* d, m_uint size, m_uint* index)
{
  m_uint i;
  m_float min = INFINITY;
  for(i = 0; i < size; i++)
  {
    if(d[i].dist < min)
    {
      min = d[i].dist;
      *index = i;
    }
  }
  return min;
}

void id_dist_sort(id_dist* d, id_dist* ret, m_uint size)
{
  m_uint i, j, k, index;
  id_dist tmp[size];
  id_dist Tmp[size];

  memcpy(tmp, d, size * sizeof(id_dist));
  for(i = 0; i < size; i++)
  {
    id_dist_min(tmp, size - i, &index);
    k = 0;
    for(j = 0; j < size - i; j++)
    {
      if(j != index)
      {
        Tmp[k] = tmp[j];
        k++;
      }
      else
        ret[i] = tmp[j];
    }
    for(j = 0; j < size - i - 1; j++)
      tmp[j] = Tmp[j];
  }
}

m_float euclidian_distance(m_float* a, m_float* b, m_uint size)
{
  m_uint i;
  m_float ret = 0.0;
  for(i = 0; i < size; i++)
  {
    m_float dist = (a[i] - b[i]);
    ret += (dist * dist);
  }
  return ret;
}

m_uint knn_classify(m_uint x, m_uint y, m_float** d, m_uint n_labels, m_uint* labels, m_float* instance, m_uint k)
{
  m_uint i, j = 0, ret;
  m_float  max = 0.0;
  id_dist 	id[x];
  id_dist		ret_id[x];
  m_uint _labels[n_labels];
 
  for(i = 0; i < x; i++)
  {
    id[i].id = i;
    id[i].dist = euclidian_distance(d[i], instance, y);
  }
  id_dist_sort(id, ret_id, x);
  memset(_labels, 0, n_labels * sizeof(m_uint));
  for(i = 0; i < k; i++)
    _labels[ labels[ ret_id[i].id ] ]++;
  for(i = 0; i < n_labels; i++)
  {
    if(_labels[i] > j)
    {
      j = _labels[i];
      ret = i;
    }
  }
  return ret;
}

m_uint* knn_classify_multi(m_uint x, m_uint y, m_float** d, m_uint n_labels, m_uint* labels, m_uint n_instance, m_float** instance, m_uint k)
{
  m_uint l;
  m_uint* ret = malloc(n_instance * sizeof(m_uint));
  for(l = 0; l < n_instance; l++) {
    m_uint i, j = 0;
    m_float  max = 0.0;
    id_dist 	id[x];
    id_dist		ret_id[x];
    m_uint _labels[n_labels];
    for(i = 0; i < x; i++)
    {
      id[i].id = i;
      id[i].dist = euclidian_distance(d[i], instance[l], y);
    }
    id_dist_sort(id, ret_id, x);
    memset(_labels, 0, n_labels * sizeof(m_uint));
    for(i = 0; i < k; i++)
      _labels[ labels[ ret_id[i].id ] ]++;
    for(i = 0; i < n_labels; i++) {
      if(_labels[i] > j) {
        j = _labels[i];
        ret[l] = i;
      }
    }
  }
  return ret;
}
