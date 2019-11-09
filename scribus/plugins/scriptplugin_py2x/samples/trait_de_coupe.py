#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" Draws a "crop marks" around selected object """

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

def main():
    userUnit = getUnit()
    setUnit(1)
    x,y = getPosition()
    l,h = getSize()
    t1 = createLine(x, y-2, x, y-7)
    setLineWidth(0.1, t1)
    t2 = createLine(x+l, y-2, x+l, y-7)
    setLineWidth(0.1, t2)
    t3 = createLine(x, y+7+h, x, y+2+h)
    setLineWidth(0.1, t3)
    t4 = createLine(x+l, y+7+h, x+l, y+2+h)
    setLineWidth(0.1, t4)
    t5 = createLine(x-2, y, x-7, y)
    setLineWidth(0.1, t5)
    t6 = createLine(x-2, y+h, x-7, y+h)
    setLineWidth(0.1, t6)
    t7 = createLine(x+l+2, y+h, x+l+7, y+h)
    setLineWidth(0.1, t7)
    t7 = createLine(x+l+2, y, x+l+7, y)
    setLineWidth(0.1, t7)
    deselectAll()
    setUnit(userUnit)

if __name__ == '__main__':
    if haveDoc() and selectionCount():
        main()
    else:
        messageBox("trait_de_coupe.py", "Please select an object to put crop marks around<i>before</i> running this script.", ICON_INFORMATION)
