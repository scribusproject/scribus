#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
ABOUT THIS SCRIPT:

Export Scribus Colors to CSV

color2csv.py allows a user to export  the colors of a given scribus document in a csv file. The file will be a text file with comma seperated values in the following format:
"colorname", c,m,y,k 

If there is a document opened in scribus, color2csv uses this document as color source.
If there is no document opened in scribus, color2csv displays a file open dialog to allow the user to chose a scribus file to get the colors from.

Use csv2color to import the colors into a scribus document from a csv file.

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
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

Author: Sebastian Stetter

please report bugs to: scribusscript@sebastianstetter.de
"""
from __future__ import division
import sys

__version__=1.0

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
import os
#define some variables

def getColorsFromDoc():
    """returns a list ("name", c,y,m,k)
    get all the colors of that doc. """
    #get a list of al the colornames
    scribus.statusMessage("Reading Colors...")

    try:
        colorlist = scribus.getColorNames()
        scribus.progressTotal(len(colorlist))
        i=0
        colordata=[]
        for color in colorlist:
            colorvalues=scribus.getColor(color)
            c=int(colorvalues[0]/2.55) #convert values from 0-255 to 0-100
            m=int(colorvalues[1]/2.55)
            y=int(colorvalues[2]/2.55)
            k=int(colorvalues[3]/2.55)
            name=color.strip() #eliminate leading and tailing whitespace
            cd = [name,c ,m,y,k] 
            colordata.append(cd)
            i=i+1
            scribus.progressSet(i)
        
        return colordata
    except:
        scribus.messageBox("color2csv", "Can not retrieve colors - There is no Document", icon=scribus.ICON_WARNING)
        sys.exit()
        

def writeColorCsvFile(filename, colorlist):
    """writes all the colors to a csv file"""
    scribus.statusMessage("Writing colors to csv file...")
    scribus.progressTotal(len(colorlist))
    i=0
    try:
        csvwriter=csv.writer(file(filename, "w"), quoting=csv.QUOTE_NONNUMERIC)
        for line in colorlist:
            csvwriter.writerow(line)
            i=i+1
            scribus.progressSet(i)
    except:
        scribus.messageBox("color2csv", "Could not write file!", icon=scribus.ICON_WARNING)
        sys.exit()

def main(argv):
    """Main method - here we check if we have a doc - else we open one. we get all the colors and write them to a csv file."""
    if scribus.haveDoc() > 0:  #DOC OPEN
    #get colors, get filename, write stuff
        cols = getColorsFromDoc()
        filename = scribus.fileDialog("color2csv: Save csv color file", defaultname="colors.csv",  issave=True ,  haspreview=False)
        
        #@TODO: optimize path checking
        if filename !="":
            if os.path.exists(filename): #make shure we don't accidentaly overwrite existing files
                answer= scribus.messageBox("color2csv", "File already exists! \n do you want to overwrite it?",  icon=scribus.ICON_WARNING,  button1=scribus.BUTTON_YES,  button2=scribus.BUTTON_ABORT)
                if answer == scribus.BUTTON_YES:
                    writeColorCsvFile(filename,  cols)
                else:
                    sys.exit()
            else:
                writeColorCsvFile(filename,  cols)
        else:
            sys.exit()
    else: # NO DOC OPEN - open one!
        scribusfile = scribus.fileDialog("color2csv: Open scribus file",  "Scribus files(*.sla *.SLA *.sla.gz *.SLA.GZ)")
        if scribusfile !="":
            try:
                scribus.openDoc(scribusfile)
            except:
                scribus.messageBox("color2csv", "Could not open file!")
                sys.exit()
            #get colors, get filename, write stuff
            cols = getColorsFromDoc()
            filename = scribus.fileDialog("color2csv: Save csv color file", defaultname="colors.csv",  issave=True )
            writeColorCsvFile(filename,  cols)
        else:
            sys.exit()

def main_wrapper(argv):
    """The main_wrapper() function disables redrawing, sets a sensible generic
    status bar message, and optionally sets up the progress bar. It then runs
    the main() function. Once everything finishes it cleans up after the main()
    function, making sure everything is sane before the script terminates."""
    try:
        #scribus.statusMessage("Running script...")
        #scribus.progressReset()
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
