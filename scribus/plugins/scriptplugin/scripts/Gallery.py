#!/usr/bin/env python
# -*- coding: utf-8 -*-
# Gallery.py
# © 2016.11.20  Gregory Pittman
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

"""
USAGE

Before running this script, you should have created and selected one or more image frames.
Run the script, after which the script informs you of your selection number, then asks for a 
file.

Once you choose an image, there is a loop that repeatedly asks for another image file. Hit Cancel
to break out of the loop.

You next are asked what width you wish frames to be resized to (default 250 points), but if you enter
0 (zero), no resizing will be done.
The script will quit either when you run out of selected frames or run out of images in the
selected directory.

See the wiki for further information: https://wiki.scribus.net/canvas/Gallery.py

HINT: Using the Outline dialog can facilitate frame selection, especially when you are selecting
frames on more than one page. The order of selection of frames seems to matter in trials of 
this script. The order of selection of images does determine the order in which they are placed.

"""

import sys
import os

try:
	import scribus
except ImportError:
	print 'Unable to import the scribus module. This script will only run within'
	print 'the Python interpreter embedded in Scribus. Try Script->Execute Script.'
	sys.exit(1)

try:
    from PIL import Image
    pil_found = 1
except ImportError:
    pil_found = 0

if not scribus.haveDoc():
	scribus.messageBox('Error','You must have a document open',scribus.ICON_WARNING,scribus.BUTTON_OK)
	sys.exit(2)

original_units = scribus.getUnit()
scribus.setUnit(scribus.UNIT_POINTS)
framecount = scribus.selectionCount()
if framecount == 0:
    scribus.messageBox('Scribus - Script Error','There is no object selected. Please select at least one image frame and try again.',scribus.ICON_WARNING, scribus.BUTTON_OK)
    sys.exit(2)
if framecount == 1:
        f = 'frame'
else:
        f = 'frames'
imageframes = []
images = []
frame = int(0)
nextimage = 0

scribus.messageBox('Image File', 'You have selected ' + str(framecount) + " " + f,scribus.ICON_NONE, scribus.BUTTON_OK)
while 1:
        image = scribus.fileDialog('Select Images', 'When finished, click Cancel(*.*)',haspreview=True)
        if image == '':
                break
        images.append(image)
nrimages = len(images)
frame_width = scribus.valueDialog('Size for Width', 'Change this to the standard width you desire, in points.\nChange to 0 to opt out of resizing.',"250")
if frame_width == "0":
    pil_found = 0 # now you pretend you don't have PIL
frame_width = float(frame_width)
while nextimage <= nrimages - 1:
    newimage = images[nextimage]
    currentframe = scribus.getSelectedObject(frame)
    if pil_found == 1:  # if you don't have PIL, frame will not be resized
        im = Image.open(newimage)
        xsize, ysize = im.size
        scribus.sizeObject(float(frame_width),float(frame_width*ysize/xsize),currentframe)
    scribus.loadImage(newimage, currentframe)
    scribus.setScaleImageToFrame(1,1,currentframe)
    frame += 1
    framecount -= 1
    nextimage += 1
    if framecount <= 0:
        break
scribus.setUnit(original_units)
