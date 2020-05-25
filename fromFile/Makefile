CXX=clang++
CFLAGS=-I.
DEPS = AudioFile.h
OBJ = main.o

%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $< $(CFLAGS)

audio: $(OBJ)
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	rm $(OBJ)