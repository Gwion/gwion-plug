m_uint*      kmeans(m_uint x, m_uint y, m_float** d, m_uint k, m_float t,
	m_float **centroids, m_uint initial_centroids);
m_uint knn_classify(m_uint x, m_uint y, m_float** d, m_uint n_labels, m_uint* labels, m_float* instance, m_uint k);
m_uint* knn_classify_multi(m_uint x, m_uint y, m_float** d, m_uint n_labels, m_uint* labels, m_uint n_instance, m_float** instance, m_uint k);
m_float** matrix_alloc(m_uint x, m_uint y);
void matrix_release(m_float** d);
m_float** kmeans_refine(m_uint x, m_uint y, m_float** d, m_uint iter, m_uint n_points, m_uint n_label);
