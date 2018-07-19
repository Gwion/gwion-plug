class Vaisseau3 : public Fx {
  Ptr<BackgroundSubtractor> sub;
  Mat bg;
public:
  Vaisseau3();
  virtual void run(Loup* loup, Mat mat);
};
