#!/bin/bash
OK=" ..... ok"

echo -ne " * delete all 'bin'"
rm -rf 'bin/'
rm -rf 'output/'
rm -rf 'obj/'
echo $OK

echo -ne " * delete all 'Makefile'"
find . -iname 'Makefile' -delete
echo $OK

echo -ne " * delete all '*.o'"
find . -iname '*.o' -delete
echo $OK

echo -ne " * delete doxygen documentation"
rm -rf 'latex/'
rm -rf 'html/'
echo $OK

echo -ne " * delete CMake File"
rm -rf 'CMakeFiles/'
find . -iname '*.cmake' -delete
find . -iname 'CMakeCache.txt' -delete
find . -iname 'CMakeLists.txt.user' -delete
find . -iname '*.pro.user' -delete
echo $OK
