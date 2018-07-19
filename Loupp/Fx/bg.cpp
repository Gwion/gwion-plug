#include <cv.hpp>
#include <opencv2/video/background_segm.hpp>

#include "loup.hpp"
#include "fx.hpp"
#include "bg.hpp"

Bg::Bg() {
  sub = createBackgroundSubtractorKNN();
  //sub = createBackgroundSubtractorMOG2();
}

void Bg::run(Loup* loup, Mat frame) {
  Mat m = Mat::zeros(frame.size(), frame.type());
  sub->apply(frame, bg); 
  frame.copyTo(m, bg);
  //imshow("bg", bg);
  //imshow("bg2", m);
  m.copyTo(frame);
}
