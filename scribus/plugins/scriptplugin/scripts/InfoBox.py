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
(C) 2005 by Thomas R. Koll, <tomk32@gmx.de>, http://verlag.tomk32.de
(c) 2008 modifications, additional features by Gregory Pittman

A simple script for exact placement of a frame (infobox)
over the current textbox, asking the user for the width
of the infobox and in which column to place it.
Some enhancements:
* You can now create a text frame or an image frame, and also load
an image.
* More than one infobox can be added to a text frame
* Height and Y-Pos of top of infobox can be specified
* Works with any page unit - pts, mm, in, and picas
* Infobox has Text Flows Around Frame activated, also
  Scale Image to Frame for images

USAGE
Select a textframe, start the script and have phun
Default name for the infobox is 'infobox' + name_of_selected_frame,
but this can be changed.


"""

try:
    import scribus
except ImportError:
    print "Unable to import the 'scribus' module. This script will only run within"
    print "the Python interpreter embedded in Scribus. Try Script->Execute Script."
    sys.exit(1)
try:
    from PIL import Image
except ImportError:
    print "Unable to import the Python Imaging Library module."
    sys.exit(1)

def main(argv):
    unit = scribus.getUnit()
    units = [' pts','mm',' inches',' picas','cm',' ciceros']
    unitlabel = units[unit]
    if scribus.selectionCount() == 0:
        scribus.messageBox('Scribus - Script Error',
            "There is no object selected.\nPlease select a text frame and try again.",
            scribus.ICON_WARNING, scribus.BUTTON_OK)
        sys.exit(2)
    if scribus.selectionCount() > 1:
        scribus.messageBox('Scribus - Script Error',
            "You have more than one object selected.\nPlease select one text frame and try again.",
            scribus.ICON_WARNING, scribus.BUTTON_OK)
        sys.exit(2)
    textbox = scribus.getSelectedObject()
    pageitems = scribus.getPageItems()
    boxcount = 1
    for item in pageitems:
        if (item[0] == textbox):
            if (item[1] != 4):
                scribus.messageBox('Scribus - Script Error', 
                          "This is not a textframe. Try again.", scribus.ICON_WARNING, scribus.BUTTON_OK)
                sys.exit(2)

# While we're finding out what kind of frame is selected, we'll also make sure we
# will come up with a unique name for our infobox frame - it's possible we may want
# more than one for a multicolumn frame.
        if (item[0] == ("infobox" + str(boxcount) + textbox)):
                boxcount += 1
    left, top = scribus.getPosition(textbox)
    o_width, o_height = scribus.getSize(textbox)
    o_cols = int(scribus.getColumns(textbox))
    o_gap = scribus.getColumnGap(textbox)
            
    columns_width = 0
    column_pos = 0
    o_colwidth = (o_width - ((o_cols - 1) * o_gap)) / o_cols
    if (o_cols > 1):
        while (columns_width > o_cols or columns_width < 1):
            columns_width = scribus.valueDialog('Width',
                                            'How many columns width shall the '+
                                            'box be (max ' + str(o_cols) + ')?','1')
            columns_width = int(columns_width)
        if (columns_width < o_cols):
            max = o_cols - columns_width
            while (column_pos <= max and column_pos <= 1):
                column_pos = scribus.valueDialog('Placement',
                                         'In which column do you want '
                                         'to place the box (1 to ' +
                                         str(o_cols) + ')?','1')
            column_pos = int(column_pos) - 1 
    if (o_cols == 1):
	columns_width = 1
    new_height = 0
    while (new_height == 0):
        new_height = scribus.valueDialog('Height','Your frame height is '+ str(o_height) +
                                                 unitlabel +'. How tall\n do you want your ' +
                                                 'infobox to be in '+ unitlabel +'?\n If you load an image, height will be\n calculated, so the value here does not\n matter.', str(o_height))
    new_top = -1
    while (new_top < 0):
        new_top = scribus.valueDialog('Y-Pos','The top of your infobox is currently\n'+ str(top) +
                                                 unitlabel +'. Where do you want \n' +
                                                 'the top to be in '+ unitlabel +'?', str(top))
    framename = scribus.valueDialog('Name of Frame','Name your frame or use this default name',"infobox" + str(boxcount) + textbox)
    frametype = 'text'
    frametype = scribus.valueDialog('Frame Type','Change to anything other\n than "text" for image frame.\nEnter "imageL" to also load an image',frametype)
    new_width = columns_width * o_colwidth + (columns_width-1) * o_gap
    new_left = left + ((column_pos) * o_colwidth) + ((column_pos) * o_gap)
    if (frametype == 'text'):
        new_textbox = scribus.createText(new_left, float(new_top), new_width, float(new_height),framename)
        scribus.setColumnGap(0, new_textbox)
        scribus.setColumns(1, new_textbox)
        scribus.textFlowMode(new_textbox, 1)
    else:
        if (frametype == 'imageL'):
	    imageload = scribus.fileDialog('Load image','Images(*.jpg *.png *.tif *.JPG *.PNG *.jpeg *.JPEG *.TIF)',haspreview=1)
            im = Image.open(imageload)
            xsize, ysize = im.size
	    new_height = float(ysize)/float(xsize)*new_width
	    new_image = scribus.createImage(new_left, float(new_top), new_width, float(new_height),framename)
	    scribus.loadImage(imageload, new_image)
        else:
	    new_image = scribus.createImage(new_left, float(new_top), new_width, float(new_height),framename)
        scribus.textFlowMode(new_image, 1)
        scribus.setScaleImageToFrame(scaletoframe=1, proportional=1, name=new_image)
if __name__ == '__main__':
    # This script makes no sense without a document open
    if not scribus.haveDoc():
        scribus.messageBox('Scribus - Script Error', "No document open", scribus.ICON_WARNING, scribus.BUTTON_OK)
        sys.exit(1)
    # Disable redraws
    scribus.setRedraw(False)
    # Run the main script, ensuring redraws are re-enabled even if the
    # script aborts with an exception, and don't fail with an exception
    # even if the document is closed while the script runs.
    try:
        main(sys.argv)
    finally:
        try:
            scribus.setRedraw(True)
        except:
            pass

