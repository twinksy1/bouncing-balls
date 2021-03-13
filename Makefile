MAIN=vec.o rendering.o gui.o circle.o main.o
HEADERS=*.h
ARGS=-Wall
EXE=main
LIBS=-lSDL2
CC=g++

all: $(EXE)

%.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@ $(ARGS)

$(EXE): $(MAIN)
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm $(EXE) $(MAIN)
