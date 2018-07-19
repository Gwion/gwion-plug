#include "iostream"
#include <cv.hpp>

#include "loup.hpp"
#include "fx.hpp"

void* loup_process(void* data) {
  Loup* loup = (Loup*)data;
  while(true) {
    if(!loup->run())
      return NULL;
    waitKey(100); // was 1000/25
  }
  return NULL;
}

Loup::Loup() : fx() {
  if(!cap.open(0)) {
    std::cout << "No webcam found!!!\n";
    exit(2);
  }
  pthread_create(&thread, NULL, loup_process, this);
}

bool Loup::run() {
  Mat frame;
  cap >> frame;
  if(frame.empty()) {
    std::cerr << "Problem with webcam\n";
    return 0;
  }
  for(auto &e : fx)
    if(e->ok)
      e->run(this, frame);
  imshow("this is you, smile! :)", frame);
  return 1;
}

int Loup::add(Fx* e) {
  int ret = fx.size();
  fx.push_back(e);
  return ret;
}

int Loup::set(int index, bool ok) {
  Fx* e = fx[index];  
  return e ? (fx[index]->ok = ok) : - 1;
}

Loup::~Loup() {
  pthread_cancel(thread);
  pthread_join(thread, NULL);
  for(auto &e : fx)
    delete e;
}
