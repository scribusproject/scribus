#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" When you have an image selected this script creates small text legende
(caption) below the image. The new textframe contains name of the file. """

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

import os

def main():
    userUnit = getUnit()
    setUnit(1)
    sel_count = selectionCount()

    if sel_count == 0:
        messageBox("legende.py",
                "Please select the object to add a caption to before running this script.",
                ICON_INFORMATION)
        sys.exit(1)

    x,y = getPosition()
    l,h = getSize()
    texte = getImageFile()
    image = os.path.basename(texte)
    a = createText(x,y+h+2,l,8)
    insertText(image,0,a)
    setTextAlignment(2,a)
    setFontSize(7,a)
    setUnit(userUnit)

if __name__ == '__main__':
    main()
