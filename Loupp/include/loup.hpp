using namespace cv;
class Fx;

class Joy {
public:
  Joy() :scale(1), x(.5), y(.5) {}
  float scale;
  float x, y;
  int   hx, hy; // hat
  int   button[3];
  int   trigger;
};

class Nano {
  float slider[8];
  float button[8];
  bool mute[8];
  bool solo[8];
  bool rec[8];
  bool transport[5];
  bool utils[5];
};

class Loup {
  cv::VideoCapture cap;
  std::vector<Fx*> fx;
  pthread_t thread;
  Joy  m_joy;
  Nano m_nano;
  public:
  Loup(void);
  bool run(void);
  inline Joy *joy()   { return &m_joy; }
  inline Nano *nano() { return &m_nano; }
  int add(Fx*);
  int set(int , bool);
  ~Loup(void);
};
