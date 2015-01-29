#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
./graduate-work -l 1.png -r frame01.png
