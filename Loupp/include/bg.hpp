class Bg : public Fx {
  Ptr<BackgroundSubtractor> sub;
  Mat bg;
public:
  Bg();
  virtual void run(Loup* loup, Mat mat);
};
