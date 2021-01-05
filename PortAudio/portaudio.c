#include <portaudio.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "object.h"
#include "ugen.h"
#include "driver.h"
#include "plug.h"

#define SAMPLE_RATE (48000)
#define FRAMES_PER_BUFFER  (64)

struct PaInfo {
  PaStream *stream;
  PaStreamParameters outputParameters;
  PaStreamParameters  inputParameters;
};

static int callback(const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo __attribute__((unused)),
                    PaStreamCallbackFlags statusFlags __attribute__((unused)), void *userData) {
  VM* vm = (VM*)userData;
  float *in  = (float*)inputBuffer;
  float *out = (float*)outputBuffer;
  m_uint i, j;
  for(i = 0; i < framesPerBuffer; i++) {
//    for(j = 0; j < vm->bbq->si->in; j++)
//      vm->bbq->in[j] = *in++;
    vm->bbq->run(vm);
    for(j = 0; j < (m_uint)vm->bbq->si->out; j++)
      *out++ = vm->bbq->out[j];
    ++vm->bbq->pos;
  }
  return paContinue;
}

static DRVINI(portaudio_ini) {
  struct PaInfo* info = (struct PaInfo*)xmalloc(sizeof(struct PaInfo));
  di->driver->data = info;
  if(Pa_Initialize() != paNoError)
    return GW_ERROR;
  info->outputParameters.device = Pa_GetDefaultOutputDevice();
  if(info->outputParameters.device == paNoDevice) {
    gw_err("Error: No default output device.\n");
    goto error;
  }
  info->outputParameters.channelCount = 2;
//  info->outputParameters.sampleFormat = di->format;
  info->outputParameters.sampleFormat = paFloat32;
  info->outputParameters.suggestedLatency = Pa_GetDeviceInfo(info->outputParameters.device)->defaultLowOutputLatency;
  info->outputParameters.hostApiSpecificStreamInfo = NULL;

//  info->inputParameters.device = Pa_GetDefaultInputDevice(); /* default output device */
/*  if(info->inputParameters.device == paNoDevice) {
    gw_err("Error: No default input device.\n");
    goto error;
  }
  info->inputParameters.channelCount = 2;
  info->inputParameters.sampleFormat = paFloat32;
  info->inputParameters.suggestedLatency = Pa_GetDeviceInfo(info->inputParameters.device)->defaultLowOutputLatency;
  info->inputParameters.hostApiSpecificStreamInfo = NULL;
*/
  if(Pa_OpenStream(
        &info->stream,
NULL,//        &info->inputParameters,
        &info->outputParameters,
        di->si->sr,
128, //        di->bufsize,
        paClipOff,
        callback,
        vm) != paNoError)
    goto error;
  return GW_OK;
error:
  Pa_Terminate();
  return GW_ERROR;
}

static DRVDEL(portaudio_del) {
  struct PaInfo* info = (struct PaInfo*)di->driver->data;
  Pa_StopStream(info->stream);
  Pa_Terminate();
}

static DRVRUN(portaudio_run) {
  struct PaInfo* info = (struct PaInfo*)di->driver->data;
  Pa_StartStream(info->stream);
  while(vm->bbq->is_running)
    Pa_Sleep(1);
}

GWDRIVER(portaudio) {
  d->ini = portaudio_ini;
  d->run = portaudio_run;
  d->del = portaudio_del;
}
