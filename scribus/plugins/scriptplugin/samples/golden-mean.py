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
"""
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

from math import sqrt
from scribus import *

GMLAYER = "Golden Mean Layer"

def goldenMean(aSize=0):
    """x = (?5-1)/2"""
    return aSize * ((sqrt(5) - 1)/2)


def createMark(X=0, Y=0, aName=""):
    """Create something like cross on page."""
    D = 10
    if ObjectExists(aName) == 1:
        return
    CreatePolyLine(
        [X-D, Y, X+D, Y, X, Y-D, X, Y+D,
         X-D, Y, X, Y-D, X, Y+D, X+D, Y], aName)


# main
if HaveDoc():
    # remember user settings
    unit = GetUnit()
    layer = GetActiveLayer()

    # set my environment - points needed
    SetUnit(0)
    # Paper format
    paper = PageDimension()
    
    layerExists = 0
    for i in GetLayers():
        if i == GMLAYER:
           layerExists = 1
    if layerExists == 0:
        CreateLayer(GMLAYER)
    SetActiveLayer(GMLAYER)
    SetLayerPrintable(GMLAYER, 0)
    
    createMark(goldenMean(paper[0]), goldenMean(paper[1]), "GoldenMeanA4-1")
    createMark(paper[0] - goldenMean(paper[0]), goldenMean(paper[1]),
                "GoldenMeanA4-2")
    createMark(goldenMean(paper[0]), paper[1] - goldenMean(paper[1]),
                "GoldenMeanA4-3")
    createMark(paper[0] - goldenMean(paper[0]), paper[1] - goldenMean(paper[1]),
                "GoldenMeanA4-4")

    # restore user settings
    SetUnit(unit)
    SetActiveLayer(layer)
