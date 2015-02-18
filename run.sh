#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
./lukas_kanade_qt -l input/frame00.png -r input/frame01.png -d
./lukas_kanade_qt -l input/frame00.png -r input/frame01.png -d -i 2
./lukas_kanade_qt -l input/frame00.png -r input/frame01.png -d -i 3
./lukas_kanade_qt -l input/frame00.png -r input/frame01.png -d -i 4
./lukas_kanade_qt -l input/frame00.png -r input/frame01.png -d -w 2
./lukas_kanade_qt -l input/frame00.png -r input/frame01.png -d -w 3
./lukas_kanade_qt -l input/frame00.png -r input/frame01.png -d -w 4
