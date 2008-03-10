#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" 

Diese Skript importiert ein Bild und setzt es auf die akutelle Seite.
Der Bildrahmen wird dem Bild angepasst und in den nicht-proportionalen Modus
gesetzt, das heisst, beliebige Verzerrungen sind moeglich.

Um das Bild proportional zu vergroessern, die STRG-Taste beim Bearbeiten druecken.

Tested with scribus 1.3.3.3

Author: Konrad Stania

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
    
def main():
    #setRedraw(False) 

    pageX,pageY = getPageSize()
    ImageFileName = fileDialog("Image Import", "*","" ,True, False)
    Breite = pageX*0.8
    Hoehe = Breite
    
    if pageX >= pageY:
       Breite = pageY*0.8
       Hoehe = Breite

    ImageFrame = createImage(pageX/2 - Breite/2, pageY/2 - Hoehe/2, Breite, Hoehe)
    loadImage(ImageFileName, ImageFrame)
    setScaleImageToFrame(True, False,ImageFrame)
    setFillColor("None", ImageFrame)
    setLineColor("None", ImageFrame)
    scaleX,scaleY = getImageScale(ImageFrame)

    if scaleX > scaleY:
       Breite = Breite * scaleY / scaleX
       sizeObject(Breite, Hoehe, ImageFrame)
       setScaleImageToFrame(True, False,ImageFrame)

    if scaleX < scaleY:
       Hoehe = Hoehe * scaleX / scaleY
       setScaleImageToFrame(True, False,ImageFrame)
       sizeObject(Breite, Hoehe, ImageFrame)
 
    #setRedraw(True) 
    


    
if __name__ == '__main__':
    if haveDoc():
        main()
    else:
        messageBox("Image Import", "You need to have a document open <i>before</i> you can run this script succesfully.", ICON_INFORMATION)