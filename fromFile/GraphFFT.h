#ifndef _GRAPH_FFT_
#define _GRAPH_FFT_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "FFT.h"
#include "AudioFile.cpp"
#include <chrono>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define FFT_BUFF_SIZE 128
#define MAX_VOL 100

class FFTdata {
public:
  FFTdata() {};
  FFTdata(double frequency, double phase, double amplitude): frequency(frequency), phase(phase), amplitude(amplitude) {};
  double frequency;
  double phase;
  double amplitude;
};

class graphFFT {
  public:
    graphFFT() {};
    graphFFT(AudioFile<double> audioFile): audioFile(audioFile) {};
    TTF_Font *font;
    int playingAudio;
    AudioFile<double> audioFile;
    float FFTtime;
    float highestFreq;
    int running;
    std::chrono::time_point<std::chrono::high_resolution_clock> audioStart;
    void drawGraph(char* filename);
    void drawRect(SDL_Renderer* rend, float x, float y, float width, float height, uint32_t color);
    void drawText(SDL_Renderer *rend, int x, int y, char *text);
    void getDataFFT(std::complex<double>* vec, double t);
    void organizeFFT(std::complex<double>* vec, FFTdata* data);
    void playAudio(SDL_AudioDeviceID deviceId, uint32_t wavLength, uint8_t *wavBuffer);
};

#endif
