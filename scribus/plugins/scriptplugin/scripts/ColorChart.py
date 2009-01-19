#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
ABOUT THIS SCRIPT:

ColorChart.py allows a user to create color charts with all the colors of a given scribus document. 
It generates a color field for each color and a description of the color, containing the color name, the CMYK values and the RGB values.

If there is a document opened in scribus, ColorChart uses this document as color source and creates a new document with the color chart.
If there is no document opened in scribus, ColorChart displays a file open dialog to allow the user to chose a scribus file to generate a colorchart of.
You will be asked to give a name for the color chart. This name will be displayed in the pages headlines.
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

####################
# IMPORTS GO HERE  #
####################

COLOR_FIELD_HEIGHT=25
#space between colorfields
HSPACE=5
VSPACE=4
#space for header and footer
HEADERSIZE = 10
FOOTERSIZE = 5
TEXT_BOX_WIDTH = 50
global pageTitle
pageTitle="COLOR CHART"

def drawHeaderFooter(pagetitle):
    """draw some info on the pages"""
    # get page size
    pageSize=scribus.getPageSize()
    pageWidth=pageSize[0]
    pageHeight=pageSize[1]
    #pageMargins
    pageMargins=scribus.getPageMargins()
    topMargin=pageMargins[0]
    leftMargin=pageMargins[1]
    rightMargin=pageMargins[2]
    bottomMargin=pageMargins[3]
    
    #create textbox and insert text for header
    textbox=scribus.createText(leftMargin, topMargin,  pageWidth-leftMargin-rightMargin, HEADERSIZE)
    #set proper font size and alignment
    scribus.setFontSize(18, textbox)
    scribus.setTextAlignment(scribus.ALIGN_CENTERED, textbox)
    #load the string into the textbox
    headerstring=pagetitle
    scribus.insertText(headerstring, 0, textbox)
    
    #create textbox and insert text for footer
    textbox=scribus.createText(leftMargin, pageHeight-bottomMargin-FOOTERSIZE,  pageWidth-leftMargin-rightMargin, FOOTERSIZE)
    #set proper font size and alignment
    scribus.setFontSize(9, textbox)
    scribus.setTextAlignment(scribus.ALIGN_LEFT, textbox)
    #load the string into the textbox
    footerstring="Created using ColorChart.py V %s script for Scribus by Sebastian Stetter - http://www.sebastianstetter.de" % str(__version__)
    scribus.insertText(footerstring, 0, textbox)

def getColorsFromDocument():
    """gets colors from opend document. if there is no document, display dialog to chose a file. returns a list[name,c,m,y,k]"""
    def getColors():
        """gets the colors and returns a list[name,c,m,y,k]"""
        colorNames=scribus.getColorNames()
        list=[]
        scribus.statusMessage("Reading Colors...")
        stepsTotal=len(colorNames)
        scribus.progressTotal(stepsTotal)
        steps=0
        for name in colorNames:
            color=scribus.getColor(name)
            listitem=[name, color[0], color[1],  color[2],  color[3]]
            list.append(listitem)
            #update progress bar
            steps=steps+1
            scribus.progressSet(steps)
        return list

    #check if we have a document - otherwise display open file dialog
    if scribus.haveDoc() == 1:
        pass
        list=getColors()
        return list
    else:
        pass
        #display file open dialog
        file=scribus.fileDialog("ColorChart by Sebastian Stetter",  'Scribus files(*.sla *.SLA *.sla.gz *.SLA.GZ)')
        #open file
        try:
            scribus.openDoc(file)
        except:
            scribus.messageBox("ColorChart by Sebastian Stetter", "could not open file")
            sys.exit()
        list=getColors()
        return list

def prepareDocument():
    """creates the new document, sets up colors """
    colorList = getColorsFromDocument()
    scribus.statusMessage("Preparing new document...")
    scribus.newDocument(scribus.PAPER_A4,  (15,15,  20, 20),  scribus.PORTRAIT, 1, scribus.UNIT_POINTS,  scribus.PAGE_1, 0, 1) 
    scribus.setUnit(scribus.UNIT_MILLIMETERS)
    #delete existing colors
    cols = scribus.getColorNames()
    for col in cols:
        scribus.deleteColor(col, "None")
    
    #create our new colors
    for color in colorList:
        cname=color[0]
        c = int(color[1])
        m = int(color[2])
        y = int(color[3])
        k = int(color[4])
        scribus.defineColor(cname,  c, m, y, k )
    
    #get the pageTitle form user and store it in PageTitle
    global pageTitle
    pageTitle=scribus.valueDialog("ColorChart by Sebastian Stetter", "Please enter document title", "Scribus COLOR CHART")
    drawHeaderFooter(pageTitle)
    
def createPage():
    """appends a new page"""
    scribus.newPage(-1) #append new page
    #new page - new header and footer
    drawHeaderFooter(pageTitle)


def rgbhex(r,g,b):
    '''convert rgb values in 0-255 style to hex string in #000000 to #ffffff style'''
    hr=hex(r)
    hr = hr.replace("0x", "")
    if len(hr)== 0:
        hr = "00"
    elif len(hr)==1:
        hr = "0"+hr
    else:
        pass
    hg=hex(g)
    hg = hg.replace("0x", "")
    if len(hg)== 0:
        hg = "00"
    elif len(hg)==1:
        hg = "0"+hg
    else:
        pass
    hb=hex(b)  
    hb = hb.replace("0x", "")
    if len(hb)== 0:
        hb = "00"
    elif len(hb)==1:
        hb = "0"+hb
    else:
        pass
    rgbstring="#"+hr+hg+hb
    rgbstring=rgbstring.upper()
    return rgbstring


def drawColor(colorname, h, v, width,  height): #h horizontal position, v vertical position
    """draws a color chart field with its caption for the given colorname at the h and v position"""
    #get cmyk values and convert them to 0 - 255 values
    color = scribus.getColor(colorname)
    c= int(round(color[0]/2.55))
    m=int(round(color[1]/2.55))
    y=int(round(color[2]/2.55))
    k=int(round(color[3]/2.55))
    #get rgb color
    rgbcolor=scribus.getColorAsRGB(colorname)
    r=rgbcolor[0]
    g=rgbcolor[1]
    b=rgbcolor[2]
    #get webcolor
    webcolor=rgbhex(r, g, b)
    #but String for Textbox together
    colorstring="%s\nC %i, M %i, Y %i, K %i, \nR %i, G %i, B %i \nRGB: %s" %(colorname, c, m, y, k, r, g, b,  webcolor)
    #draw rectangle and set colors
    rect=scribus.createRect(h, v, width,  height)
    scribus.setFillColor(colorname, rect)
    #if total amount of color is < 20 draw outline in Black for rectangle, else in same color
    if c +m+y+k < 20:
        scribus.setLineColor("Black", rect)
    else:
        scribus.setLineColor(colorname, rect)
    #create textbox and insert text
    textbox=scribus.createText(h+width+5, v, 50, height)
    #set proper font size
    scribus.setFontSize(11, textbox)
    scribus.setTextAlignment(scribus.ALIGN_LEFT, textbox)
    #load the string into the textbox
    scribus.insertText(colorstring, 0, textbox)
    


def createChart():
    """actually handles the whole chart creation process"""
    prepareDocument()
    # get page size
    pageSize=scribus.getPageSize()
    pageWidth=pageSize[0]
    pageHeight=pageSize[1]
    #pageMargins
    pageMargins=scribus.getPageMargins()
    topMargin=pageMargins[0]
    leftMargin=pageMargins[1]
    rightMargin=pageMargins[2]
    bottomMargin=pageMargins[3]
    
    
    #color field dimensions
    colorFieldWidth= pageWidth - leftMargin - rightMargin - (TEXT_BOX_WIDTH+HSPACE) #50+5 is the with of the textbox plus the space between textbox and colorfield
    
    #how much space does one field use?
    vSpaceUsedByField = COLOR_FIELD_HEIGHT+VSPACE
    
    #how much space is available per row?
    vSpaceAvailable=pageHeight-topMargin-bottomMargin-HEADERSIZE-FOOTERSIZE
    
    #counts the colorFields created for a page. reset this variable after creation of new page
    colorFieldCounter=0
    
    #get list of all colors in document
    colorList = scribus.getColorNames()
    #prepare the progressbar
    colorNumber=len(colorList)
    scribus.progressTotal(colorNumber)
    #@TODO: implement possibility to abort script (button2=scribus.BUTTON_CANCEL) buttons should return int 1 or 2
    #scribus.messageBox("ColorChart Script by Sebastian Stetter", "...going to create a chart of "+str(colorNumber)+" colors.\n This may take a while.",  button1 = scribus.BUTTON_OK)
    scribus.statusMessage("Drawing color fields...")
    stepCompleted=0
    #disable redrawing for better performance
    scribus.setRedraw(False)
    for color in colorList:
        if (vSpaceUsedByField * (colorFieldCounter+1)) <= vSpaceAvailable:
            # when there is enought space left draw a color field...
            
            #calculate Position for new colorField
            h=leftMargin
            v=topMargin + (vSpaceUsedByField * colorFieldCounter)+HEADERSIZE
            #draw the colorField
            drawColor(color, h, v,  colorFieldWidth, COLOR_FIELD_HEIGHT )
            colorFieldCounter = colorFieldCounter+1
            #update progressbar
            stepCompleted = stepCompleted+1
            scribus.progressSet(stepCompleted)
        else:
            #not enough space? create a new page!
            createPage()
            #reset the colorFieldCounter to '0' since we created a new page
            colorFieldCounter = 0
            h=leftMargin
            v=topMargin + (vSpaceUsedByField * colorFieldCounter)+HEADERSIZE
            drawColor(color, h, v,  colorFieldWidth, COLOR_FIELD_HEIGHT )
            colorFieldCounter = colorFieldCounter+1

            #update progressbar
            stepCompleted = stepCompleted+1
            scribus.progressSet(stepCompleted)
    
    #make shure pages are redrawn
    scribus.setRedraw(True)

        





def main(argv):
    """just invokes createChart() and displays a message after the chart is finished."""
    createChart()
    scribus.messageBox("ColorChart Script by Sebastian Stetter", "Your chart has been created, but not saved, yet!\nThanks for using ColorChart and Scribus!")

    


def main_wrapper(argv):
    """The main_wrapper() function disables redrawing, sets a sensible generic
    status bar message, and optionally sets up the progress bar. It then runs
    the main() function. Once everything finishes it cleans up after the main()
    function, making sure everything is sane before the script terminates."""
    try:
        scribus.statusMessage("Creating color chart...")
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
