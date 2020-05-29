#include "GraphFFT.h"

using namespace std;
using namespace std::chrono;

void graphFFT::getDataFFT(complex<double>* vec, double t) {
  for (int i = 0; i < FFT_BUFF_SIZE; i++)
  {
    int idx = (t*audioFile.getSampleRate()) - i;
    if(idx < 0) idx = 0;
    if(idx > audioFile.getNumSamplesPerChannel()) break;
    vec[i] = audioFile.samples[0][idx];
  }
}

void graphFFT::organizeFFT(complex<double>* vec, FFTdata* data) {
  DSP::FFT(vec, FFT_BUFF_SIZE, 1);
  for (int i = 0; i < FFT_BUFF_SIZE/2; i++)
  {
    double freq = (i*audioFile.getSampleRate()/(FFT_BUFF_SIZE/2));
    double amplitude = vec[i].real();
    double phase = vec[i].imag();
    data[i] = FFTdata(freq,amplitude,phase);
  }
}

void graphFFT::drawText(SDL_Renderer *rend, int x, int y, char *text) {
      SDL_Color textColor = { 0, 0, 255, 255 };
      SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
      SDL_Texture *textTexture = SDL_CreateTextureFromSurface(rend, textSurface);

      SDL_Rect textRect;
      textRect.x = x;
      textRect.y = y;
      textRect.w = 50;
      textRect.h = 50;
      SDL_RenderCopy(rend, textTexture, NULL, &textRect);
      SDL_FreeSurface(textSurface);
}

void graphFFT::drawRect(SDL_Renderer* rend, float x, float y, float width, float height, uint32_t color) {
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = width;
  rect.h = height;

  int r = ((color & 0xFF0000) >> 16);
  int g = ((color & 0x00FF00) >>  8);
  int b = ((color & 0x0000FF) >>  0);

  SDL_SetRenderDrawColor(rend, r, g, b, 255);
  SDL_RenderFillRect(rend, &rect);
}

void graphFFT::playAudio(SDL_AudioDeviceID deviceId, uint32_t wavLength, uint8_t *wavBuffer) {
  int success = SDL_QueueAudio(deviceId, wavBuffer, wavLength);
  //SDL_PauseAudioDevice(deviceId, 0);
  audioStart = std::chrono::high_resolution_clock::now();
  playingAudio = 1;
}

void graphFFT::drawGraph(char* filename) {
    audioFile.load(filename);
    avgMax = 0;

    SDL_Window *window;                    // Declare a pointer
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    SDL_Init(SDL_INIT_AUDIO);
    TTF_Init();

    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength);
    SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        WINDOW_WIDTH,                               // WINDOW_WIDTH, in pixels
        WINDOW_HEIGHT,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
    }
    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("CascadiaMono.ttf", FFT_BUFF_SIZE);

    running = 1;
    // The window is open: could enter program loop here (see SDL_PollEvent())
    //complex<double>* samples = (complex<double>*) calloc(FFT_BUFF_SIZE,sizeof(complex<double>));
    int sCount = FFT_BUFF_SIZE/2;
    complex<double> vec[FFT_BUFF_SIZE];
    FFTdata samples[sCount];
    while(running) {
      SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
      SDL_RenderClear(rend);
      auto now = high_resolution_clock::now();
      double playTime = std::chrono::duration<double>(now-audioStart).count();
      getDataFFT(vec, playTime);
      organizeFFT(vec, samples);
      int max = 0;
      for(int row = 0; row < sCount; row++) {
        if(samples[row].amplitude > max) max = row;
        if(playingAudio) drawRect(rend,row*(WINDOW_WIDTH/sCount),WINDOW_HEIGHT,(WINDOW_WIDTH/sCount),-(samples[row].amplitude*WINDOW_HEIGHT)/MAX_VOL,0xff00ff);
      }
      avgMax = (avgMax + max) / 2;
      cout << avgMax << endl;
      if(playingAudio) drawRect(rend,avgMax*(WINDOW_WIDTH/sCount),WINDOW_HEIGHT,(WINDOW_WIDTH/sCount),-(samples[avgMax].amplitude*WINDOW_HEIGHT)/MAX_VOL,0x00ff00);
      SDL_RenderPresent(rend);

      SDL_Event event;
      while (SDL_PollEvent(&event)) {
          switch (event.type) {
          case SDL_QUIT:
              running = 0;
              break;
          case SDL_KEYDOWN:
            // keyboard API for key pressed
            switch (event.key.keysym.scancode) {
              case SDL_SCANCODE_SPACE:
                playAudio(deviceId, wavLength, wavBuffer);
                break;
              }
            break;
          }
        }
    }

    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    // Close and destroy the window
    SDL_DestroyWindow(window);
    // Clean up
    SDL_Quit();
}
