#include <stdio.h>
#include "AudioFile.h"
AudioFile<double> audioFile;
int main() {
    audioFile.load ("clean.wav");
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

    for (int i = 0; i < numSamples; i++)
    {
        double currentSample = audioFile.samples[channel][i];
        char buf[32];
        sprintf(buf, "%f\r\n", currentSample);
        //printf(buf);
    }
    return 0;
}