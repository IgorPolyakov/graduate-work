#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
./lukas_kanade_qt -l input/I1.png -r input/I2.png
