#ifndef _GRAPH_FFT_
#define _GRAPH_FFT_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "FFT.h"
#include "AudioFile.h"

class graphFFT {
  public:
    TTF_Font *font;
    AudioFile<double> *audioFile;
    float FFTtime = 0;
    void drawGraph(AudioFile<double>* audioFile);
    void drawRect(SDL_Renderer* rend, float x, float y, float width, float height, uint32_t color);
    void drawText(SDL_Renderer *rend, int x, int y, char *text);
    std::complex<double>* doFFT();
};

#endif
