#!/bin/bash
set -x
rm -r out
mkdir out
./latexicons.py latex_symbols template
cp predefined/* out
cd out
tar -cf ../../latex.tar *.png
cd ..
rm -f tmp tmp.aux tmp.dvi tmp.log