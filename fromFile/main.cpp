#include <stdio.h>
#include "AudioFile.h"
#include "GraphFFT.h"
#include "effects.h"
#include <math.h>

using namespace std;
using namespace std::chrono;

AudioFile<double> audioFile;

int main() {
    audioFile.load ("res/noisy.wav");
    if(!audioFile.isMono()) audioFile.samples.resize(1);
    int channel = 0;

    reverb myreverb;
    fuzz myfuzz = fuzz(1,30);
    octave myoctave;
    chorus mychorus;
    noise_comp prenoisecomp = noise_comp(1,9,1);
    noise_comp postnoisecomp = noise_comp(1,5,1);
    effect* effect_list[2] = {&prenoisecomp, &postnoisecomp};
    //effect* effect_list[4] = {&prenoisecomp, &mychorus, &myreverb, &postnoisecomp};
    overdrive myoverdrive = overdrive(60,100);


    cout << "Beginning transformation" << endl;
    auto start = high_resolution_clock::now();
    for (int i = 0; i < audioFile.getNumSamplesPerChannel(); i++)
    {
      for(effect* e : effect_list) {
        e->step(&audioFile.samples[channel][i], (float)i/audioFile.getSampleRate());
      }
    }
    auto end = high_resolution_clock::now();
    double durr = std::chrono::duration<double>(end-start).count();
    printf("Took %f seconds (%f per each sample)\r\n", durr, durr/(FFT_BUFF_SIZE/2));
    audioFile.save ("res/denoised.wav", AudioFileFormat::Wave);

    //graphFFT visualFFT;
    //visualFFT.drawGraph((char*)"res/440Hz.wav");
    return 0;
}
