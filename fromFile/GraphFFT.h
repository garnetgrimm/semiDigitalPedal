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
#define MAX_VOL 20

class FFTdata {
public:
  FFTdata();
  double getFreq();
  double getPhase();
  double getVolume();
private:
  double freq;
  double phase;
  double volume;
};

class graphFFT {
  public:
    TTF_Font *font;
    AudioFile<double> audioFile;
    float FFTtime;
    float highestFreq;
    int running;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastRun;
    void drawGraph(char* filename);
    void drawRect(SDL_Renderer* rend, float x, float y, float width, float height, uint32_t color);
    void drawText(SDL_Renderer *rend, int x, int y, char *text);
    void getDataFFT(std::complex<double>* vec);
    void prepGraphFFT(std::complex<double>* normalized);
};

#endif
