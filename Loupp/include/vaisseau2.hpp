class Vaisseau2 : public Fx {
  Ptr<BackgroundSubtractor> sub;
  Mat bg;
public:
  Vaisseau2();
  virtual void run(Loup* loup, Mat mat);
};
