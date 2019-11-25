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
#include "plug.h"
#include "driver.h"

// TODO: use arguments
#define BUFSIZE 256
#define FILENAME "gwion"

DRVINI(sndfile_ini) {
  SNDFILE** sf = (SNDFILE**)xcalloc(di->si->out, sizeof(void*));
  char tmp[140];
  SF_INFO info;
  info.samplerate = di->si->sr;
  info.channels = 1;
  info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
  m_uint chan;
  if(di->si->out == 1) {
    sprintf(tmp, "%s.wav", FILENAME);
    sf[0] = sf_open(tmp, SFM_WRITE, &info);
  } else for(chan = 0; chan < di->si->out; chan++) {
      sprintf(tmp, "%s_%02" UINT_F ".wav", FILENAME, chan);
      sf[chan] = sf_open(tmp, SFM_WRITE, &info);
  }
  di->driver->data = sf;
  return GW_OK;
}

DRVRUN(sndfile_run) {
  SNDFILE** sf = (SNDFILE**)di->driver->data;
  m_float buf[di->si->out][BUFSIZE];
  while(vm->bbq->is_running) {
    for(m_uint i = 0; i < 256; i++) {
      di->run(vm);
      for(m_uint chan = 0; chan < di->si->out; chan++)
        buf[chan][i] = vm->bbq->out[chan];
      ++vm->bbq->pos;
    }
    for(m_uint chan = 0; chan < di->si->out; chan++)
      sf_write(sf[chan], (const m_float*)buf[chan], BUFSIZE);
  }
}

DRVDEL(sndfile_del) {
  SNDFILE** sf = (SNDFILE**)di->driver->data;
  for(m_uint i = 0; i < di->si->out; i++)
    sf_close(sf[i]);
  free(sf);
}

GWMODSTR(sndfile)
GWDRIVER(sndfile) {
  d->ini = sndfile_ini;
  d->run = sndfile_run;
  d->del = sndfile_del;
}

