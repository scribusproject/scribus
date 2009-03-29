#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""

(C) 2007 Jeremy Brown

Craig Bradney, Scribus Team 10/3/08: Added to Scribus 1.3.3.12svn distribution 
"as was" from Scribus wiki for bug #6826.

This program is free software; you can redistribute it and/or modify
it under the terms of the  GPL, v2 (GNU General Public License as published by
the Free Software Foundation, version 2 of the License), or any later version.
See the Scribus Copyright page in the Help Browser for further informaton 
about GPL, v2.

REQUIREMENTS

Must be run from Scribus. You should have a document open with items selected. 
Script fails with no output if these are not met.

SYNOPSIS

This script unflips all the items by switching their horizontal and vertical flip 
flags to False.  One might typically use this after you have grouped a number of 
items on the page, then flipped the group so the positions are a mirror image of
the original.  Unfortunately this flips the content as well, so this script 
flips the content back.

USAGE

Select the items you wish to have their content flipped to their original state. If you
also Group them, the script will run more reliably. Run the script.

"""

from scribus import *
if haveDoc():
    nbrSelected = selectionCount()
    objList = []
    for i in range(nbrSelected):
        objList.append(getSelectedObject(i))
    for i in range(nbrSelected):
        try:
            obj = objList[i]
            setProperty(obj, "m_ImageIsFlippedH", False)
            setProperty(obj, "m_ImageIsFlippedV", False)
            moveObject(1, 0, obj)
            moveObject(-1, 0, obj)
            docChanged(1)
            setRedraw(True)
        except:
            nothing = "nothing"
