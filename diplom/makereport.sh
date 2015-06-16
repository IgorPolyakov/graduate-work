#!/bin/bash

pdflatex main.tex
bibtex main.aux
bibtex main.aux
pdflatex main.tex
rm -f *.ps *.dvi *.aux *.toc *.idx *.ind *.ilg *.log *.out *.brf *.blg *.bbl
evince main.pdf &
