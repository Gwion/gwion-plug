class Fx {
public:
  bool ok;
  Fx() : ok(1) {}
  virtual void run(Loup*, Mat) = 0;
};

