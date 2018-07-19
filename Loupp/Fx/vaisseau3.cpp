#include <cv.hpp>

#include "loup.hpp"
#include "fx.hpp"
#include "vaisseau3.hpp"

static int speed_factor = 5; // 10 was good
Vaisseau3::Vaisseau3() {
  sub = createBackgroundSubtractorKNN();
  //sub = createBackgroundSubtractorMOG2();
 }

void Vaisseau3::run(Loup* loup, Mat mat) {
  Joy* joy = loup->joy();
  Mat base = mat.clone();
  Mat bgm = Mat::zeros(mat.size(), mat.type());
  sub->apply(mat, bg); 
  mat.copyTo(bgm, bg);
  threshold(bgm, bgm, 250,255,0);
  bgm.copyTo(base);
  
  int width = 640;
  int height = 480;
  int x = 0;
  int y = 0;

  Mat m = Mat::zeros(mat.size(), mat.type());
  std::vector<Point2f> p(4);
  std::vector<Point2f> q(4);
  unsigned int i;
  unsigned int max = (joy->scale+.1) * speed_factor;
  for(i = 0; i < max; i++) {

    p[0].x = x;     p[0].y = y;
    p[1].x = width; p[1].y = y;
    p[2].x = x;     p[2].y = height;
    p[3].x = width;     p[3].y = height;


  int x_offset = (joy->x-.5) * (640 *(i/10.));
  int y_offset = (joy->y-.5) * (480 *(i/10.));
  x += x_offset; y += y_offset;
  width += x_offset; height += y_offset;
    q[0].x = x;     q[0].y = y;
    q[1].x = width; q[1].y = y;
    q[2].x = x;     q[2].y = height;
    q[3].x = width;     q[3].y = height;
    Mat H = findHomography(p, q); //get homography
    Mat warped_image;
    warpPerspective(base, warped_image, H, mat.size());
    addWeighted(warped_image, .07, m, 1, 1, m); 
    
}
m.copyTo(mat);
//applyColorMap(m, m, COLORMAP_SPRING);
}
