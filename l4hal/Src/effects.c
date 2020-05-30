#include "effects.h"
#include <stdlib.h>

void step_reverb(reverb* r, int* sample) {
  *sample += (r->buffer[r->idx % r->bufferSize] * r->fade);
  r->buffer[r->idx % r->bufferSize] = *sample;
  r->idx++;
}

void default_init_reverb(reverb* r) {
  r->idx = 0;
  r->bufferSize = 2500;
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
  o->bufferSize = 1000;
  o->buffer = (short*) calloc(o->bufferSize, sizeof(short));
  o->readIdx = o->bufferSize/2;
  o->writeIdx = 0;
  o->octChange = -2;
}
