#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
./lukas_kanade_qt -l 1.png -r frame01.png
