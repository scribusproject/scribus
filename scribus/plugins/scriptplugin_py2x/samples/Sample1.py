#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" A sample script """

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

margins = (10, 10, 10, 30)

def main():
    if newDocument(PAPER_A4, margins, PORTRAIT, 1, UNIT_POINTS, NOFACINGPAGES, FIRSTPAGERIGHT, 1):
        a = createText(50, 50, 200, 80)
        setText("A Test for Scribus", a)
        setFontSize(20, a)
        b = createEllipse(267, 391, 60, 60)
        setFillColor("Red", b)
        saveDocAs("Sample1.sla")

if __name__ == '__main__':
    main()
