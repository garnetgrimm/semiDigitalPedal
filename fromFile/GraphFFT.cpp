#include "GraphFFT.h"

complex<double>* graphFFT::doFFT() {
  int FFTBuffSize = 128;
  int sampleRate = audioFile->getSampleRate();
  complex<double> vec[FFTBuffSize];
  FFTtime++;
  for (int i = 0; i < FFTBuffSize; i++)
  {
    vec[i] = sinf(2. * M_PI * ((FFTtime+i)/sampleRate) * (440+FFTtime));
  }
  FFT(vec, FFTBuffSize, 1);
  complex<double> normalized[FFTBuffSize];
  float biggestFound = 0;
  for (int i = 0; i < FFTBuffSize; i++)
  {
    if(abs(vec[i].real()) > biggestFound) biggestFound = abs(vec[i].real());
  }
  for (int i = 0; i < FFTBuffSize; i++)
  {
    normalized[i] = complex<double>((i*sampleRate/FFTBuffSize), abs(vec[i].real())/biggestFound);
  }
  return normalized;
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

void graphFFT::drawGraph(AudioFile<double>* audioFile) {
    this->audioFile = audioFile;
    SDL_Window *window;                    // Declare a pointer
    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2
    TTF_Init();
    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );
    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
    }
    SDL_Renderer* rend = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    font = TTF_OpenFont("CascadiaMono.ttf", 128);
    // The window is open: could enter program loop here (see SDL_PollEvent())
    int running = 1;
    complex<double>* samples = (complex<double>*) calloc(128,sizeof(complex<double>));
    while(running) {

      SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
      SDL_RenderClear(rend);
      complex<double>* samples = doFFT();
      for(int row = 0; row < 128; row++) {
        drawRect(rend,row*(640/128),480,(640/128),-samples[row].imag()*480,0xff00ff);
        /*char buf[32];
        sprintf(buf, "%d\r\n", (int)samples[row].real());
        drawText(rend,row*(640/128),480-(samples[row].imag()*480),buf);*/
      }
      SDL_RenderPresent(rend);

      SDL_Event event;
      while (SDL_PollEvent(&event)) {
          switch (event.type) {
          case SDL_QUIT:
              running = 0;
              break;
          }
        }
    }
    // Close and destroy the window
    SDL_DestroyWindow(window);
    // Clean up
    SDL_Quit();
}
