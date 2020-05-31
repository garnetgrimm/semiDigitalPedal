#ifndef _EFFECTS_H_
#define _EFFECTS_H_

typedef struct _fuzz {
  int threshold;
  int center;
} fuzz;
void default_init_fuzz(fuzz* f);
void step_fuzz(fuzz* f, int* sample);

typedef struct _reverb {
  int bufferSize;
  short* buffer;
  int idx;
  double fade;
} reverb;
void default_init_reverb(reverb* r);
void step_reverb(reverb* r, int* sample);

typedef struct _octave {
  int bufferSize;
  short* buffer;
  float readIdx;
  int writeIdx;
  int octChange;
} octave;
void default_init_octave(octave* o);
void step_octave(octave* o, int* sample);


#endif
