#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
./lukas_kanade_qt -l input/1.png -r input/frame01.png
