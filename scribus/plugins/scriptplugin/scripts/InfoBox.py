#!/usr/bin/env python
# -*- coding: utf-8 -*-

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


# Craig Bradney, Scribus Team
# 10/3/08: Added to Scribus 1.3.3.12svn distribution "as was" from Scribus wiki for bug #6826, script is GPLd

"""
(C) 2005 by Thomas R. Koll, <tomk32@gmx.de>, http://verlag.tomk32.de

A simple script for exact placment of a textbox (infobox)
over the current textbox, asking the user for the width
of the infobox and in which column to place it.
I normally use it for a table-like box above the regular text.

USAGE
Select a textframe, start the script and have phun
Default name for the infobox is 'infobox' + name_of_selected_frame

TODO
* ask for height
* ask for name
* ask for content?

"""

import sys
import re
import string

try:
    from scribus import *
except ImportError:
    print "Unable to import the 'scribus' module. This script will only run within"
    print "the Python interpreter embedded in Scribus. Try Script->Execute Script."
    sys.exit(1)

def main(argv):
    unit = getUnit()
    setUnit(UNIT_MILLIMETERS)
    if selectionCount() == 0:
        messageBox('Scribus - Script Error',
            "There is no object selected.\nPlease select a text frame and try again.",
            ICON_WARNING, BUTTON_OK)
        sys.exit(2)
    try:
        textbox = getSelectedObject()
        left, top = getPosition(textbox)
        o_width, o_height = getSize(textbox)
        o_cols = int(getColumns(textbox))
        o_gap = getColumnGap(textbox)
    except:
        messageBox('Scribus - Script Error', 
             "Can't get size of object, maybe not a textframe", ICON_WARNING, BUTTON_OK)
        sys.exit(2)
    columns_width = 0
    column_pos = 1
    o_colwidth = (o_width - ((o_cols - 1) * o_gap)) / o_cols
    if (o_cols > 1):
        while (columns_width > o_cols or columns_width < 1):
            columns_width = valueDialog('Width',
                                            'How many columns width shall the '+
                                            'box be (max ' + str(o_cols) + ')?')
            columns_width = int(columns_width)
        if (columns_width < o_cols):
            max = o_cols - columns_width
            while (column_pos <= max and column_pos <= 1):
                column_pos = valueDialog('Placement',
                                         'In which column do you want '
                                         'to place the box (1 to ' +
                                         str(o_cols) + ')?')
            column_pos = int(column_pos) - 1 
    new_width = columns_width * o_colwidth + (columns_width-1) * o_gap
    new_left = left + ((column_pos) * o_colwidth) + ((column_pos) * o_gap)
    new_textbox = createText(new_left, top, new_width, o_height,
                             "infobox " + textbox)
    setColumnGap(0, new_textbox)
    setColumns(1, new_textbox)
    setUnit(unit)

if __name__ == '__main__':
    # This script makes no sense without a document open
    if not haveDoc():
        messageBox('Scribus - Script Error', "No document open", ICON_WARNING, BUTTON_OK)
        sys.exit(1)
    # Disable redraws
    setRedraw(False)
    # Run the main script, ensuring redraws are re-enabled even if the
    # script aborts with an exception, and don't fail with an exception
    # even if the document is closed while the script runs.
    try:
        main(sys.argv)
    finally:
        try:
            setRedraw(True)
        except:
            pass