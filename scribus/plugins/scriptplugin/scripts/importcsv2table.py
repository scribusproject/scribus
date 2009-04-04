#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
ABOUT THIS SCRIPT:

Import CSV data files as tables into Scribus

1. Create any frame of any size on your page but positioned 
where you want the table to be located (upper left corner)

2. Make sure it is selected

3. Execute this script:

You will be prompted first for the width of the left column in mm,
then the right column in mm, then height of all cells, and finally
for a csv filename

4. The data from the csv file will be imported and a table of
textboxes will be drawn on the page.

LIMITATIONS:

1. You are limited to two-column CSV data in your file.

2. In Scribus versions 1.3.5svn, when the script ends, you cannot
adjust text, colors, and line features for a group, whereas in 1.3.3.x,
all of these can be done without ungrouping.

HINTS:

Postgresql:
You can easily create a CSV file with a Postgresql database. From Postgresql,
toggle unaligned output with the '\a' switch, then activate a comma as
a separator with '\f ,' (without apostrophes). Send output to a file
with '\o myfile.csv', then query your database.

Sqlite3:
You can use "sqlite3 -csv" in the command line or ".mode csv" in sqlite's
interactive shell.

############################

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

Author: Sebastian Stetter

Modifications: Gregory Pittman

please report bugs to: scribusscript@sebastianstetter.de
"""

from __future__ import division
import sys

try:
    # Please do not use 'from scribus import *' . If you must use a 'from import',
    # Do so _after_ the 'import scribus' and only import the names you need, such
    # as commonly used constants.
    import scribus
except ImportError,err:
    print "This Python script is written for the Scribus scripting interface."
    print "It can only be run from within Scribus."
    sys.exit(1)

#########################
# YOUR IMPORTS GO HERE  #
#########################
import csv

#get information about the area where the bale should be drawed
def getPosition():
    if scribus.selectionCount() == 1:
        areaname = scribus.getSelectedObject()
        position= scribus.getPosition(areaname)
        vpos = position[1]
        hpos = position[0]
        scribus.deleteObject(areaname)
        return vpos, hpos
        
    else: 
        scribus.messageBox("csv2table", "please select ONE Object to mark the drawing area for the table")
        sys.exit()

#get the cvs data
def getCSVdata():
    """opens a csv file, reads it in and returns a 2 dimensional list with the data"""
    csvfile = scribus.fileDialog("csv2table :: open file", "*.csv")
    if csvfile != "":
        try:
            reader = csv.reader(file(csvfile))
            datalist=[]
            for row in reader:
                rowlist=[]
                for col in row:
                    rowlist.append(col)
                datalist.append(rowlist)
            return datalist
        except Exception,  e:
            scribus.messageBox("csv2table", "Could not open file %s"%e)
    else:
        sys.exit

def getDataInformation(list):
    """takes a 2 dimensional list object and returns the numbers of rows and cols"""
    datainfo = dict()
    datainfo["rowcount"]=len(list)
    datainfo["colcount"]= len(list[0])
    return datainfo

def cellsize(areainfo, datainfo):
    """"takes the area and data info and calculates the prper cell size"""
    csize=dict()
    csize["v"]= areainfo["vsize"] / datainfo["rowcount"]
    csize["h"]= areainfo["hsize"] / datainfo["colcount"]
    return csize
    
def main(argv):
    """This is a documentation string. Write a description of what your code
    does here. You should generally put documentation strings ("docstrings")
    on all your Python functions."""
    #########################
    #  YOUR CODE GOES HERE  #
    #########################
    userdim=scribus.getUnit() #get unit and change it to mm
    scribus.setUnit(scribus.UNIT_MILLIMETERS)
    cellwidthleft = 0
    cellwidthright = 0
    cellHeight = 0
    pos = getPosition()
    while cellwidthleft <= 0:
      cellwidthL = scribus.valueDialog('Left Cell Width','How wide (mm) do you wish left cells to be?','30.0')
      cellwidthleft = float(cellwidthL)
    while cellwidthright <= 0:
      cellwidthR = scribus.valueDialog('Right Cell Width','How wide (mm) do you wish right cells to be?','30.0')
      cellwidthright = float(cellwidthR)
    while cellHeight <= 0:
      cellheight = scribus.valueDialog('Cell Height','How tall (mm) do you wish cells to be?','10.0')
      cellHeight = float(cellheight)
    data = getCSVdata()
    di= getDataInformation(data)
    hposition=pos[1]
    vposition=pos[0]
    
    objectlist=[] # here we keep a record of all the created textboxes so we can group them later
    i=0
    scribus.progressTotal(len(data))
    scribus.setRedraw(False)
    for row in data:
        c=0
        for cell in row:
            cell = cell.strip()
            cellsize=cellwidthleft
            if c == 1: cellsize=cellwidthright
            textbox=scribus.createText(hposition, vposition, cellsize, cellHeight) #create a textbox
            objectlist.append(textbox)
            scribus.insertText(cell,0, textbox)#insert the text into the textbox
            hposition=hposition+cellwidthleft #move the position for the next cell
            c=1
        vposition=vposition+cellHeight #set vertical position for next row
        hposition=pos[1] #reset vertical position for next row
        i=i+1
        scribus.progressSet(i)
    
    scribus.groupObjects(objectlist)
    scribus.progressReset()
    scribus.setUnit(userdim) # reset unit to previous value
    scribus.docChanged(True)
    scribus.statusMessage("Done")
    scribus.setRedraw(True)

def main_wrapper(argv):
    """The main_wrapper() function disables redrawing, sets a sensible generic
    status bar message, and optionally sets up the progress bar. It then runs
    the main() function. Once everything finishes it cleans up after the main()
    function, making sure everything is sane before the script terminates."""
    try:
        scribus.statusMessage("Importing .csv table...")
        scribus.progressReset()
        main(argv)
    finally:
        # Exit neatly even if the script terminated with an exception,
        # so we leave the progress bar and status bar blank and make sure
        # drawing is enabled.
        if scribus.haveDoc():
            scribus.setRedraw(True)
        scribus.statusMessage("")
        scribus.progressReset()

# This code detects if the script is being run as a script, or imported as a module.
# It only runs main() if being run as a script. This permits you to import your script
# and control it manually for debugging.
if __name__ == '__main__':
    main_wrapper(sys.argv)
