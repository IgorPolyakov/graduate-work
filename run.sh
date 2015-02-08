#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
./lukas_kanade_qt -l input/frame00.png -r input/frame01.png -d
