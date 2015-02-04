#!/bin/bash
OK=" ..... ok"

echo -ne " * delete all 'bin'"
rm -rf 'lukas_kanade_qt'
echo $OK

echo -ne " * delete all 'Makefile'"
find . -iname 'Makefile' -delete
echo $OK

echo -ne " * delete all '*.o'"
find . -iname '*.o' -delete
echo $OK

echo -ne " * delete output file"
rm -rf 'output'
echo $OK
