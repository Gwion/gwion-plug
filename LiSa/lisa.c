#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "plug.h"
#include "operator.h"
#include "import.h"
#include "ugen.h"
#include "array.h"
#include "gwi.h"

typedef struct Channel {
  m_float *gains;
  m_uint loop_start, loop_end;
  m_float gain, pan;
  m_float p_inc, pindex; // playback increment
  m_float rampup_len,
          rampdown_len,
          rampup_len_inv,
          rampdown_len_inv,
          rampctr;
  bool rampup, rampdown, loopplay, play, bi;
} Channel;

ANN static inline void channel_ini(Channel *const chan, const m_uint length /*, const m_uint nchan*/) {
  chan->loop_start = 0;
  chan->loop_end = length;
  chan->pindex = 0;
  chan->play = chan->bi = false;
  chan->loopplay = true;
  chan->p_inc = 1.;
  chan->gain = 1.;
  chan->pan = 0.5;
  chan->rampup = chan->rampdown = false;
  chan->rampup_len = chan->rampdown_len = 0.;
  chan->rampup_len_inv = chan->rampdown_len_inv = 1.;
  chan->rampctr = 0.;
}

enum SyncMode {
  INTERNAL,
  POSITION,
  DURATION
};

typedef struct LiSa {
  m_float * mdata;
  m_float * outsamples; // samples by channel to send out
  Channel *channels;
  m_uint mdata_len;
  m_uint maxvoices;
  m_uint loop_end_rec, max;
  m_uint rindex; // record and play indices
  enum SyncMode sync;
  m_uint nchan;
  m_float coeff; // feedback coeff
  m_float rec_ramplen, rec_ramplen_inv;
  bool record, looprec, reset, append;
} LiSa;

// allocate memory, length in samples
// TODO: order
ANN static inline void buffer_alloc(LiSa *const l, const m_uint length, const m_uint maxvoices, const m_uint nchan) {
  l->mdata = (m_float *)calloc((length + 1), sizeof(m_float)); // extra sample for safety....
  l->max = l->loop_end_rec = l->mdata_len = length;
  l->looprec = true;
//  l->coeff = 0.;
//  l->rindex = 0;
  l->record = false;
  l->maxvoices = maxvoices;
//  l->sync = INTERNAL;
  l->nchan = nchan;
  for(m_uint i = 0; i < maxvoices; i++)
    channel_ini(&l->channels[i], length/*, nchan*/);
}

// grab a sample from the buffer, with linear interpolation (add prc's SINC interp later)
// increment play index
// which specifies voice number
ANN static inline m_float getNextSamp(LiSa *const l, Channel *const chan) {
  if(chan->loopplay) {
    if(chan->bi) { // change direction if bidirectional mode
      if(chan->pindex >= chan->loop_end || chan->pindex < chan->loop_start) { //should be >= ?
        chan->pindex  -=  chan->p_inc;
        chan->p_inc    = -chan->p_inc;
      }
    }
    //catch this condition to avoid infinite while loops
		if(chan->loop_start == chan->loop_end) chan->pindex = chan->loop_start;
		else {
			while(chan->pindex >= chan->loop_end) chan->pindex = chan->loop_start + (chan->pindex - chan->loop_end); //again, >=?
			while(chan->pindex < chan->loop_start) chan->pindex = chan->loop_end - (chan->loop_start - chan->pindex);
		}
  } else if(chan->pindex >= l->mdata_len || chan->pindex < 0) {
    chan->play = 0;
    return (m_float) 0.;
  }
  // interp
  m_uint whereTrunc = (m_uint)chan->pindex;
  const m_float whereFrac = chan->pindex - (m_float)whereTrunc;
  m_uint whereNext = whereTrunc + 1;
  if(chan->loopplay) {
		if((whereNext) >= chan->loop_end)
			whereNext = chan->loop_start;
		if((whereTrunc) >= chan->loop_end)
      whereTrunc = chan->loop_start;
  } else {
    if((whereTrunc) >= l->mdata_len) {
			whereTrunc = l->mdata_len - 1;
			whereNext = 0;
    }
		if((whereNext) >= l->mdata_len)
			whereNext = 0;
	}
  chan->pindex += chan->p_inc;
  const m_float *data = l->mdata;
  m_float outsample = data[whereTrunc] + (data[whereNext] - data[whereTrunc]) * whereFrac;
  // ramp stuff
  if(chan->rampup) {
    outsample *= chan->rampctr++ * chan->rampup_len_inv; //remove divide
    if(chan->rampctr >= chan->rampup_len) chan->rampup = false;
  } else if(chan->rampdown) {
    outsample *= (chan->rampdown_len - chan->rampctr++) * chan->rampdown_len_inv;
    if(chan->rampctr >= chan->rampdown_len) {
      chan->rampdown = false;
      chan->play = false;
    }
  }
  return outsample * chan->gain;
}

// dump a sample into the buffer; retain existing sample, scaled by "coeff"
ANN static inline void recordSamp(LiSa *const l, const m_float insample) {
  m_float tempsample;
  if(l->record) {
    if(l->looprec) {
      if(l->rindex >= l->loop_end_rec) l->rindex = l->channels[0].loop_start;
      tempsample = l->coeff * l->mdata[l->rindex] + insample;
    } else {
      if(l->rindex < l->loop_end_rec)
        tempsample = l->coeff * l->mdata[l->rindex] + insample;
      else {
        l->record = 0;
        return;
      }
    }
    // ramp stuff here
    if(l->rindex < l->rec_ramplen)
      tempsample *= (l->rindex * l->rec_ramplen_inv);
    else if(l->rindex > (l->loop_end_rec - l->rec_ramplen))
      tempsample *= (l->loop_end_rec - l->rindex) * l->rec_ramplen_inv;
    l->mdata[l->rindex] = tempsample;
    l->rindex++;
  }
}

// grab a sample from the buffer, with linear interpolation (add prc's SINC interp later)
// given a position within the buffer
ANN static inline m_float getSamp(LiSa *const l, Channel *const chan, m_float where) {
  // constrain
  if(where > chan->loop_end)   where = chan->loop_end;
  else if(where < chan->loop_start) where = chan->loop_start;
  const m_uint whereTrunc = (m_uint) where;
  const m_float whereFrac = where - (m_float)whereTrunc;
  m_uint whereNext = whereTrunc + 1;
  if(whereNext == chan->loop_end) whereNext = chan->loop_start;
  const m_float *const data = l->mdata;
  const m_float outsample = data[whereTrunc] + (data[whereNext] - data[whereTrunc]) * whereFrac;
  return outsample *chan->gain;
}

// for simple stereo panning of a particular voice, and...
// l.channelGain(voice, channel, gain)
// to set the gain for a particular voice going to a particular channel;
// ...good for > 2 voices (like 6 channels!)
ANN static inline m_float * tick_multi(LiSa *const l, m_float in) {
  m_float tempsample = 0;
  for(m_uint i = 0; i < l->nchan; i++) l->outsamples[i] = 0.; // improve?
  if(!l->mdata) return l->outsamples;
  recordSamp(l, in);
  if(l->sync == INTERNAL) {
    for(m_uint i=0; i < l->maxvoices; i++) {
      Channel *const chan = &l->channels[i];
      if(chan->play) {
        tempsample = getNextSamp(l, chan);
        for(m_uint j=0; j < l->nchan; j++)
          l->outsamples[j] += tempsample * chan->gains[j];
      }
    }
  } else if(l->sync == POSITION) {
    if(in < 0.0) in = -in;
    for(m_uint i = 0; i < l->maxvoices; i++) {
      Channel *const chan = &l->channels[i];
      if(chan->play) {
        m_float location = chan->loop_start + in * (chan->loop_end - chan->loop_start);
        tempsample = getSamp(l, chan, location);
        for(m_uint j = 0; j < l->nchan; j++)
          l->outsamples[j] += tempsample * chan->gains[j];
      }
    }
  } else if(l->sync == DURATION && l->channels[0].play) {
    if(in < 0.0) in = -in; // only use voice 0 when tracking with durs.
      tempsample = getSamp(l, &l->channels[0], in);
    for(m_uint j = 0; j < l->nchan; j++)
      l->outsamples[j] += tempsample * l->channels[0].gains[j];
  }
  return l->outsamples;
}

ANN static inline void ramp_up(Channel *const chan, const m_float uptime) {
  chan->rampup = true;
  chan->play = true;
  chan->rampup_len = uptime;
  if(chan->rampup_len > 0.) chan->rampup_len_inv = 1./chan->rampup_len;
  else chan->rampup_len = 1.;
  if(chan->rampdown) {
    chan->rampctr = chan->rampup_len * (1. - chan->rampctr/chan->rampdown_len);
    chan->rampdown = false;
  } else chan->rampctr = 0;
}

ANN static inline void ramp_down(Channel *const chan, const m_float downtime) {
  chan->rampdown = true;
  chan->rampdown_len = downtime;
  if(chan->rampdown_len > 0.) chan->rampdown_len_inv = 1./chan->rampdown_len;
  else chan->rampdown_len = 1.;
  if(chan->rampup) {
    chan->rampctr = chan->rampdown_len * (1. - chan->rampctr/chan->rampup_len);
    chan->rampup = false;
  } else chan->rampctr = 0;
}

ANN static inline void set_rec_ramplen(LiSa *const l, m_float newlen) {
  l->rec_ramplen = (m_float)newlen;
  if(l->rec_ramplen > 0.) l->rec_ramplen_inv = 1. / l->rec_ramplen;
  else l->rec_ramplen_inv = 1.;
}

ANN static inline void clear_buf(LiSa *const l) {
  for(m_uint i = 0; i < l->mdata_len; i++)
    l->mdata[i] = 0.;
}

ANN static inline m_int get_free_voice(LiSa *const l) {
  m_uint which = 0;
  while(l->channels[which].play && which < l->maxvoices)
    which++;
  return which != l->maxvoices
      ? (m_int)which
      : -1;
}

// stick sample in record buffer
ANN static inline void pokeSample(LiSa *const l, const m_float insample, m_int index) {
  // constrain
  if(index < 0) index = 0;
  else if(index >= (m_int)l->mdata_len) index = l->mdata_len;
  l->mdata[index] = insample;
}

// grab sample directly from record buffer, with linear interpolation
ANN static inline m_float grabSample(LiSa *const l, m_float where) {
  // constrain
  if(where > l->mdata_len) where = l->mdata_len;
  else if(where < 0) where = 0;
  m_float *const data = l->mdata;
  const m_int whereTrunc = (m_int) where;
  const m_float whereFrac = where - (m_float)whereTrunc;
  m_int whereNext = whereTrunc + 1;
  if(whereNext == (m_int)l->mdata_len) whereNext = 0;
  const m_float outsample = (m_float)data[whereTrunc] + (m_float)(data[whereNext] - data[whereTrunc]) * whereFrac;
  return (m_float)outsample; // * gain?
}

#define GET_LISA(o) (LiSa*)(o->data + SZ_INT)
#define LISA_CHAN(shred, l, offset) \
  const m_int idx = *(m_int*)MEM(SZ_INT);\
  if(idx < 0 || idx >= (m_int)l->maxvoices) {\
    xfun_handle(shred, offset, "InvalidLiSaInit");\
    return;\
  }\

static TICK(LiSa_tick) {
  LiSa *const l = u->module.gen.data;
	u->out = tick_multi(l, u->in)[0];
}

static TICK(LiSa_tickf) {
  LiSa *const l = u->module.gen.data;
  for(m_uint i = 0; i < u->connect.multi->n_chan; i++) {
    const M_Object chan = u->connect.multi->channel[i];
    m_float * temp_out_samples = tick_multi(l, UGEN(chan)->in);
    UGEN(chan)->out = temp_out_samples[i];
  }
}

static MFUN(LiSa_new) {
  LiSa *const l = GET_LISA(o);
  *(M_Object*)RETURN = o;
  const m_int   nchans   = *(m_int*)MEM(SZ_INT*1);
  const m_int   voices   = *(m_int*)MEM(SZ_INT*2);
  const m_float length   = *(m_float*)MEM(SZ_INT*3);
  if(nchans < 0 || voices < 0 || length < 0) {
    handle(shred, "InvalidLiSaInit");
    return;
  }
  l->nchan = nchans;
  l->maxvoices = voices;
  const size_t sz = sizeof(m_float) * nchans;
  l->channels = mp_calloc2(shred->info->mp, sizeof(struct Channel) * l->maxvoices);
  for(m_uint i = 0; i < l->maxvoices; i++)
    l->channels[i].gains = mp_calloc2(shred->info->mp, sz);
  buffer_alloc(l, length, voices, nchans);
  ugen_ini(shred->info->vm->gwion, UGEN(o), nchans, nchans);
  ugen_gen(shred->info->vm->gwion, UGEN(o), nchans > 1 ? LiSa_tickf : LiSa_tick, l, 0);
// we may not need outsamples
  l->outsamples = mp_calloc2(shred->info->mp, sz);

  // set ugens;
}

static DTOR(LiSa_dtor) {
  LiSa *const l = GET_LISA(o);
  const size_t sz = sizeof(m_float) * l->nchan;
//  for(m_uint i = 0; i < l->maxvoices; i++)
//    mp_free2(shred->info->mp, sz, l->channels[i].gains);
  mp_free2(shred->info->mp, sizeof(struct Channel) * l->maxvoices, l->channels);
// we may not need outsamples
  mp_free2(shred->info->mp, sz, l->outsamples);
  // free LiSa stuff
}

static MFUN(LiSa_set_size) {
  LiSa *const l = GET_LISA(o);
  const m_float buflen = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = buflen > l->max
    ? l->max
    : buflen;
}

static MFUN(LiSa_get_size) {
  LiSa *const l = GET_LISA(o);
  *(m_float*)RETURN = (m_float)l->mdata_len;
}

static MFUN(LiSa_set_record) {
  LiSa *const l = GET_LISA(o);
  *(m_int*)RETURN = l->record = *(bool*)MEM(SZ_INT);
}

static MFUN(LiSa_set_play) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT*2);
  struct Channel *const chan = &l->channels[idx];
  *(m_uint*)RETURN = chan->play = *(m_uint*)MEM(SZ_INT);
  chan->rampdown = false;
  chan->rampup = false;
}

static MFUN(LiSa_get_play) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  *(m_uint*)RETURN = chan->play;
}

static MFUN(LiSa_set_rate) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT + SZ_FLOAT);
  struct Channel *const chan = &l->channels[idx];
  *(m_float*)RETURN = chan->p_inc = *(m_float*)MEM(SZ_INT);
}

static MFUN(LiSa_get_rate) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  *(m_float*)RETURN = chan->p_inc;
}

static MFUN(LiSa_set_pindex) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT+SZ_FLOAT);
  struct Channel *const chan = &l->channels[idx];
  *(m_float*)RETURN = chan->pindex = *(m_float*)MEM(SZ_INT);
}

static MFUN(LiSa_get_pindex) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  *(m_float*)RETURN = (m_float)chan->pindex;
}

static MFUN(LiSa_set_rindex) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT+ SZ_FLOAT);
// check rindex
  *(m_float*)RETURN = (l->rindex = (m_int)*(m_float*)MEM(SZ_INT));
}

static MFUN(LiSa_get_rindex) {
  LiSa *const l = GET_LISA(o);
  *(m_float*)RETURN = (m_float)l->rindex;
}

static MFUN(LiSa_set_lstart) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT*2);
// check loop_start
  struct Channel *const chan = &l->channels[idx];
  const m_int loop_start = *(m_float*)MEM(SZ_INT);
	if (loop_start < 0) chan->loop_start = 0;
	else if (chan->loop_start >= l->mdata_len) chan->loop_start = l->mdata_len-1;
  else chan->loop_start = loop_start;
  *(m_float*)RETURN = (m_float)chan->loop_start;
}

static MFUN(LiSa_get_lstart) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  *(m_float*)RETURN = (m_float)chan->loop_start;
}

static MFUN(LiSa_set_lend) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT+SZ_FLOAT);
  struct Channel *const chan = &l->channels[idx];
  chan->loop_end = (m_int)*(m_float*)MEM(SZ_INT);
	//check to make sure loop_end is not too large
	if (chan->loop_end >= l->mdata_len) chan->loop_end = l->mdata_len - 1;
	*(m_float*)RETURN = (m_float)chan->loop_end;
}

static MFUN(LiSa_get_lend) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  *(m_float*)RETURN = (m_float)chan->loop_end;
}

static MFUN(LiSa_set_loop) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT*2);
  struct Channel *const chan = &l->channels[idx];
  *(m_uint*)RETURN = chan->loopplay = *(m_uint*)MEM(SZ_INT);
}

static MFUN(LiSa_get_loop) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  *(m_uint*)RETURN = (m_int)chan->loopplay;
}

static MFUN(LiSa_set_bi) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT*2);
  struct Channel *const chan = &l->channels[idx];
  *(m_uint*)RETURN = chan->bi = *(bool*)MEM(SZ_INT);
}

static MFUN(LiSa_get_bi) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  *(m_uint*)RETURN = chan->bi;
}

static MFUN(LiSa_set_loop_end_rec) {
  LiSa *const l = GET_LISA(o);
  l->loop_end_rec = (m_uint)*(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = (m_float)l->loop_end_rec;
}

static MFUN(LiSa_get_loop_end_rec) {
  LiSa *const l = GET_LISA(o);
  *(m_float*)RETURN = (m_float)l->loop_end_rec;
}

static MFUN(LiSa_set_loop_rec) {
  LiSa *const l = GET_LISA(o);
  l->looprec = (*(m_uint*)RETURN = *(m_uint*)MEM(SZ_INT));
}

static MFUN(LiSa_get_loop_rec) {
  LiSa *const l = GET_LISA(o);
  *(m_uint*)RETURN = l->looprec;
}

static MFUN(LiSa_set_sample) {
  LiSa *const l = GET_LISA(o);
  const m_int index_in = *(m_int*)(MEM(SZ_INT));
	const m_float sample_in = *(m_float*)RETURN = *(m_float*)MEM(SZ_INT + SZ_FLOAT);
	pokeSample(l, sample_in, index_in);
}

static MFUN(LiSa_get_sample) {
  LiSa *const l = GET_LISA(o);
	const m_float index_in = *(m_float*)MEM(SZ_INT);
  *(m_float*)RETURN = grabSample(l, index_in); //change this to getSamp for interpolation
}

static MFUN(LiSa_set_gain) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT+SZ_FLOAT);
  struct Channel *const chan = &l->channels[idx];
   *(m_float*)RETURN = chan->gain = *(m_float*)MEM(SZ_INT);
}

static MFUN(LiSa_get_gain) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  *(m_float*)RETURN = chan->gain;
}

static MFUN(LiSa_set_pan) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT+SZ_FLOAT);
  struct Channel *const chan = &l->channels[idx];
  chan->pan = *(m_float*)MEM(SZ_INT);
  // reset channel gains for voice
  for(m_uint i = 0; i < l->nchan; i++) chan->gains[i] = 0.;
  for(m_uint i = 0; i < l->nchan; i++) {
    Channel *const chan  = &l->channels[i];
    // get truncated value (0.4 -> 0; 5.6 -> 5) as channel index
    m_uint panTrunc = (m_uint)chan->pan;
    // if channel of interest
    if(i == panTrunc) {
      // calculate pan value
      chan->gains[i] = 1.0 - (chan->pan-(m_float)i);
      if(l->nchan > 1) {
        // handle the case that pan is between last and first channel
        if(i == l->nchan - 1) {
          // compute the first channel
          chan->gains[0] = 1. - chan->gains[i];
          chan->gains[0] = sqrt(chan->gains[0]);
        } else {
          // compute the adjacent channel pan
          chan->gains[i+1] = 1. - chan->gains[i];
          chan->gains[i+1] = sqrt(chan->gains[i+1]);
        }
      }
      // calculate sqrt
      chan->gains[i] = sqrt(chan->gains[i]);
		}
	}
  *(m_float*)RETURN = chan->pan;
}

static MFUN(LiSa_get_pan) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  *(m_float*)RETURN = chan->pan;
}

static MFUN(LiSa_set_coeff) {
  LiSa *const l = GET_LISA(o);
	*(m_float*)RETURN = l->coeff = *(m_float*)MEM(SZ_INT);
}

static MFUN(LiSa_get_coeff) {
  LiSa *const l = GET_LISA(o);
  *(m_float*)RETURN = l->coeff;
}

static MFUN(LiSa_get_clear) {
  LiSa *const l = GET_LISA(o);
  clear_buf(l);
}

static MFUN(LiSa_get_voice) {
  LiSa *const l = GET_LISA(o);
  *(m_int*)RETURN = (m_int)get_free_voice(l);
}

static MFUN(LiSa_set_rampup) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT+SZ_FLOAT);
  struct Channel *const chan = &l->channels[idx];
  const m_float len = *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  ramp_up(chan, len);
}

static MFUN(LiSa_set_rampdown) {
  LiSa *const l = GET_LISA(o);
  LISA_CHAN(shred, l, SZ_INT+ SZ_FLOAT);
  const m_float len = *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  struct Channel *const chan = &l->channels[idx];
  ramp_down(chan, len);
}

static MFUN(LiSa_set_rec_ramplen) {
  LiSa *const l = GET_LISA(o);
  const m_float newramplen = *(m_float*)RETURN = *(m_float*)MEM(SZ_INT);
  set_rec_ramplen(l, newramplen);
}

static MFUN(LiSa_set_sync) {
  LiSa *const l = GET_LISA(o);
  *(m_int*)RETURN = l->sync = *(m_int*)MEM(SZ_INT);
}

static MFUN(LiSa_get_sync) {
  LiSa *const l = GET_LISA(o);
  *(m_int*)RETURN = l->sync;
}

GWION_IMPORT(LiSa) {
  gwidoc(gwi, "a (li)ve (sa)mpling unit generator; also popularly used for granular synthesis.");
  DECL_OB(const Type, t_lisa, = gwi_class_ini(gwi, "LiSa", "UGen"));
  gwi_class_xtor(gwi, NULL, LiSa_dtor);
  SET_FLAG(t_lisa, abstract | ae_flag_final);
  t_lisa->nspc->offset += sizeof(LiSa);

  GWI_BB(gwi_func_ini(gwi, "auto", "new"));
  GWI_BB(gwi_func_arg(gwi, "int", "nchan"));
  GWI_BB(gwi_func_arg(gwi, "int", "nvoice"));
  GWI_BB(gwi_func_arg(gwi, "dur", "duration"));
  GWI_BB(gwi_func_end(gwi, LiSa_new, ae_flag_none));

  #define SET(type, name, func)                             \
  GWI_BB(gwi_func_ini(gwi, type, name));                    \
  GWI_BB(gwi_func_arg(gwi, type, "val"));                   \
  GWI_BB(gwi_func_end(gwi, LiSa_set_##func, ae_flag_none));

  #define GET(type, name, func)                             \
  GWI_BB(gwi_func_ini(gwi, type, name));                    \
  GWI_BB(gwi_func_end(gwi, LiSa_get_##func, ae_flag_none));

  #define SETGET(type, name, func)                          \
  SET(type, name, func);                                    \
  GET(type, name, func);

  #define VOICE_SET(type, name, func)                       \
  GWI_BB(gwi_func_ini(gwi, type, name));                    \
  GWI_BB(gwi_func_arg(gwi, "int", "voice"));                \
  GWI_BB(gwi_func_arg(gwi, type, "val"));                   \
  GWI_BB(gwi_func_end(gwi, LiSa_set_##func, ae_flag_none)); \


  #define VOICE_SETGET(type, name, func)                    \
  VOICE_SET(type, name, func)                               \
  GWI_BB(gwi_func_ini(gwi, type, name));                    \
  GWI_BB(gwi_func_arg(gwi, "int", "voice"));                \
  GWI_BB(gwi_func_end(gwi, LiSa_get_##func, ae_flag_none));

  // set/get buffer size
  SETGET("dur", "duration", size);

  // start/stop recording
  SET("bool", "record", record);

  // start/stop playing
  VOICE_SETGET("bool", "play", play);

  // set/get playback rate
  VOICE_SETGET("float", "rate", rate);

  // playback position
  VOICE_SETGET("dur", "playPos", pindex);

  // record position
  SETGET("dur", "recPos", rindex);

  // loopstart position
  VOICE_SETGET("dur", "loopStart", lstart);

  // loopend position
  VOICE_SETGET("dur", "loopEnd", lend);

  // loop
  VOICE_SETGET("bool", "loop", loop);

  // bidirectional looping
  VOICE_SETGET("bool", "bi", bi);

  // loopend_rec position
  SETGET("dur", "loopEndRec", loop_end_rec);

	// loop_rec toggle set; for turning on/off loop recording
  SETGET("bool", "loopRec", loop_rec);

	// look at or put sample directly in record buffer, do not pass go
  GWI_BB(gwi_func_ini(gwi, "float", "valueAt"));
  GWI_BB(gwi_func_arg(gwi, "dur", "index"));
  GWI_BB(gwi_func_arg(gwi, "float", "val"));
  GWI_BB(gwi_func_end(gwi, LiSa_set_sample, ae_flag_none));
  GWI_BB(gwi_func_ini(gwi, "float", "valueAt"));
  GWI_BB(gwi_func_arg(gwi, "dur", "index"));
  GWI_BB(gwi_func_end(gwi, LiSa_get_sample, ae_flag_none));

	// set/get gain
  VOICE_SETGET("float", "gain", gain);

	// set/get voicePan [value between 0 and numchans-1, to place voice]
  VOICE_SETGET("float", "pan", pan);

  // set record feedback coefficient
  SETGET("float", "feedback", coeff);

  // clear buffer
  GET("void", "clear", clear);
//  GWI_BB(gwi_func_ini(gwi, "void", "clear"));
//  GWI_BB(gwi_func_arg(gwi, "bool", "state"));
//  GWI_BB(gwi_func_end(gwi, LiSa_set_clear, ae_flag_none));

  // get free voice
  GET("int", "voice", voice);

  // ramp stuff
  VOICE_SET("dur", "rampUp", rampup);
  VOICE_SET("dur", "rampDown", rampdown);
  SET("dur", "recRamp", rec_ramplen);

  GWI_BB(gwi_enum_ini(gwi, "Sync"));
  GWI_BB(gwi_enum_add(gwi, "internal", INTERNAL));
  GWI_BB(gwi_enum_add(gwi, "position", POSITION));
  GWI_BB(gwi_enum_add(gwi, "duration", DURATION));
  GWI_OB(gwi_enum_end(gwi));
  // sync
  SETGET("Sync", "sync", sync);

  return gwi_class_end(gwi);
}
