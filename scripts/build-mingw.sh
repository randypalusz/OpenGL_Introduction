#!/bin/bash

# run this from the base project directory
rm -rf build
mkdir -p build/mingw
mkdir -p build/exe
cd build/mingw
cmake --no-warn-unused-cli -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -G "MinGW Makefiles" ../..
make -j 16

cd ../
for file in $(find . -name "*.cc")
do
  #echo $file
  ln -s ".$file" exe/
done