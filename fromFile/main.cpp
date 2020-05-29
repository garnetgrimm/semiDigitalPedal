#include <stdio.h>
#include "AudioFile.h"
#include "GraphFFT.h"
#include "effects.h"
#include <math.h>

using namespace std;
using namespace std::chrono;

AudioFile<double> audioFile;

int main() {
    audioFile.load ("res/clean.wav");
    if(!audioFile.isMono()) audioFile.samples.resize(1);
    int channel = 0;
    reverb myreverb = reverb();
    fuzz myfuzz = fuzz(1,1);
    octave myoctave = octave();
    cout << "Beginning transformation" << endl;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    {
        //abstract class!
        //myreverb.step(&audioFile.samples[channel][i]);
        //myfuzz.step(&audioFile.samples[channel][i]);
        myoctave.step(&audioFile.samples[channel][i]);
    }
    auto end = high_resolution_clock::now();
    double durr = std::chrono::duration<double>(end-start).count();
    printf("Took %f seconds (%f per each sample)\r\n", durr, durr/(FFT_BUFF_SIZE/2));
    audioFile.save ("res/octave.wav", AudioFileFormat::Wave);

    //graphFFT visualFFT;
    //visualFFT.drawGraph((char*)"res/440Hz.wav");
    return 0;
}
