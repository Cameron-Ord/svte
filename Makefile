SRCS = src/main.cpp src/io.cpp src/util.cpp src/buffer.cpp src/events.cpp
TARGET = svte
BDIR = build

all: linux


linux: CC = g++
linux: TARGET = svte
linux: LIBS = -lSDL2 -lSDL2_ttf -lm
linux: CFLAGS = -Wall -Wextra -Wpedantic -g
linux: BDIR = linux64
linux: clean
linux: $(BDIR)
linux: $(TARGET)

win64: CC = x86_64-w64-mingw32-g++
win64: TARGET = svte.exe
win64: LIBS = -lSDL2 -lSDL2main -lSDL2_ttf -mwindows -lm
win64: CFLAGS = -Wall -Wextra -Wpedantic -g 
win64: BDIR = win64
win64: clean
win64: $(BDIR)
win64: $(TARGET)

$(BDIR): 
	mkdir -p $(BDIR)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(BDIR)/$(TARGET) $(SRCS) $(LIBS)

clean:
	rm -r $(BDIR)
