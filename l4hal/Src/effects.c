#include "effects.h"
#include <stdlib.h>
#include "math.h"

void step_tremolo(tremolo* t, int* sample) {
  *sample *= sinf(2*PI*t->freq*((float)HAL_GetTick()/1000));
}

void default_init_tremolo(tremolo *t) {
  t->freq = 4;
}

void step_overdrive(overdrive* o, int* sample) {
  *sample = (o->threshold*2/PI)*atanf((*sample-VOLUME_CENTER)/o->softness) + VOLUME_CENTER;
}

void default_init_overdrive(overdrive* o) {
  o->threshold = VOLUME_CENTER;
  o->softness = 500;
}

void init_overdrive();

void step_fuzz(fuzz* f, int* sample) {
  if(*sample < VOLUME_CENTER-f->threshold) *sample = VOLUME_CENTER-f->threshold;
  if(*sample > VOLUME_CENTER+f->threshold) *sample = VOLUME_CENTER+f->threshold;
}

void default_init_fuzz(fuzz* f) {
  f->threshold = 650;
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

void step_chorus(chorus* c, int* sample) {
  c->buffer[c->writeIdx % c->bufferSize] = *sample;
  c->writeIdx++;

  c->velocity = c->amp*sinf(2*PI*((float)HAL_GetTick()/1000)*c->freq)+c->amp+c->base;
  *sample *= (1-c->wet);
  *sample += c->wet*c->buffer[(int)c->readIdx % c->bufferSize];

  c->readIdx += c->velocity;
}

void default_init_chorus(chorus* c) {
  c->bufferSize = 1000;
  c->buffer = (double*) calloc(c->bufferSize, sizeof(double));
  c->readIdx = 0;
  c->writeIdx = 0;
  c->shift = 0;
  c->velocity = 0;
  c->wet = 0.75;
  c->amp = 0.005;
  c->base = 1;
  c->freq = 1;
}
