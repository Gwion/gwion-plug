#include "../Gwion.hh"

// general includes
#include <stdio.h>
#include <limits.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
using namespace std;

// faust include
#include "faust/dsp/llvm-dsp.h"
#include "faust/gui/UI.h"
#include "faust/gui/PathBuilder.h"

#ifndef FAUSTFLOAT
#define FAUSTFLOAT m_float
#endif

//-----------------------------------------------------------------------------
// name: class FauckUI
// desc: Faust ChucK UI -> map of complete hierarchical path and zones
//-----------------------------------------------------------------------------
class FauckUI : public UI, public PathBuilder {
protected:
  // name to pointer map
  std::map<std::string, FAUSTFLOAT *> fZoneMap;

  // insert into map
  void insertMap(std::string label, FAUSTFLOAT *zone) {
    // map
    fZoneMap[label] = zone;
  }

public:
  // constructor
  FauckUI() {}
  // destructor
  virtual ~FauckUI() {}

  // -- widget's layouts
  void openTabBox(const char *label) { fControlsLevel.push_back(label); }
  void openHorizontalBox(const char *label) { fControlsLevel.push_back(label); }
  void openVerticalBox(const char *label) { fControlsLevel.push_back(label); }
  void closeBox() { fControlsLevel.pop_back(); }

  // -- active widgets
  void addButton(const char *label, FAUSTFLOAT *zone) {
    insertMap(buildPath(label), zone);
  }
  void addCheckButton(const char *label, FAUSTFLOAT *zone) {
    insertMap(buildPath(label), zone);
  }
  void addVerticalSlider(const char *label, FAUSTFLOAT *zone NUSED, FAUSTFLOAT init NUSED,
                         FAUSTFLOAT fmin NUSED, FAUSTFLOAT fmax NUSED, FAUSTFLOAT step NUSED) {
    insertMap(buildPath(label), zone);
  }
  void addHorizontalSlider(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init NUSED,
                           FAUSTFLOAT fmin NUSED, FAUSTFLOAT fmax NUSED, FAUSTFLOAT step NUSED) {
    insertMap(buildPath(label), zone);
  }
  void addNumEntry(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init NUSED,
                   FAUSTFLOAT fmin NUSED, FAUSTFLOAT fmax NUSED, FAUSTFLOAT step NUSED) {
    insertMap(buildPath(label), zone);
  }

  // -- passive widgets
  void addHorizontalBargraph(const char *label, FAUSTFLOAT *zone,
                             FAUSTFLOAT fmin NUSED, FAUSTFLOAT fmax NUSED) {
    insertMap(buildPath(label), zone);
  }
  void addVerticalBargraph(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT fmin NUSED,
                           FAUSTFLOAT fmax NUSED) {
    insertMap(buildPath(label), zone);
  }

  // -- soundfiles
  void addSoundfile(const char *label NUSED, const char *filename NUSED,
                    Soundfile **sf_zone NUSED) {}

  // -- metadata declarations
  void declare(FAUSTFLOAT *zone NUSED, const char *key NUSED, const char *val NUSED) {}

  // set/get
  void setValue(const std::string &path, FAUSTFLOAT value) {
    // append "/chuck/" if necessary
    string p =
        path.length() > 0 && path[0] == '/' ? path : string("/chuck/") + path;

    // TODO: should check if path valid?
    if (fZoneMap.find(p) == fZoneMap.end()) {
      // error
      cerr << "[Faust]: cannot set parameter named: " << path;
      if (path != p)
        cerr << " OR " << p << endl;
      else
        cerr << endl;
      return;
    }

    // set it!
    *fZoneMap[p] = value;
  }

  m_float getValue(const std::string &path) {
    // append "/0x00/" if necessary
    string p =
        path.length() > 0 && path[0] == '/' ? path : string("/0x00/") + path;

    // TODO: should check if path valid?
    if (fZoneMap.find(p) == fZoneMap.end()) {
      // error
      cerr << "[Faust]: cannot get parameter named: " << path;
      if (path != p)
        cerr << " OR " << p << endl;
      else
        cerr << endl;
      return 0;
    }

    return *fZoneMap[path];
  }

  void dumpParams() {
    // iterator
    std::map<std::string, FAUSTFLOAT *>::iterator iter = fZoneMap.begin();
    // go
    for (; iter != fZoneMap.end(); iter++) {
      // print
      cerr << iter->first << " : " << *(iter->second) << endl;
    }
  }

  // map access
  std::map<std::string, FAUSTFLOAT *> &getMap() { return fZoneMap; }
  // get map size
  int getParamsCount() { return fZoneMap.size(); }
  // get param path
  std::string getParamPath(int index) {
    std::map<std::string, FAUSTFLOAT *>::iterator it = fZoneMap.begin();
    while (index-- > 0 && it++ != fZoneMap.end()) {}
    return (*it).first;
  }
};

//-----------------------------------------------------------------------------
// name: class Faust
// desc: class definition of internal chugin data
//-----------------------------------------------------------------------------
// implementation for tick function
static TICK(faust_tick);

#define SAFE_DELETE(x) if((x))delete (x);
#define SAFE_DELETE_ARRAY(x) if((x))delete[] (x);

class Faust {
public:
  // constructor
  Faust(void) {
    // clear
    m_factory = NULL;
    m_dsp     = NULL;
    m_ui      = NULL;
    // zero
    m_input  = NULL;
    m_output = NULL;
    // default
    m_numInputChannels  = 0;
    m_numOutputChannels = 0;
    // auto import
    m_autoImport = string("// Faust Chugin auto import:\n \
        import(\"stdfaust.lib\");\n");
  }

  // destructor
  ~Faust() {
    deleteDSPFactory(m_factory);
    delete m_ui;
    delete[] m_input;
    delete[] m_output;
  }

  // allocate
  void allocate(const Gwion gwion, const M_Object gwobj, const int inputChannels, const int outputChannels) {
    // set
    m_numInputChannels  = inputChannels;
    m_numOutputChannels = outputChannels;

    // allocate channels
    m_input  = new FAUSTFLOAT *[m_numInputChannels];
    m_output = new FAUSTFLOAT *[m_numOutputChannels];

    const UGen u = UGEN(gwobj);
    ugen_ini(gwion, u, m_numInputChannels, m_numOutputChannels);
    ugen_gen(gwion, u, faust_tick, this, 0);

    const int nchans    = max(m_numInputChannels, m_numOutputChannels);
    if(nchans > 1 ) {
      for(int i = 0; i < m_numInputChannels; i++)
        m_input[i] = &UGEN(u->connect.multi->channel[i])->in;
      for(int i = 0; i < m_numInputChannels; i++)
        m_output[i] = &UGEN(u->connect.multi->channel[i])->out;
    } else {
      m_input[0] = &u->in;
      m_output[0] = &u->in;
    }
  }

  // eval
  static Faust *eval(const VM_Shred shred, const M_Object gwobj, const string &code) {
    // arguments
    const int    argc = 0;
    const char **argv = NULL;
    // optimization level
    const int optimize = -1;

    Faust *f = new Faust();
    f->m_code = string(code);
    string theCode = string(f->m_autoImport + "\n" + code);
    string errorString = string("");
    string name = string("gwion");
    string target = string("");

    f->m_factory = createDSPFactoryFromString(name, theCode, argc, argv, target,
                                           errorString, optimize);

    if (errorString != "") {
      cerr << "[Faust]: " << errorString << endl;
      delete f;
      return NULL;
    }
    f->m_dsp = f->m_factory ->createDSPInstance();
    f->m_ui = new FauckUI();
    f->m_dsp->buildUserInterface(f->m_ui);
    const int inputs = f->m_dsp->getNumInputs();
    const int outputs = f->m_dsp->getNumOutputs();
    f->allocate(shred->info->vm->gwion, gwobj, inputs, outputs );
    f->m_dsp->init( (int)(shred->info->vm->bbq->si->sr + .5) );
    *(Faust**)(gwobj->data + SZ_INT) = f;
    return f;
  }

  static Faust *compile(const VM_Shred shred, const M_Object gwobj, const string &path) {
    // open file
    ifstream fin(path.c_str());
    // check
    if (!fin.good()) {
      // error
      cerr << "[Faust]: ERROR opening file: '" << path << "'" << endl;
      return NULL;
    }

    // clear code string
    string m_code = "";
    // get it
    for (string line; std::getline(fin, line); m_code += line + '\n');

    // eval it
    return eval(shred, gwobj, m_code);
  }

  // dump (snapshot)
  void dump() {
    cerr << "---------------- DUMPING [Faust] PARAMETERS ---------------"
         << endl;
    m_ui->dumpParams();
    cerr << "Number of Inputs: " << m_numInputChannels << endl;
    cerr << "Number of Outputs: " << m_numOutputChannels << endl;
    cerr << "-----------------------------------------------------------"
         << endl;
  }

  void tick(void) {
    m_dsp->compute(1, m_input, m_output);
  }

  m_float setParam(const string &n, m_float p) {
    m_ui->setValue(n, p);
    return p;
  }

  m_float getParam(const string &n) { return m_ui->getValue(n); }

  string code() { return m_code; }

private:
  // faust DSP object
  dsp *m_dsp;
  // code text (pre any modifications)
  string m_code;
  // llvm factory
  llvm_dsp_factory *m_factory;
  // auto import
  string m_autoImport;

  // faust input buffer
  FAUSTFLOAT **m_input;
  FAUSTFLOAT **m_output;

  // UI
  FauckUI *m_ui;

  // input and output
  int m_numInputChannels;
  int m_numOutputChannels;
};

static TICK(faust_tick) {
  Faust *const f_obj = (Faust *)u->module.gen.data;
  f_obj->tick();
}

static DTOR(faust_dtor) {
  Faust *f_obj = *(Faust **)(o->data + SZ_INT);
  delete f_obj;
}

static SFUN(faust_eval) {
  const VM_Code vmcode = *(VM_Code*)REG(SZ_INT);
  const m_str _code = STRING(*(M_Object*)MEM(0));
  const string code = _code;
  const M_Object gwobj = new_object(shred->info->mp, vmcode->ret_type);
  UGEN(gwobj) = new_UGen(shred->info->mp);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(gwobj));
  Faust::eval(shred, gwobj, code);
  *(M_Object*)RETURN = gwobj;
}

static SFUN(faust_compile) {
  const VM_Code vmcode = *(VM_Code*)REG(SZ_INT);
  const m_str _path = STRING(*(M_Object*)MEM(0));
  const string path = _path;
  const M_Object gwobj = new_object(shred->info->mp, vmcode->ret_type);
  UGEN(gwobj) = new_UGen(shred->info->mp);
  vector_add(&shred->info->vm->ugen, (vtype)UGEN(gwobj));
  Faust::compile(shred, gwobj, path);
  *(M_Object*)RETURN = gwobj;
}

static MFUN(faust_v_set) {
  Faust *const f = *(Faust **)(o->data + SZ_INT);
  const m_str name = STRING(*(M_Object*)MEM(SZ_INT));
  const m_float v = *(m_float *)MEM(SZ_INT);
  f->setParam(name, v);
  *(m_float*)RETURN = v;
}

static MFUN(faust_v_get) {
  Faust *const f = *(Faust **)(o->data + SZ_INT);
  const m_str name = STRING(*(M_Object*)MEM(SZ_INT));
  *(m_float*)RETURN = f->getParam(name);
}

static MFUN(faust_dump) {
  Faust *const f = *(Faust **)(o->data + SZ_INT);
  f->dump();
}

static MFUN(faust_code) {
  Faust *const f = *(Faust **)(o->data + SZ_INT);
  string _code = f->code();
  char const *code = _code.c_str();
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, (m_str)code);
}

extern "C" {

GWION_IMPORT(Faust) {
  DECL_B(const Type, t_faust, = gwi_class_ini(gwi, "Faust", "UGen"));
  SET_FLAG(t_faust, abstract | ae_flag_final);
  gwi_class_xtor(gwi, NULL, faust_dtor);

  GWI_B(gwi_func_ini(gwi, "Faust", "eval"));
  GWI_B(gwi_func_arg(gwi, "string", "path"));
  GWI_B(gwi_func_end(gwi, faust_eval, ae_flag_static));

  GWI_B(gwi_func_ini(gwi, "Faust", "compile"));
  GWI_B(gwi_func_arg(gwi, "string", "path"));
  GWI_B(gwi_func_end(gwi, faust_compile, ae_flag_static));

  GWI_B(gwi_func_ini(gwi, "float", "v"));
  GWI_B(gwi_func_arg(gwi, "string", "key"));
  GWI_B(gwi_func_arg(gwi, "float", "value"));
  GWI_B(gwi_func_end(gwi, faust_v_set, ae_flag_none));

  GWI_B(gwi_func_ini(gwi, "float", "v"));
  GWI_B(gwi_func_arg(gwi, "string", "key"));
  GWI_B(gwi_func_end(gwi, faust_v_get, ae_flag_none));

  GWI_B(gwi_func_ini(gwi, "void", "dump"));
  GWI_B(gwi_func_end(gwi, faust_dump, ae_flag_none));

  GWI_B(gwi_func_ini(gwi, "string", "code"));
  GWI_B(gwi_func_end(gwi, faust_code, ae_flag_none));

  GWI_B(gwi_item_ini(gwi, "int", "@f_data"));
  GWI_B(gwi_item_end(gwi, ae_flag_none, num, 0));

  return gwi_class_end(gwi);
}

}
