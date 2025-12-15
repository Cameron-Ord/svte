x86_64-w64-mingw32-g++ \
   src/*.cpp \
  -o svte.exe \
  -I/usr/x86_64-w64-mingw32/sys-root/mingw/include/SDL2 \
  -L/usr/x86_64-w64-mingw32/sys-root/mingw/lib \
  -lSDL2main -lSDL2 -lSDL2_ttf -mwindows -lm -g -Wall -Wpedantic

