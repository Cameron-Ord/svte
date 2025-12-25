SRCS = src/main.cpp 
SRCS += src/core/io.cpp  
SRCS += src/util/util.cpp 
SRCS += src/core/buffer.cpp 
SRCS += src/events/events.cpp 
SRCS += src/renderer/font.cpp 
SRCS += src/renderer/renderer.cpp 
SRCS += src/window/window.cpp

TARGET = svte
BUILD_ROOT = build
BDIR = linux64
CFLAGS = -Wall -Wextra -Wpedantic -g -O1 -fsanitize=address,undefined,leak -fno-omit-frame-pointer -Wshadow -Wconversion -Wnull-dereference -Wdouble-promotion -std=c++20
all: linux 

linux: CC = g++
linux: TARGET = svte
linux: LIBS = -lSDL3 -lSDL3_ttf -lm
linux: BDIR = linux64
linux: $(BUILD_ROOT)
linux: $(BDIR)
linux: $(TARGET)

win64: CC = x86_64-w64-mingw32-g++
win64: TARGET = svte.exe
win64: LIBS = -lSDL3 -lSDL3_ttf -lm -mconsole
win64: BDIR = win64
win64: $(BUILD_ROOT)
win64: $(BDIR)
win64: $(TARGET)

$(BUILD_ROOT):
	mkdir -p $(BUILD_ROOT)

$(BDIR): 
	mkdir -p $(BUILD_ROOT)/$(BDIR)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(BUILD_ROOT)/$(BDIR)/$(TARGET) $(SRCS) $(LIBS)

clean:
	rm -r $(BUILD_ROOT)
