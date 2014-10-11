#!/bin/bash
rm *.pdf
g++ -Ofast tower_Of_Brahma.cpp
./a.out
chmod 755 script.sh
./script.sh
gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile=finished.pdf drawing*pdf
pdflatex movie.tex
pdflatex movie.tex
rm *.aux
rm *.log
rm a.out