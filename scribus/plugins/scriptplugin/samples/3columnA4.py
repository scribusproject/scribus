#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" Creates 3 column layout on A4 paper and save it under 3columnA4.sla filename. This is a simple way to demonstrate creating a doc on the fly. """

try:
    # Please do not use 'from scribus import *' . If you must use a 'from import',
    # Do so _after_ the 'import scribus' and only import the names you need, such
    # as commonly used constants.
    import scribus
except ImportError,err:
    print "This Python script is written for the Scribus scripting interface."
    print "It can only be run from within Scribus."
    sys.exit(1)

def main(argv):
    """This is a simple way to demonstrate creating a doc on the fly. """

    pass    # <--- Delete this line
#########################
# YOUR IMPORTS GO HERE  #
#########################

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

margins = (50, 50, 50, 50)
size = (612, 792)

def main():
    if newDocument(PAPER_A4, margins, LANDSCAPE, 1, UNIT_POINTS, NOFACINGPAGES, FIRSTPAGELEFT,1):
        a = createText(50, 50, 230, 495)
        setTextAlignment(1,a)
        setText("Column A", a)
        setFontSize(12, a)
        b = createText(280, 50, 230, 495)
        setTextAlignment(1,b)
        setText("Column B", b)
        setFontSize(12, b)
        c = createText(510, 50, 230, 495)
        setTextAlignment(1,b)
        setText("Column C", c)
        setFontSize(12, c)
        saveDocAs("3columnA4.sla")
        

if __name__ == '__main__':
    main()
