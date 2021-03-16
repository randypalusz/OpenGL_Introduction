#!/bin/bash

clean=0

while [ -n "$1" ]; do # while loop starts
	case "$1" in
	--clean) echo "--clean option passed, regenerating all build files" 
           clean=1 ;; 
	*) echo "Option $1 not recognized" ;; # In case you typed a different option other than a,b,c
	esac

	shift
done
 

if [ "$clean" -eq "1" ]
then
  # if clean is selected, remove the entire build directory
  rm -rf build
else
  # else, just remove the project files
  rm -rf build/exe
  rm -rf build/res
  rm -rf build/*.cc
  rm -rf build/mingw/CMakeFiles/OpenGL_Test.dir
  rm -rf build/mingw/CMakeFiles/Example.dir
fi


# run this from the base project directory
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