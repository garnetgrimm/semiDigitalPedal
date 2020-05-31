#include "effects.h"
#include <stdlib.h>

void step_fuzz(fuzz* f, int* sample) {
  if(*sample < f->center-f->threshold) *sample = f->center-f->threshold;
  if(*sample > f->center+f->threshold) *sample = f->center+f->threshold;
  //get back the amplitude we lost
  //*sample *= 2;
  //*sample *= ((float)(4095-f->threshold)/4095)*10;
}


void default_init_fuzz(fuzz* f) {
  f->threshold = 750;
  f->center = 4095/2;
}

void step_reverb(reverb* r, int* sample) {
  *sample += (r->buffer[r->idx % r->bufferSize] * r->fade);
  r->buffer[r->idx % r->bufferSize] = *sample;
  r->idx++;
}

void default_init_reverb(reverb* r) {
  r->idx = 0;
  r->bufferSize = 7500;
  r->buffer = (short*) calloc(r->bufferSize, sizeof(short));
  r->fade = 0.5;
}

void step_octave(octave* o, int* sample) {
  o->buffer[o->writeIdx % o->bufferSize] = *sample;
  o->writeIdx++;
  *sample = o->buffer[(int)o->readIdx % o->bufferSize];

  float scale = 0;
  if(o->octChange < 0) scale = -1.0f/(o->octChange-1);
  if(o->octChange >= 0) scale = o->octChange+1;
  o->readIdx += scale;
}

void default_init_octave(octave* o) {
  o->bufferSize = 7500;
  o->buffer = (short*) calloc(o->bufferSize, sizeof(short));
  o->readIdx = o->bufferSize/2;
  o->writeIdx = 0;
  o->octChange = -1;
}
