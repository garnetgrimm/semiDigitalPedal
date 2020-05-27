#include <stdio.h>
#include "AudioFile.h"
#include "math.h"

AudioFile<double> audioFile;
const int bufferSize = 45000;
const int bufferSize2 = 35000;
double buffer[bufferSize] = {1.0};
int index = 0;

double chorusBuffer[bufferSize] = {0};
double chorusBuffer2[bufferSize2] = {0};

void fuzz(double gain, double volume, double* sample) {
    double threshold = 1/gain;
    if(*sample < -threshold) *sample = -threshold;
    if(*sample > threshold) *sample = threshold;
    *sample *= volume;
}
void reverb(double* sample) {
    *sample += (buffer[index % bufferSize] * .3);
    buffer[index % bufferSize] = *sample;
    index++;
}
void chorus(double* sample) {
    double chorusSample = chorusBuffer[index % bufferSize];
    chorusBuffer[index % bufferSize] = *sample;
    double chorusSample2 = chorusBuffer2[index % bufferSize2];
    chorusBuffer2[index % bufferSize2] = *sample;
    *sample += (chorusSample  + chorusSample2 );
    int increase = (5.0 * rand()) / RAND_MAX;
    increase = 5;
    index += increase;
}

void octave() {

}

void overdrive(double gain, double volume, double* sample) {
    double threshold = 1 / gain;
    *sample = atan(*sample * (10 / M_PI)) * threshold * (2.0 / M_PI);
    *sample *= volume;
}

void tremolo(double freq, double t, double* sample) {
    *sample *= sinf (2. * M_PI * ((float) t / audioFile.getSampleRate()) * freq); 
}

int main() {
    audioFile.load ("clean.wav");
    //audioFile.load ("the_best_song_ever.wav");
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

    double gain = 60;
    double volume = 10;
    for (int i = 0; i < numSamples; i++)
    {
        //float sample = sinf (2. * M_PI * ((float) i / sampleRate) * frequency); 
        //fuzz(60,10,&audioFile.samples[channel][i]);
        overdrive(60, 100, &audioFile.samples[channel][i]);

        //double currentSample = audioFile.samples[channel][i];
        //char buf[32];
        //sprintf(buf, "%f\r\n", currentSample);
        //printf(buf);
    }

    audioFile.save ("newfile.wav", AudioFileFormat::Wave);
    return 0;
}