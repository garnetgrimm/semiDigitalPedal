CXX=clang++
CFLAGS=-I.
DEPS = AudioFile.h FFT.h GraphFFT.h effects.h
OBJ = main.o GraphFFT.o FFT.o AudioFile.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

audio: $(OBJ)
	$(CXX) -lSDL2 -lSDL2_ttf -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ)
