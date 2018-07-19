#include <cv.hpp>
#include <opencv2/video/background_segm.hpp>

#include "loup.hpp"
#include "fx.hpp"
#include "vaisseau2.hpp"

Vaisseau2::Vaisseau2() {
  sub = createBackgroundSubtractorKNN();
  //sub = createBackgroundSubtractorMOG2();
}

void Vaisseau2::run(Loup* loup, Mat frame) {
  Mat gray;
  Mat m = Mat::zeros(frame.size(), frame.type());
  sub->apply(frame, bg); 
  frame.copyTo(m, bg);
  //imshow("bg", bg);
  //imshow("bg2", m);
  //
 
  // turn to gray, blur, apply threshold
  //cvtColor(bg, gray, CV_BGR2GRAY);
  //blur(gray, gray, Size(3,3));
  blur(bg, gray, Size(3,3));
  threshold(gray, gray, 200, 255, CV_THRESH_BINARY); 

  // Create a structuring element
  int erosion_size = 8;  
  Mat element = getStructuringElement(cv::MORPH_CROSS,
      Size(2 * erosion_size + 1, 2 * erosion_size + 1),
      Point(erosion_size, erosion_size) );

 // Apply erosion or dilation on the image
 erode(gray, gray,element);  // dilate(image,dst,element);
  
  //Find the contours. Use the contourOutput Mat so the original image doesn't get overwritten
    std::vector<std::vector<cv::Point> > contours;
    //Mat contourOutput(frame.size(), CV_8UC1, cv::Scalar(0,0,0));
    Mat contourOutput = Mat(frame.rows, frame.cols, CV_8UC1, gray.data);
    //frame.copyTo(contourOutput);
    //Mat contourOutput = frame.clone();
    //Mat contourOutput;
    //frame.convertTo(contourOutput, CV_8UC1);
    //contourOutput.convertTo(contourOutput, CV_8UC1);
    //findContours(contourOutput, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
    findContours(gray, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE );
    //findContours(contourOutput, contours, CV_RETR_FLOODFILL, CV_CHAIN_APPROX_NONE );

  //Draw the contours
    Mat contourImage(frame.size(), CV_8UC3, cv::Scalar(0,0,0));
  //Mat contourImage(frame.size(), CV_8UC3, cv::Scalar(125,123,165));
    Scalar colors[3];
    colors[0] = Scalar(255, 0, 0);
    colors[1] = Scalar(0, 255, 0);
    colors[2] = Scalar(0, 0, 255);


  Mat trans = frame.clone();
  for (size_t idx = 0; idx < contours.size(); idx++) {
    drawContours(contourImage, contours, idx, colors[idx % 3]);
   

    //Point** _ppt = (Point**)malloc(2* contours[idx].size() * sizeof(Point));
    //for (size_t i = 0; i < contours[idx].size(); i++)
    //_ppt[0][i] = contours[idx][i];
    //const Point** ppt = (const Point**)_ppt;
    //int npt[] = { (int)contours[idx].size() };
    //fillPoly(trans, ppt, npt, 1, colors[0]);
  }

for (std::vector<Point> contour : contours) {
  //if (iscorrect(contour)) {
        polylines(frame, contours[0], true, colors[0], 1, 8);
        //}
}

  imshow("bg2", m);
  imshow("grayscale", gray);
  imshow("contour", contourImage);
  //m.copyTo(frame);
}
