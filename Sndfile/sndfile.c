#ifdef USE_DOUBLE
#define sf_write sf_write_double
#else
#define sf_write sf_write_float
#endif

#include <sndfile.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"

// TODO: use arguments
#define BUFSIZE 256
#define FILENAME "gwion"

DRVINI(sndfile_ini) {
  char tmp[128];
  SF_INFO info = { .samplerate= di->si->sr, .channels=di->si->out,
    .frames=BUFSIZE*2, .format=(SF_FORMAT_WAV | SF_FORMAT_PCM_24) };
  char *arg = strchr(di->si->arg, '=');
  if(arg)
    sprintf(tmp, "%s.wav", arg+1);
  else
    sprintf(tmp, "%s.wav", FILENAME);
  di->driver->data = sf_open(tmp, SFM_WRITE, &info);
  return GW_OK;
}

DRVRUN(sndfile_run) {
  SNDFILE* sf = (SNDFILE*)di->driver->data;
  m_float buf[di->si->out * BUFSIZE];
  while(vm->bbq->is_running) {
    for(m_uint i = 0; i < BUFSIZE; ++i) {
      di->run(vm);
      for(m_uint chan = 0; chan < di->si->out; ++chan)
        buf[2*i + chan] = vm->bbq->out[chan];
      next_bbq_pos(vm);
    }
    sf_write(sf, buf, di->si->out * BUFSIZE);
  }
}

DRVDEL(sndfile_del) {
  SNDFILE* sf = (SNDFILE*)di->driver->data;
  sf_close(sf);
}

GWDRIVER(Sndfile) {
  d->ini = sndfile_ini;
  d->run = sndfile_run;
  d->del = sndfile_del;
}
