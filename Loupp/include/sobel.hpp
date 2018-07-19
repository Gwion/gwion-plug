class sobel : public Fx {
  Ptr<BackgroundSubtractor> sub;
  Mat bg;
public:
  virtual void run(Loup* loup, Mat mat);
};
