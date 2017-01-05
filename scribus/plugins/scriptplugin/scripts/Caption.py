#!/usr/bin/env python
# -*- coding: utf-8  -*-

# ****************************************************************************
#  This program is free software; you can redistribute it and/or modify 
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
# 
# ****************************************************************************


"""

© 2017 Gregory Pittman

caption.py

Creates a text frame (caption) in selected location relative to 
one or more selected frames.

USAGE

Select one or more object (can be any kind of frame, or even imported vector graphics), 

run script. Dialog appears for choosing placement of all caption frames 

B/T/R/L (Bottom/Top/Right/Left).

Bottom and Top caption frames are sized to width of parent object and height 

of 24 points. Right and Left caption frames sized to 150 points width and 

40 points height.

"""

try:
    import scribus
except ImportError:
    print "Unable to import the 'scribus' module. This script will only run within"
    print "the Python interpreter embedded in Scribus. Try Script->Execute Script."
    sys.exit(1)

numselect = scribus.selectionCount()
count = 0
frames = []

if numselect == 0:
    scribus.messageBox('Selection Count', "You must have at least one object selected",
                       scribus.ICON_WARNING, scribus.BUTTON_OK)
    sys.exit(2)

captionloc = scribus.valueDialog("Caption Location","Where to put the caption(s) -\n B/T/R/L?", "b")
captionloc = captionloc[0]
location = captionloc.upper()

pageunits = scribus.getUnit()
scribus.setUnit(scribus.UNIT_POINTS)

while count < numselect:
    frames.append(scribus.getSelectedObject(count))
    count += 1
    
for frame in frames:
    fwidth, fheight = scribus.getSize(frame)
    fx, fy = scribus.getPosition(frame)
    if location == "B":
        textf = scribus.createText(fx, fy+fheight, fwidth, 24)
    elif location == "T":
        textf = scribus.createText(fx, fy-24, fwidth, 24)
    elif location == "R":
        textf = scribus.createText(fx + fwidth, fy, 150, 40)
    elif location == "L":
        textf = scribus.createText(fx-150, fy + fheight - 40, 150, 40)
scribus.setUnit(pageunits)

scribus.setRedraw(True)

