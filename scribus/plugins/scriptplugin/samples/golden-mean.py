#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Golden Mean for Scribus.

This script creates supplementary marks on the page to
help design the "right" layout in golden mean (golden
ratio).
The marks are located in new non-printable layer.

See www.yarpen.cz, py.cz and scribus.net for fresh
versions to come...

REQUIREMENTS:
Scribus - CVS version later 02/24/2004 or later release 1.5

MORE INFO:
See e.g.
http://home.att.net/~vmueller/prop/theo.html
or Google for more theory :)

CONTACT:
email : petr@yarpen.cz
Feature requests and bug reports welcomed


LICENSE:

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
"""

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

from math import sqrt

GMLAYER = "Golden Mean Layer"

def goldenMean(aSize=0):
    """x = (?5-1)/2"""
    return aSize * ((sqrt(5) - 1)/2)


def createMark(X=0, Y=0, aName=""):
    """Create something like cross on page."""
    D = 10
    if objectExists(aName) == 1:
        return
    createPolyLine(
        [X-D, Y, X+D, Y, X, Y-D, X, Y+D,
         X-D, Y, X, Y-D, X, Y+D, X+D, Y], aName)


def main():
    # remember user settings
    unit = getUnit()
    layer = getActiveLayer()

    # set my environment - points needed
    setUnit(0)
    # Paper format
    paper = pageDimension()

    layerExists = 0
    for i in getLayers():
        if i == GMLAYER:
           layerExists = 1
    if layerExists == 0:
        createLayer(GMLAYER)
    setActiveLayer(GMLAYER)
    setLayerPrintable(GMLAYER, 0)

    createMark(goldenMean(paper[0]), goldenMean(paper[1]), "GoldenMeanA4-1")
    createMark(paper[0] - goldenMean(paper[0]), goldenMean(paper[1]),
                "GoldenMeanA4-2")
    createMark(goldenMean(paper[0]), paper[1] - goldenMean(paper[1]),
                "GoldenMeanA4-3")
    createMark(paper[0] - goldenMean(paper[0]), paper[1] - goldenMean(paper[1]),
                "GoldenMeanA4-4")

    # restore user settings
    setUnit(unit)
    setActiveLayer(layer)

if __name__ == '__main__':
    if haveDoc():
        main()
    else:
        messageBox("Golden Mean.py", "Please run this script with a document already open", ICON_INFORMATION);
