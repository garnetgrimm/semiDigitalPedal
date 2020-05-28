#ifndef _GRAPH_FFT_
#define _GRAPH_FFT_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "FFT.h"
#include "AudioFile.cpp"
#include <chrono>

class graphFFT {
  public:
    TTF_Font *font;
    AudioFile<double> audioFile;
    float FFTtime = 0;
    float highestFreq = 0;
    int running = 1;
    std::chrono::time_point<std::chrono::high_resolution_clock> lastRun;
    void drawGraph(char* filename);
    void drawRect(SDL_Renderer* rend, float x, float y, float width, float height, uint32_t color);
    void drawText(SDL_Renderer *rend, int x, int y, char *text);
    std::complex<double>* doFFT();
};

#endif
