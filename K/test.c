#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defs.h"
#include "k.h"

static m_float** matrix_file(const char* s)
{
  m_uint i;
  /*char c[16];*/
  FILE* f = fopen(s, "r");
  m_float** ret = matrix_alloc(150, 4);
  for(i = 0; i < 150; i++) {
    m_float a, b, c, d;
    char e[16];
    fscanf(f, "%f,%f,%f,%f,Iris-%15s",  &a, &b, &c, &d, &e);
    ret[i][0] = a;
    ret[i][1] = b;
    ret[i][2] = c;
    ret[i][3] = d;
  }
  fclose(f);
  return ret;
}

int main() {
  m_uint labels[] = {0, 1, 0,1,0,1,0,1};
  m_float instance[]={1, 1, 3};
  m_uint k=1;
  m_uint n_labels=2;
  m_uint i;
  m_float** d = matrix_alloc(8, 3);

  d[0][0] = 1.2; d[0][1] = 1.1; d[0][2] =1.3;
  d[1][0] = 3.2; d[1][1] = 3.1; d[1][2] =3.3;
  d[2][0] = 3.2; d[2][1] = 3.1; d[2][2] =3.4;
  d[3][0] = 3.2; d[3][1] = 3.1; d[3][2] =3.3;
  d[4][0] = 3.2; d[4][1] = 3.1; d[4][2] =3.4;
  d[5][0] = 1.2; d[5][1] = 1.1; d[5][2] =1.3;
  d[6][0] = 3.2; d[6][1] = 3.1; d[6][2] =3.3;
  d[7][0] = 3.2; d[7][1] = 3.1; d[7][2] =3.4;

  m_uint ret = knn_classify(8, 3, d, n_labels, labels, instance, k);

  m_float** instances = matrix_alloc(1,3);
  memcpy(instances[0], instance, 3 * sizeof(m_float));
  m_uint* l = knn_classify_multi(8, 3, d, n_labels, labels, 1, instances, k);
  free(l);
  matrix_release(instances);
  
  m_float** centroids = matrix_alloc(2, 3);
  l = kmeans(8, 3, d, 2, 1e-4, centroids, 0);
  free(l);
  ret = knn_classify(2, 3, centroids, n_labels, labels, instance, k);
  matrix_release(centroids);

  centroids = kmeans_refine(8, 3, d, 3, 5, 2);
  l = kmeans(8, 3, d, 2, 1e-4, centroids, 1);
  free(l);
  matrix_release(d);
  ret = knn_classify(2, 3, centroids, n_labels, labels, instance, k);

  instances = matrix_alloc(1,3);
  memcpy(instances[0], instance, 3 * sizeof(m_float));
  l = knn_classify_multi(2, 3, centroids, n_labels, labels, 1, instances, k);
  free(l);
  matrix_release(centroids);
  matrix_release(instances);

  m_float** iris = matrix_file("iris.data");
  m_uint iris_labels[150];
  for(i = 0; i < 50; i++) {
     iris_labels[i] = 0;
     iris_labels[i+50] = 1;
     iris_labels[i+100] = 2;
  }
  ret = knn_classify(150, 4, iris, 3, iris_labels, iris[100], 15);
  matrix_release(iris);
  return ret;
}
