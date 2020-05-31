#ifndef _EFFECTS_H_
#define _EFFECTS_H_

struct effect {
  virtual void step(double* sample) = 0;
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
  void step(double* sample) override {
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
  void step(double* sample) override {
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
  void step(double* sample) override {
    double threshold = 1 / gain;
    *sample = atan(*sample * (10 / M_PI)) * threshold * (2.0 / M_PI);
    *sample *= volume;
  }
} _overdrive;

typedef struct chorus {
  chorus() {
  };
  void step(double* sample) {
  }
} _chorus;

typedef struct tremolo {
  tremolo() {
  };
  void step(double* sample) {
    //*sample *= sinf (2. * M_PI * ((float) t / audioFile.getSampleRate()) * freq);
  }
} _tremolo;

typedef struct octave: effect {
  uint32_t bufferSize = 5000;
  double* bufferA;
  double* bufferB;
  //NOTE NO OFFSET
  float readIdxA = 0;
  //NOTE THE OFFSET
  float readIdxB = bufferSize/2;
  uint32_t writeIdx = 0;
  int8_t octChange = -1;
  octave() {
    bufferA = (double*) calloc(bufferSize, sizeof(double));
    bufferB = (double*) calloc(bufferSize, sizeof(double));
  };
  octave(uint32_t bufferSize, int8_t octChange):
  bufferSize(bufferSize),
  octChange(octChange)
  {
      bufferA = (double*) calloc(bufferSize, sizeof(double));
      bufferB = (double*) calloc(bufferSize, sizeof(double));
  };
  void step(double* sample) override {
    //circular buffer
    //the incoming sample gets placed at the same index in each buffer
    //(reading each buffer is offset by buffSize/2)
    bufferA[writeIdx % bufferSize] = *sample;
    bufferB[writeIdx % bufferSize] = *sample;

    writeIdx++;

    //when sample = 0 is uncommented, only the octave goes through, no clean
    //*sample = 0;

    *sample += bufferA[(int)readIdxA % bufferSize];
    *sample += bufferB[(int)readIdxB % bufferSize];

    //scale it so that you can easily set negative octaves
    float scale = 0;
    if(octChange < 0) scale = 1.0f/(-octChange+1);
    if(octChange >= 0) scale = octChange;

    //increase the buffer index by the expected amount
    readIdxB += scale;
    readIdxA += scale;

  }
} _octave;


#endif
