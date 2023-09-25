#ifndef __GWEVDEV
#define __GWEVDEV
typedef struct {
  struct libevdev* evdev;
  pthread_t thread;
  pthread_mutex_t mutex;
  MP_Vector *args;
  MemPool mp;
  uint32_t idx;
  int index;
  int fd;
} EvdevInfo;

ANN m_bool import_absinfo(const Gwi gwi);
#define ABSINFO(o) *(struct input_absinfo**)(o->data)

ANN m_bool import_uinput(const Gwi gwi);
#define UINPUT(o) *(struct libevdev_uinput**)(o->data)

ANN m_bool import_evdevev(const Gwi gwi);
typedef struct gw_input_event {
  m_int type;
  m_int code;
  m_int value;
  m_int sec;
  m_int usec;
} gw_input_event;
#define EVDEVEV(o) (gw_input_event*)((o)->data)
#endif
