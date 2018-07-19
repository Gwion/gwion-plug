#include <cv.hpp>

#include "loup.hpp"
#include "fx.hpp"
#include "sobel.hpp"

void sobel::run(Loup* loup, Mat frame) {
    Mat grey;
    cvtColor(frame, grey, CV_BGR2GRAY); 
    Mat sobelx;
    Sobel(grey, sobelx, CV_32F, 1, 0);
 
    double minVal, maxVal;
    minMaxLoc(sobelx, &minVal, &maxVal); //find minimum and maximum intensities
    //cout << "minVal : " << minVal << endl << "maxVal : " << maxVal << endl;
 
    Mat draw;
    sobelx.convertTo(draw, CV_8U, 255.0/(maxVal - minVal), -minVal * 255.0/(maxVal - minVal));
 
    namedWindow("image", CV_WINDOW_AUTOSIZE);
    imshow("image", draw);

}
