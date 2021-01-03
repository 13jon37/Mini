#!/bin/bash
pushd build
gcc -c ../src/main.c -g -Wall && gcc *.o -o a.out -lSDL2main -lSDL2 -lSDL2_image 
popd