#include <cv.hpp>

#include "loup.hpp"
#include "fx.hpp"
#include "trounoir.hpp"

void TrouNoir::run(Loup* loup, Mat mat) {
  Joy *joy = loup->joy();
  Mat m = Mat::zeros(mat.size(), mat.type());
  Mat n = Mat::zeros(mat.size(), mat.type());
  Point center(640 * joy->x, 480 * joy->y);
  Scalar scalar(255, 255, 255, 0);
  circle(m, center, 400 * joy->scale, scalar, CV_FILLED, 8, 0);
  mat.copyTo(n, m);
  n.copyTo(mat);
}
