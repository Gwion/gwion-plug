#include "Gwion.hh"

#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <map>
#include <string>

//-------------------------------------------------------------------
// Generic min and max using C++ inline
//-------------------------------------------------------------------

#ifndef WIN32

inline int max(unsigned int a, unsigned int b) { return (a > b) ? a : b; }
inline int max(int a, int b) { return (a > b) ? a : b; }

inline long max(long a, long b) { return (a > b) ? a : b; }
inline long max(int a, long b) { return (a > b) ? a : b; }
inline long max(long a, int b) { return (a > b) ? a : b; }

inline float max(float a, float b) { return (a > b) ? a : b; }
inline float max(int a, float b) { return (a > b) ? a : b; }
inline float max(float a, int b) { return (a > b) ? a : b; }
inline float max(long a, float b) { return (a > b) ? a : b; }
inline float max(float a, long b) { return (a > b) ? a : b; }

inline double max(double a, double b) { return (a > b) ? a : b; }
inline double max(int a, double b) { return (a > b) ? a : b; }
inline double max(double a, int b) { return (a > b) ? a : b; }
inline double max(long a, double b) { return (a > b) ? a : b; }
inline double max(double a, long b) { return (a > b) ? a : b; }
inline double max(float a, double b) { return (a > b) ? a : b; }
inline double max(double a, float b) { return (a > b) ? a : b; }

inline int min(int a, int b) { return (a < b) ? a : b; }

inline long min(long a, long b) { return (a < b) ? a : b; }
inline long min(int a, long b) { return (a < b) ? a : b; }
inline long min(long a, int b) { return (a < b) ? a : b; }

inline float min(float a, float b) { return (a < b) ? a : b; }
inline float min(int a, float b) { return (a < b) ? a : b; }
inline float min(float a, int b) { return (a < b) ? a : b; }
inline float min(long a, float b) { return (a < b) ? a : b; }
inline float min(float a, long b) { return (a < b) ? a : b; }

inline double min(double a, double b) { return (a < b) ? a : b; }
inline double min(int a, double b) { return (a < b) ? a : b; }
inline double min(double a, int b) { return (a < b) ? a : b; }
inline double min(long a, double b) { return (a < b) ? a : b; }
inline double min(double a, long b) { return (a < b) ? a : b; }
inline double min(float a, double b) { return (a < b) ? a : b; }
inline double min(double a, float b) { return (a < b) ? a : b; }

#endif // ndef WIN32

inline int lsr(int x, int n) { return int(((unsigned int)x) >> n); }
inline int int2pow2(int x) {
  int r = 0;
  while ((1 << r) < x) r++;
  return r;
}

/******************************************************************************
 *******************************************************************************

 FAUST META DATA

 *******************************************************************************
 *******************************************************************************/

struct Meta : std::map<std::string, std::string> {
  void declare(const char *key, const char *value) { (*this)[key] = value; }
};

/* UI class - do-nothing (from FAUST/minimal.cpp) */

#ifdef WIN32
#ifdef interface
#undef interface
#endif // interface
#endif // WIN32

class UI {
public:
  virtual ~UI() {}

  // active widgets
  virtual void addButton(const char *label, float *zone)             = 0;
  virtual void addToggleButton(const char *label, float *zone)       = 0;
  virtual void addCheckButton(const char *label, float *zone)        = 0;
  virtual void addVerticalSlider(const char *label, float *zone, float init,
                                 float min, float max, float step)   = 0;
  virtual void addHorizontalSlider(const char *label, float *zone, float init,
                                   float min, float max, float step) = 0;
  virtual void addNumEntry(const char *label, float *zone, float init,
                           float min, float max, float step)         = 0;

  // passive widgets
  virtual void addNumDisplay(const char *label, float *zone, int precision) = 0;
  virtual void addTextDisplay(const char *label, float *zone, char *names[],
                              float min, float max)                         = 0;
  virtual void addHorizontalBargraph(const char *label, float *zone, float min,
                                     float max)                             = 0;
  virtual void addVerticalBargraph(const char *label, float *zone, float min,
                                   float max)                               = 0;

  // layout widgets
  virtual void openFrameBox(const char *label)      = 0;
  virtual void openTabBox(const char *label)        = 0;
  virtual void openHorizontalBox(const char *label) = 0;
  virtual void openVerticalBox(const char *label)   = 0;
  virtual void closeBox()                           = 0;

  virtual void declare(float *zone, const char *key, const char *value) {}
};

class dsp {
public:
  virtual ~dsp();
  virtual int  getNumInputs()                                    = 0;
  virtual int  getNumOutputs()                                   = 0;
  virtual void buildUserInterface(UI *interface)                 = 0;
  virtual void init(int samplingRate)                            = 0;
  virtual void compute(int len, float **inputs, float **outputs) = 0;

  m_float **ck_frame_in;
  m_float **ck_frame_out;

protected:
  int fSamplingFreq;
};

dsp::~dsp() {}

        /*
         * FAUST intrinsic
         */
        << includeIntrinsic
    >>

/*
 * FAUST defines UI values as private, but provides no getters/setters.
 * In our particular case it's way more convenient to access them directly
 * than to set up a complicated UI structure.  Also get rid of everything
 * being "virtual", since it may stop the compiler from inlining properly!
 */
#define private public
#define virtual

/* Rename the class the name of our DSP. */
#define mydsp % dsp_name %

    /*
     * FAUST class
     */
    << includeclass >>

#undef private
#undef virtual
#undef mydsp

    /*
     * Gwion glue code
     */
    // mono tick
    static TICK(% dsp_name % _tick) {
  % dsp_name % *d = (% dsp_name % *)u->module.gen.data;
  d->compute(1, d->ck_frame_in, d->ck_frame_out);
}

static CTOR(% dsp_name % _ctor) {
  % dsp_name % *d                      = new % dsp_name % ;
  *(% dsp_name % **)(o->data + SZ_INT) = d;
  d->init(shred->info->vm->bbq->si->sr);
  const int nin    = d->getNumInputs();
  const int nout   = d->getNumInputs();
  d->ck_frame_in   = new m_float *[nin];
  d->ck_frame_out  = new m_float *[nout];
  const int nchans = max(nin, nout);
  ugen_ini(shred->info->vm->gwion, UGEN(o), nin, nout);
  ugen_gen(shred->info->vm->gwion, UGEN(o), % dsp_name % _tick, d, 0);

  const UGen u = UGEN(o);
  if (nchans > 1) {
    for (int i = 0; i < nin; i++)
      d->ck_frame_in[i] = &UGEN(u->connect.multi->channel[i])->in;
    for (int i = 0; i < nout; i++)
      d->ck_frame_out[i] = &UGEN(u->connect.multi->channel[i])->out;
  } else {
    d->ck_frame_in[0]  = &u->in;
    d->ck_frame_out[0] = &u->out;
  }
}

static DTOR(% dsp_name % _dtor) {
  % dsp_name % *d = *(% dsp_name % **)(o->data + SZ_INT);

  delete[] d->ck_frame_in;
  delete[] d->ck_frame_out;

  delete d;
}

% ctrl_cget_functions %

    extern "C" {

  GWION_IMPORT(% dsp_name % _query) {
    gwi_class_ini(gwi, "%dsp_name%", "UGen");
    gwi_class_xtor(gwi, % dsp_name % _ctor, % dsp_name % _dtor);

    // add member variable
    gwi_item_ini(gwi, "int", "@%dsp_name%_data");
    gwi_item_end(gwi, ae_flag_none, num, 0);
    // add functions
    % ctrl_cget_query %
        // end import
        gwi_class_end(gwi);
    return GW_OK;
  }
}
