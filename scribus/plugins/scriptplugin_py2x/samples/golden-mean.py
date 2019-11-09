#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Golden Mean for Scribus.

This script creates supplementary guides on the page to
help design the "right" layout in golden mean (golden
ratio).

See scribus.net and CVS for fresh versions to come...

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
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
"""

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

from math import sqrt


def goldenMean(aSize=0):
    """x = (?5-1)/2"""
    return aSize * ((sqrt(5) - 1)/2)


def main():
    # remember user settings
    unit = getUnit()
    # set my environment - points needed
    setUnit(0)
    # Paper format
    paper = pageDimension()
    # set the guides. The get* functions are for "remembering" the old ones...
    setVGuides(getVGuides() + [goldenMean(paper[0]), paper[0] - goldenMean(paper[0])])
    setHGuides(getHGuides() + [goldenMean(paper[1]), paper[1] - goldenMean(paper[1])])
    # restore user settings
    setUnit(unit)

if __name__ == '__main__':
    if haveDoc():
        main()
    else:
        messageBox("Golden Mean.py", "Please run this script with a document already open", ICON_INFORMATION);
