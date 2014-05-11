#!/bin/sh
SCRIBUSDIR=".."

export PYTHONPATH=$SCRIBUSDIR/codegen/cheetah/ 
python $SCRIBUSDIR/codegen/fromrelaxng/generate.py $SCRIBUSDIR/scribus/relaxng/schema/scribusdoc.rnc $SCRIBUSDIR/scribus/relaxng/templates $SCRIBUSDIR/scribus/relaxng

