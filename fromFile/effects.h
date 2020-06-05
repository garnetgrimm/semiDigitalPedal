#ifndef _EFFECTS_H_
#define _EFFECTS_H_

struct effect {
  virtual void step(double* sample, float t) = 0;
};

typedef struct reverb : effect {
  uint32_t bufferSize;
  double* buffer;
  uint32_t idx = 0;
  reverb() {
    bufferSize = 23000;
    buffer = (double*) calloc(bufferSize, sizeof(double));
  };
  reverb(uint32_t bufferSize):
  bufferSize(bufferSize)
  {
      buffer = (double*) calloc(bufferSize, sizeof(double));
  };
  void step(double* sample, float t) override {
      *sample += (buffer[idx % bufferSize] * .3);
      buffer[idx % bufferSize] = *sample;
      idx++;
  }
} _reverb;

typedef struct fuzz : effect {
  float gain;
  float volume;
  fuzz() {
    gain = 1;
    volume = 20;
  };
  fuzz(float gain, float volume):
  gain(gain),
  volume(volume)
  {};
  void step(double* sample, float t) override {
    double threshold = 1/gain;
    if(*sample < -threshold) *sample = -threshold;
    if(*sample > threshold) *sample = threshold;
    *sample *= volume;
  }
} _fuzz;

typedef struct overdrive : effect {
  float gain;
  float volume;
  overdrive() {
    gain = 60;
    volume = 100;
  };
  overdrive(float gain, float volume):
  gain(gain),
  volume(volume)
  {};
  void step(double* sample, float t) override {
    double threshold = 1 / gain;
    *sample = atan(*sample * (10 / M_PI)) * threshold * (2.0 / M_PI);
    *sample *= volume;
  }
} _overdrive;

typedef struct chorus: effect {
  uint32_t bufferSize = 1000;
  double* buffer;
  float readIdx = 0;
  uint32_t writeIdx = 0;
  float shift = 0;
  float velocity = 0;
  float wet = 0.5;
  chorus() {
    buffer = (double*) calloc(bufferSize, sizeof(double));
  };
  chorus(uint32_t bufferSize, int8_t octChange):
  bufferSize(bufferSize)
  {
      buffer = (double*) calloc(bufferSize, sizeof(double));
  };
  void step(double* sample, float t) override {
    //circular buffer
    //the incoming sample gets placed at the same index in each buffer
    //(reading each buffer is offset by buffSize/2)
    buffer[writeIdx % bufferSize] = *sample;
    writeIdx++;

    float amp = 0.01;
    float base = 1;
    float freq = 2;
    velocity = amp*sinf(2*M_PI*t*1)+amp+base;

    //when sample = 0 is uncommented, only the octave goes through, no clean
    *sample *= (1-wet);
    *sample += wet*buffer[(int)readIdx % bufferSize];

    //increase the buffer index by the expected amount
    readIdx += velocity;

  }
} _chorus;

typedef struct tremolo {
  tremolo() {
  };
  void step(double* sample, float t) {
    //*sample *= sinf (2. * M_PI * ((float) t / audioFile.getSampleRate()) * freq);
  }
} _tremolo;

typedef struct octave: effect {
  uint32_t bufferSize = 7500;
  double* buffer;
  float readIdx = 0;
  uint32_t writeIdx = 0;
  int8_t octChange = -1;
  float wet = 1;
  octave() {
    buffer = (double*) calloc(bufferSize, sizeof(double));
  };
  octave(uint32_t bufferSize, int8_t octChange):
  bufferSize(bufferSize),
  octChange(octChange)
  {
      buffer = (double*) calloc(bufferSize, sizeof(double));
  };
  void step(double* sample, float t) override {
    //circular buffer
    //the incoming sample gets placed at the same index in each buffer
    //(reading each buffer is offset by buffSize/2)
    buffer[writeIdx % bufferSize] = *sample;
    writeIdx++;

    //when sample = 0 is uncommented, only the octave goes through, no clean
    *sample *= (1-wet);
    *sample += wet*buffer[(int)readIdx % bufferSize];

    //scale it so that you can easily set negative octaves
    float scale = 0;
    if(octChange < 0) scale = 1.0f/(-octChange+1);
    if(octChange >= 0) scale = octChange;

    //increase the buffer index by the expected amount
    readIdx += scale;

  }
} _octave;

typedef struct noise_comp : effect {
  double avg = 0;
  int sampleWeight = 1;
  int volumeBoost = 1;
  int avgWeight = 9;
  noise_comp() {
    
  }
  noise_comp(int sampleWeight, int avgWeight, int volumeBoost) {
    this->sampleWeight = sampleWeight;
    this->avgWeight = avgWeight;
    this->volumeBoost = volumeBoost;
  }
  void step(double* sample, float t) override {
    avg = ((*sample*sampleWeight)+(avg*avgWeight))/(sampleWeight+avgWeight);
    *sample = avg*volumeBoost;
  }
} _noise_comp;

#endif
