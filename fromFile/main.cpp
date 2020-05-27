#include <stdio.h>
#include "AudioFile.h"
#include "GraphFFT.h"
#include <math.h>
AudioFile<double> audioFile;
void fuzz(double gain, double volume, double* sample) {
    double threshold = 1/gain;
    if(*sample < -threshold) *sample = -threshold;
    if(*sample > threshold) *sample = threshold;
    *sample *= volume;
}
void reverb() {

}
void chorus() {

}
void octave() {

}
void overdrive() {

}
void tremolo(double freq, double t, double* sample) {
    *sample *= sinf (2. * M_PI * ((float) t / audioFile.getSampleRate()) * freq);
}

int main() {
    audioFile.load ("440Hz.wav");
    if(!audioFile.isMono()) {
        printf("Bad format, removing channel 1.\r\n");
        audioFile.samples.resize(1);
    }
    int sampleRate = audioFile.getSampleRate();
    int bitDepth = audioFile.getBitDepth();

    int numSamples = audioFile.getNumSamplesPerChannel();
    double lengthInSeconds = audioFile.getLengthInSeconds();

    int numChannels = audioFile.getNumChannels();
    bool isMono = audioFile.isMono();
    bool isStereo = audioFile.isStereo();

    // or, just use this quick shortcut to print a summary to the console
    audioFile.printSummary();
    int channel = 0;

    /*double gain = 60;
    double volume = 10;
    for (int i = 0; i < numSamples; i++)
    {
        float sample = sinf (2. * M_PI * ((float) i / sampleRate) * 440);
        //fuzz(60,10,&audioFile.samples[channel][i]);
        //tremolo(2, i, &audioFile.samples[channel][i]);

        audioFile.samples[channel][i] = sample;
        //double currentSample = audioFile.samples[channel][i];
        //char buf[32];
        //sprintf(buf, "%f\r\n", currentSample);
        //printf(buf);
    }*/
    graphFFT gfft;
    gfft.drawGraph(&audioFile);

    //audioFile.save ("440Hz.wav", AudioFileFormat::Wave);*/
    return 0;
}
