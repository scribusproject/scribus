#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
ABOUT THIS SCRIPT:

Import Colors from a CSV file to Scribus

csv2color.py allows a user to import colors from a given csv file into a scribus document. 
The file must be a text file with comma seperated values in the following format:

"colorname", c,m,y,k 

There must be a document opend in scribus where the colors can be defined in. 
If the csv contanins one or more color names that already exist in the document, the colors will be imported with a `*` as prefix.

This script is especially helpfull if you want to use CMYK color representations of color systems like HKS, Pantone or RAL in Scribus. Lots of such CMYK translation tables can be found on the Web. 
One can easily copy such a table into a text file, save it in the obove described format and import it into a scribus document.

Use color2csv to export the colors from a scribus document into a csv file.

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

__version__=1.1


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

PREFIX="*"

def checkValue(c, m, y, k):
    """returns true if the cmyk values are between 0 and 255"""
    MINVAL=0
    MAXVAL=255
    valueOk=True
    for val in c, m, y, k:
        if val >=MINVAL and val <=255:
            pass
        else:
            valueOk=False
        
    return valueOk

def getColorsFromCsv(filename):
    """get colors from csv file and return a list with name and cmyk 255 values"""
    csvreader=csv.reader(file(filename))

    csvcolors=[]
    i=0
    for row in csvreader:
        name=row[0]
        name=name.strip()
        c=int(row[1] )* 2.55
        c=int(c)
        m=int(row[2] )* 2.55
        m=int(m)
        y=int(row[3] )* 2.55
        y=int(y)
        k=int(row[4] )* 2.55
        k=int(k)        
        if checkValue(c, m, y, k) ==False:
            scribus.messageBox("csv2color", "At least one CMYK value in your csv file is not correct \n(must be between 0 and 100)\nAborting script - nothing imported.",  icon=scribus.ICON_WARNING)
            sys.exit()
        else:
            pass
        color=(name, c, m, y, k)
        csvcolors.append(color)
        i=i+1
    return csvcolors

def getColorDict():
    """get the colors that already exist from the opened Document and return a dictionary"""
    scribus.statusMessage("Reading existing colors...")
    colornames = scribus.getColorNames()
    scribus.progressTotal(len(colornames))
    i=0
    colordict={}
    for name in colornames:
        colordict[name]=None
        i=i+1
        scribus.progressSet(i)
    return colordict #we can ask this dict if the color already exists
        
def importColors(colorlist):
        """check if colors exists an import"""
        colordict=getColorDict()
        scribus.statusMessage("Defining new colors...")
        scribus.progressTotal(len(colorlist))
        i=0
        for color in colorlist:
            name=color[0]
            c=color[1]
            m=color[2]
            y=color[3]
            k=color[4]
            while colordict.has_key(name):# check if color already exists - then add PREFIX to name
                name = PREFIX+name
            
            scribus.defineColor(name, c, m, y, k)
            i=i+1
            scribus.progressSet(i)

def main(argv):
    """Main method for importing colors."""
    if not scribus.haveDoc(): #do we have a doc?
        scribus.messageBox("csv2color", "No document to import colors \n Please open one, first.")
        sys.exit()
    else:
        filename=scribus.fileDialog("csv2color",  "CSV files(*.csv *.CSV *.txt *.TXT)")
        while os.path.isdir(filename):
            filename=scribus.fileDialog("csv2color",  "CSV files(*.csv *.CSV *.txt *.TXT)") #proper filename?
        else:
            try:
                colorlist=getColorsFromCsv(filename)
                messagestring = "You are going to import %i colors \n This may take a while" % len(colorlist)
                answer = scribus.messageBox("csv2color", messagestring, button1=scribus.BUTTON_OK,  button2=scribus.BUTTON_CANCEL)
                if answer != scribus.BUTTON_OK:
                    sys.exit()
                else:
                    importColors(colorlist)
                    scribus.docChanged(True)
                    scribus.messageBox("csv2color", "Colors imported! \n Thank you for using csv2color and Scribus!")
            except:
                scribus.messageBox("csv2color", "Could not import file!", icon=scribus.ICON_WARNING)
                sys.exit()
            


def main_wrapper(argv):
    """The main_wrapper() function disables redrawing, sets a sensible generic
    status bar message, and optionally sets up the progress bar. It then runs
    the main() function. Once everything finishes it cleans up after the main()
    function, making sure everything is sane before the script terminates."""
    try:
        #scribus.statusMessage("Running script...")
        scribus.progressReset()
        main(argv)
    finally:
        # Exit neatly even if the script terminated with an exception,
        # so we leave the progress bar and status bar blank and make sure
        # drawing is enabled.
        if scribus.haveDoc() > 0:
            scribus.setRedraw(True)
        scribus.statusMessage("")
        scribus.progressReset()

# This code detects if the script is being run as a script, or imported as a module.
# It only runs main() if being run as a script. This permits you to import your script
# and control it manually for debugging.
if __name__ == '__main__':
    main_wrapper(sys.argv)
