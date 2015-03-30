#!/bin/bash
OK=" ..... ok"

echo -ne " * delete all 'bin'"
rm -rf 'bin/'
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