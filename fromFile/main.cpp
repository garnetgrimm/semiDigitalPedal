#include <stdio.h>
#include "AudioFile.h"
#include "GraphFFT.h"
#include <math.h>

using namespace std;

AudioFile<double> audioFile;
const int bufferSize = 23000;
double buffer[bufferSize] = {1.0};
int idx = 0;
void fuzz(double gain, double volume, double* sample) {
    double threshold = 1/gain;
    if(*sample < -threshold) *sample = -threshold;
    if(*sample > threshold) *sample = threshold;
    *sample *= volume;
}
void reverb(double* sample) {
    *sample += (buffer[idx % bufferSize] * .3);
    buffer[idx % bufferSize] = *sample;
    idx++;
}
void chorus() {

}
void octave(graphFFT* gfft, double t, double* sample) {
  FFTdata fftd[FFT_BUFF_SIZE/2];
  complex<double> vec[FFT_BUFF_SIZE];
  gfft->getDataFFT(vec,t);
  gfft->organizeFFT(vec,fftd);
  *sample = 0;
  for(int i = 0; i < FFT_BUFF_SIZE/2; i++) {
    //*sample += 0.5*sin((2 * M_PI * (440) * (t / audioFile.getSampleRate())) + fftd[i].phase);
    //*sample += (fftd[i].amplitude/2)*cos((2 * M_PI * t) + fftd[i].phase);
    *sample += (fftd[i].amplitude);
  }
}
void overdrive() {

}
void tremolo(double freq, double t, double* sample) {
    *sample *= sinf (2. * M_PI * ((float) t / audioFile.getSampleRate()) * freq);
}

int main() {
    audioFile.load ("res/hello.wav");
    if(!audioFile.isMono()) audioFile.samples.resize(1);
    int channel = 0;
    graphFFT gfft = graphFFT(audioFile);
    gfft.audioStart = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    {
        //float sin1 = 0.2 * sinf (2. * M_PI * ((float) i / sampleRate) * (440+(500*((float)i/sampleRate))));
        //float sin2 = 0.2 * sinf (2. * M_PI * ((float) i / sampleRate) * (10000-(700*((float)i/sampleRate))));
        //float sample = sin1 + sin2;

        fuzz(1,20,&audioFile.samples[channel][i]);
        //tremolo(2, i, &audioFile.samples[channel][i]);
        //reverb(&audioFile.samples[channel][i]);
        //octave(&gfft, (double)i / sampleRate, &audioFile.samples[channel][i]);
        //audioFile.samples[channel][i] = sample;
        //double currentSample = audioFile.samples[channel][i];
        //char buf[32];
        //sprintf(buf, "%f\r\n", currentSample);
        //printf(buf);
    }
    audioFile.save ("res/hello_b.wav", AudioFileFormat::Wave);

    graphFFT visualFFT;
    visualFFT.drawGraph((char*)"res/hello_b.wav");
    return 0;
}
