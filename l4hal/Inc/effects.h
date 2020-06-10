#ifndef _EFFECTS_H_
#define _EFFECTS_H_

#define PI 3.14159265358979323846
#define VOLUME_CENTER 4095/2
#define VOLUME_MAX 4095
#define VOLUME_MIN 0

typedef struct _antinoise {
  float avg;
  float sampleWeight;
  float volumeBoost;
  float avgWeight;
} antinoise;
void default_init_antinoise(antinoise *a);
void step_antinoise(antinoise *a, int* sample);

typedef struct _tremolo {
  short freq;
} tremolo;
void default_init_tremolo(tremolo* t);
void step_tremolo(tremolo* t, int* sample);

typedef struct _overdrive {
  int threshold;
  int softness;
} overdrive;
void default_init_overdrive(overdrive* o);
void step_overdrive(overdrive* o, int* sample);

typedef struct _fuzz {
  int threshold;
} fuzz;
void default_init_fuzz(fuzz* f);
void step_fuzz(fuzz* f, int* sample);

typedef struct _reverb {
  int bufferSize;
  short* buffer;
  int idx;
  float fade;
  int maxBufferSize;
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

typedef struct _chorus {
  int bufferSize;
  short* buffer;
  float readIdx;
  int writeIdx;
  float shift;
  float velocity;
  float wet;
  float amp;
  short base;
  float freq;
} chorus;
void default_init_chorus(chorus* c);
void step_chorus(chorus* c, int* sample);

#endif
