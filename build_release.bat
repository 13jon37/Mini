@echo off
if not exist build mkdir build
pushd build
gcc -c ..\src\main.c -s -O3 -m64 -I include -I C:\SDL2-w64\include && gcc *.o -o main_release.exe -mwindows -L C:\SDL2-w64\lib -lmingw32 -lgdi32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
popd


