#!/usr/bin/env python
# -*- coding: utf-8 -*-
#This script unflips all the items so their horizontal and vertical flip flag is set to False

#****************************************************************************
#COPYRIGHT:
#Copyright (C) 2007 Jeremy Brown
#
#
#LICENSE:
#This program is free software; you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation; either version 2 of the License, or
#(at your option) any later version.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program; if not, write to the Free Software
#Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#****************************************************************************

# Craig Bradney, Scribus Team
# 10/3/08: Added to Scribus 1.3.3.12svn distribution "as was" from Scribus wiki for bug #6826, script is GPLd

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