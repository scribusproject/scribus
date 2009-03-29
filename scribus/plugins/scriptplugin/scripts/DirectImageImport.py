#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" 

Diese Skript importiert ein Bild und setzt es auf die akutelle Seite.
Der Bildrahmen wird dem Bild angepasst und in den nicht-proportionalen Modus
gesetzt, das heisst, beliebige Verzerrungen sind moeglich.

Um das Bild proportional zu vergroessern, die STRG-Taste beim Bearbeiten druecken.

Tested with scribus 1.3.3.3

Author: Konrad Stania

some modifications 2009 by Gregory Pittman, tested on Scribus 1.3.3.13svn

This newer version uses the Python Imaging Library to get the dimensions of the
image to be imported, and adjusts the frame accordingly. Initially the frame will
be created centered, at 80% of the page's width or height, whichever is smaller. 
There is an adjustment to 80% of the height of the page in case this is exceeded 
by the initial calculation.

USAGE:

You must have a document open. Run the script, a dialog asks you to choose an
image to load. A proportional frame is automatically created and image loaded, 
then adjusted to frame size.

LICENSE:

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
name
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
"""

# Craig Bradney, Scribus Team
# 10/3/08: Added to Scribus 1.3.3.12svn distribution "as was" from Scribus wiki for bug #6826, script is GPLd

import sys


try:
    from scribus import *
    
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)
try:
    from PIL import Image
except ImportError:
    print "Unable to import the Python Imaging Library module."
    sys.exit(1)
    
def main():

    pageX,pageY = getPageSize()
    ImageFileName = fileDialog("Image Import", "*","" ,True, False)
    im = Image.open(ImageFileName)
    xsize, ysize = im.size

    if (pageX < pageY):
        Breite = pageX * 0.8
    else:
        Breite = pageY * 0.8
    Hoehe = Breite * ysize/xsize

# for images taller than they are wide we want to limit height of frame to 80% of page height
    if (Hoehe > pageY * 0.8):
	Hoehe = pageY * 0.8
	Breite = Hoehe * xsize/ysize

    ImageFrame = createImage(pageX/2 - Breite/2, pageY/2 - Hoehe/2, Breite, Hoehe)
    loadImage(ImageFileName, ImageFrame)
    setScaleImageToFrame(True, False,ImageFrame)
    setFillColor("None", ImageFrame)
    setLineColor("None", ImageFrame)

    
if __name__ == '__main__':
    if haveDoc():
        main()
    else:
        messageBox("Image Import", "You need to have a document open <i>before</i> you can run this script succesfully.", ICON_INFORMATION)