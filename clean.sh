#!/bin/bash
OK=" ..... ok"

echo -ne " * delete all 'folder'"
rm -rf 'bin/'
rm -rf 'output/'
rm -rf 'obj/'
rm -rf 'dv/'
echo $OK

echo -ne " * delete all 'Debug info'"
find . -iname '*.cbp' -delete
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
find . -iname 'CMakeFiles' -delete
find . -iname 'Makefile' -delete
find . -iname 'cmake_install.cmake' -delete
find . -iname 'install_manifest.txt' -delete
echo $OK

echo -ne " * clean dv output folder"
find -type d -regextype posix-egrep -regex '.*[0-9]_[0-9].*' -exec rm -rf {} \;
echo $OK

echo -ne " * clean astyle backup"
find -iname "*.orig" -delete
echo $OK

