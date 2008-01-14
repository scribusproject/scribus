#!/bin/bash
rm -r out
mkdir out
./latexicons.py latex_symbols template
cd out
tar -cf ../../latex.tar *.png
cd ..
rm -f tmp tmp.aux tmp.dvi tmp.log