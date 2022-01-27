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
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#
# ****************************************************************************

"""
******************************************************************************

DESCRIPTION & USAGE:
This script needs Tkinter. It will create a GUI with an alphabetical list
of fonts using the names as they will be shown in Scribus. User can select
one or more fonts and create an example sheet(s) to print or create a PDF
from. It is heavily commented to make it easier for the user to adjust it
for his / her own needs.

Note: this version needs read/write access to .scribus directory in users
home. You will also need Python Imaging Library (PIL) installed.
If your system does not meet these requirements then change showPreviewPanel
to a value of 0. This will disable the new preview features.

******************************************************************************

First release    : 30/12/2003
This release     : v0.8.1tk (released 4th Dec 2005)
Copyright        : (C) 2003 - 2005 Steve Callcott
Latest releases
and support      : www.firstwish.co.uk/sjc/scribus/index.php
Maintainer       : Steve Callcott 2003 - 2005
Email            : stotte@firstwish.co.uk

For revision history see the ChangeLog file.
Bugs and future plans are listed in the TODO file.
See NEWS for new features since last version.

WHATS NEW v0.8.2tk:
A one liner change by Jean Ghali to line #734 to add the extra parameter missing.
See: http://bugs.scribus.net/view.php?id=4377

WHATS NEW v0.8.1tk:
After reloading users saved preferences the status bar was not showing
correct calculations.
Changed text in settings menu.

WHATS NEW v0.8tk Final:
Cleaned up the checkbox labels and layout.

WHATS NEW v0.8tk Preview 3:
Calls the new Scribus zoomDocument() function to make the completed font
sample document fit in Scribus window.

Grey out "Start page number count from first page" when "Print TOC" is
not checked as without a table of contents the first page would always 
start on the same page number making this option irrelevant.

WHATS NEW v0.8tk Preview 2:
Replaced the newDoc() with newDocument(). Have not put any fallback code
for use with earlier Scribus versions.

When using double sided option we now make use of Scribus ability to display
pages side by side as default. You may need to zoom out to view the
complete document width.

WHATS NEW v0.8tk Preview 1:
Rearanged the initialisation. If no fonts are found for the Table of
Contents, page numbers and font sample labels, the script shows a
message box listing the problem and a possible solution as well as a message
to the console.

A Scribus messagebox alerts the user if Tkinter is not found. Previously
this message was only printed to the console.

Now able to do a dummy run to calculate and report the amount of samples
that will fit on a page. This enables the script to correctly calculate
how many sheets will be required. Previously it was always assumed that
there would be 3 sample blocks on a sheet. This is now not always the case.

Added menu. Also added "about" and "settings" dialogs.

Sample rows can be selected or unselected to save on paper. The settings are
automatically saved when changed and can be set as user defaults.

User can choose to have page numbers count from first page of the toc instead
of the first page of samples. This can be helpful if wanting to quickly look
up a font in the toc and then using the Scribus page navigator dialog to go to
the actual page on the screen to view it without printing it out.

Added initial support for a sample paragraph. The sample paragraph defaults
to "off" due to the amount of space it uses on the page.

Some widgets read their defaults from a config dictionary.

Many code cleanups. Classes used for settings storage have been replaced with
dictionaries to make it easier for users to customise.

******************************************************************************
"""

import sys
import os
import cPickle


showPreviewPanel = 1 # change to 0 to permanently hide the preview
TEMP_PATH = os.path.join(os.path.expanduser('~'), '.scribus')
CONFIG_PATH = os.path.join(os.path.expanduser('~'), '.scribus/fontsampler')


try:
    import scribus
except ImportError,err:
    print 'This Python script is written for the Scribus scripting interface.'
    print 'It can only be run from within Scribus.'
    sys.exit(1)


try:
    from Tkinter import *
except ImportError,err:
    print 'This script will not work without Tkinter'
    scribus.messageBox('Error','This script will not work without Tkinter\nPlease install and try again',
                    scribus.ICON_WARNING)
    sys.exit(1)


if not os.path.exists(CONFIG_PATH):
    try:
        print 'Attempting to creating configuration file directory...'
        os.mkdir(CONFIG_PATH)
        print 'Success, now testing for write access of new directory...'
        if os.access(CONFIG_PATH, os.W_OK):
            print 'Write access ok.'
        else:
            print 'Error, unable to write to .scribus/fontsampler directory.'
    except:
        CONFIG_PATH = ''
        print 'Failed to make configuration file directory,'
        print 'do you have a .scribus directory in your home directory?'
        print 'font sampler will not be able to save your preferences'


try:
    from PIL import Image
except ImportError,err:
    print 'You need to install Python Imaging Library (PIL).'
    print 'If using gentoo then you need to emerge /dev-python/imaging'
    print 'If using an RPM based linux distribution then you add python-imaging or similar.'
    print 'Script will continue without the font preview panel.'
    showPreviewPanel = 0


try:
    from PIL import ImageTk
except ImportError,err:
    print 'Module ImageTk not found, font preview disabled'
    showPreviewPanel = 0


if showPreviewPanel:
    if not os.path.exists(TEMP_PATH):
        print '.scribus folder not found, disabling font preview panel'
        showPreviewPanel = 0
    if not os.access(TEMP_PATH, os.W_OK):
        print 'Unable to write to .scribus folder, disabling font preview panel'
        showPreviewPanel = 0


# A few globals for use later...
gSamplePic = None
gPreviewId = None

#*************************************************************************

WINDOW_TITLE = 'Font Sampler v0.8.1tk - Steve Callcott'
SUPPORT_PAGE = 'www.firstwish.co.uk/sjc/scribus/index.php'

fontsListFixed = (
    'Luxi Mono Regular',
    'Nimbus Mono L Regular',
    'Courier 10 Pitch Regular',
    'Courier New Regular',
    'Courier Regular',
    'Andale Mono Regular',
    'Larabiefont Regular'
)

fontsListProportional = (
    'Nimbus Sans L Regular',
    'Luxi Sans Regular',
    'Bitstream Vera Sans',
    'Helvetica',
    'Arial Regular'
)

defaultPrefs = {
    'wantDoubleSided': 0,
    'paperSize':'A4',           # currently PAPER_LETTER or PAPER_A4
    'wantTOC': 1,
    'wantBindingOffset': 0,
    'wantPageNumbers': 1,
    'wantPageOneOnFirst': 0,
    'wantAlphabet' : 1,
    'want6Point' : 1,
    'want8Point' : 1,
    'want10Point' : 1,
    'want12Point' : 1,
    'want16Point' : 1,
    'want20Point' : 1,
    'want32Point' : 1,
    'wantParagraph' : 0         # Uses a lot of space so default is off
}

userPrefs = {}

geometriesList = [
    {
        'paperName' : 'A4',
        'paperSize' : scribus.PAPER_A4,
        'paperWidth' : 595,
        'paperHeight' : 842,
        'paperTopMargin' : 60,
        'paperBottomMargin' : 50,
        'paperLeftMargin' : 50,
        'paperRightMargin' : 50,
        'paperBinding' : 16,
        'tocRowsPerPage' : 57,
        'paperPageNumVertOffset' : 16
    },
    {
        'paperName' : 'US Letter',
        'paperSize' : scribus.PAPER_LETTER,
        'paperWidth' : 612,
        'paperHeight' : 792,
        'paperTopMargin' : 27,
        'paperBottomMargin' : 45,
        'paperLeftMargin' : 50,
        'paperRightMargin' : 50,
        'paperBinding' : 18,
        'tocRowsPerPage' : 56,
        'paperPageNumVertOffset' : 16
    }
]

# define our data dictionary and some of the data...
dD = {
    'tocHeaderTitle' : 'Table of Contents',
    'tocCharsInRow' : 75,
    'previewpanelFontHeight' : 28,
    'previewpanelSampleText' : 'Woven silk pyjamas exchanged for blue quartz'
}

samplesHeader = {
    'fontSize' : 16,
    'lineSpace' : 15,
    'textHeight' : 23
}

# Use \xBC etc to insert Hex ascii chars into the sample strings below.
sampleAlphabet = {
    'fontSize' : 10.5,
    'lineSpace' : 12,
    'textString' : 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#?$*&',
    'textHeight' : 18
}

sample6Point = {
    'fontSize' : 6,
    'lineSpace' : 6,
    'textString' : 'This line is in 6 point',
    'textHeight' : 13
}

sample8Point = {
    'fontSize' : 8,
    'lineSpace' : 8,
    'textString' : 'This line is in 8 point',
    'textHeight' : 16
}

sample10Point = {
    'fontSize' : 10,
    'lineSpace' : 11,
    'textString' : 'This line is in 10 point',
    'textHeight' : 19
}

sample12Point = {
    'fontSize' : 12,
    'lineSpace' : 11,
    'textString' : 'This line is in 12 point',
    'textHeight' : 21
}

sample16Point = {
    'fontSize' : 16,
    'lineSpace' : 13,
    'textString' : 'This line is in 16 point',
    'textHeight' : 26
}

sample20Point = {
    'fontSize' : 20,
    'lineSpace' : 16,
    'textString' : 'This line is in 20 point',
    'textHeight' : 31
}

sample32Point = {
    'fontSize' : 32,
    'lineSpace' : 29,
    'textString' : 'This line is in 32 point',
    'textHeight' : 49
}

sampleParagraph = {
    'fontSize' : 9,
    'lineSpace' : 10.8,
    'textHeight' : 175
}

sampleParagraphText = 'Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Ut a sapien. \
Aliquam aliquet purus molestie dolor. Integer quis eros ut erat posuere dictum. \
Curabitur dignissim. Integer orci. Fusce vulputate lacus at ipsum. Quisque in libero \
nec mi laoreet volutpat. Aliquam eros pede, scelerisque quis, tristique cursus, \
placerat convallis, velit. Nam condimentum. Nulla ut mauris. Curabitur adipiscing, \
mauris non dictum aliquam, arcu risus dapibus diam, nec sollicitudin quam erat quis \
ligula. Aenean massa nulla, volutpat eu, accumsan et, fringilla eget, odio. \
Nulla placerat porta justo. Nulla vitae turpis.\n\nPraesent lacus.Lorem ipsum dolor sit \
amet, consectetuer adipiscing elit. Pellentesque habitant morbi tristique senectus \
et netus et malesuada fames ac turpis egestas. Quisque vel erat eget diam \
consectetuer iaculis. Cras ante velit, suscipit et, porta tempus, dignissim quis, \
magna. Vivamus viverra, turpis nec rhoncus ultricies, diam turpis eleifend nisl, a \
eleifend ante felis ac sapien. Integer bibendum. Suspendisse in mi non neque \
bibendum convallis. Suspendisse potenti. Sed sit amet purus at felis adipiscing \
aliquam. Vivamus et nisl sit amet mauris aliquet molestie. Integer tortor massa, \
aliquam a, lacinia nonummy, sagittis nec, eros.'

#*************************************************************************

def set_font_fixed(fontList):
    """Find a matching font for the Table of Contents."""
    availableFonts = scribus.getFontNames()
    found = 0
    for f in fontList:
        if found:
            break
        for i in availableFonts:
            if not found:
                if f == i:
                    return f
                    found = 1
                    break
    if not found:
        errorList = ''
        for j in fontList:
            errorList = errorList + j + '\n'
        errorMessage ='No suitable fixed width font found.\nPlease install at least one of these fixed width fonts:\n'+errorList
        print errorMessage
        raise Exception(errorMessage)


def set_font_proportional(fontList):
    """Find a matching font for the page numbers and font names above samples."""
    availableFonts = scribus.getFontNames()
    found = 0
    for p in fontList:
        if found:
            break
        for i in availableFonts:
            if not found:
                if p == i:
                    return p
                    found = 1
                    break
    if not found:
        errorList = ''
        for j in fontList:
            errorList = errorList + j + '\n'
        errorMessage = 'No suitable proportional font found.\nPlease install at least one of these proportional fonts:\n'+errorList
        print errorMessage
        raise Exception(errorMessage)


def save_user_conf(path):
    """Save the data to the save file on the path specified by CONFIG_PATH.

    Note initialisation unsets the CONFIG_PATH if it failed to verify or create"""
    if not path == '':
        try:
            file = open(os.path.join(path,'fontsampler.conf'), 'w')
            data = {
                'a' : defaultPrefs,
                'b' : userPrefs
            }
            cPickle.dump(data, file)
            file.close()
        except:
            print 'failed to save data'


def restore_user_conf(path):
    """Restore the data from the save file on the path specified by CONFIG_PATH."""
    try:
        file = open(os.path.join(path,'fontsampler.conf'), 'r')
        data = cPickle.load(file)
        file.close()
        defaultPrefs.update(data['a'])
        userPrefs.update(data['b'])
    except:
        userPrefs.update(defaultPrefs)
        print 'failed to load saved data so using default values defined in the script'


def set_page_geometry(dD, geometriesList, paperSize, wantBindingOffset):
    """This is the experimental replacement paper size setting function.

    Each paper size and other associated data are stored in a dictionary.
    The dictionaries are stored in a list. We copy appropriate dictionary
    and custom calculations into a work dictionary for use.
    The advantage of this is its easier to add new paper definitions.
    Returns a new dictionary, use .update to merge in new values into dD.
    """
    try:
        result={}
        for i in geometriesList:
            if i['paperName'] == paperSize:
                dD.update(i)

        result['paperLeftMarginOdd'] = dD['paperLeftMargin'] + \
                                       dD['paperBinding']
        result['paperRightMarginEven'] = dD['paperRightMargin'] + \
                                         dD['paperBinding']
        result['paperTextHeight'] = dD['paperHeight'] - \
                                    dD['paperTopMargin'] - \
                                    dD['paperBottomMargin']
        result['paperMargins'] =  dD['paperLeftMargin'],dD['paperRightMargin'],dD['paperTopMargin'],dD['paperBottomMargin']

        # if we are adding a binding offset to the margins then we will have less width for our text...
        if wantBindingOffset:
            result['paperTextWidth'] = dD['paperWidth'] - \
                                       dD['paperLeftMargin'] - \
                                       dD['paperRightMargin'] - \
                                       dD['paperBinding'] - \
                                       2
        else:
            result['paperTextWidth'] = dD['paperWidth'] - \
                                       dD['paperLeftMargin'] - \
                                       dD['paperRightMargin'] - \
                                       2
        return result
    except:
        errorMessage = 'set_page_geometry() failure: %s' % sys.exc_info()[1]
        print errorMessage


def set_odd_even(pageNum):
    """ Sets the left margin position.

    Checks the number passed to it and sets left margin accordingly.
    Call once after each new page is created.
    Returns 1 if even and 0 if odd page.
    """
    if pageNum % 2 == 0:
        isEvenPage = 1                                          # Even side
    else:
        isEvenPage = 0                                          # Odd side

    if userPrefs['wantBindingOffset']:
        if isEvenPage and userPrefs['wantDoubleSided']:         # Even (when double sided)
            dD['paperLeftSide'] = dD['paperLeftMargin'] + 1
        else:                                                   # Odd side
            dD['paperLeftSide'] = dD['paperLeftMarginOdd'] + 1
    else:                                                       # No binding
        dD['paperLeftSide'] = dD['paperLeftMargin'] + 1
    return isEvenPage


def draw_sample_row(font, fontSize, lineSpace, textString, x, y, w, h, getSizeOnly):
    """Creates one row of samples or a header for the top of the block.

    Called once by draw_sample_block() to create a block label then as many times
    as required to create each actual sample found in the list of dictionaries
    containing each samples definition.
    """
    if not getSizeOnly:
        f = scribus.createText(x, y, w, h)
        scribus.insertText(textString, 0, f)
        scribus.setFont(font, f)
        scribus.setFontSize(fontSize, f)
        scribus.setLineSpacing(lineSpace, f)
        scribus.setTextAlignment(0, f)
    return y + h + 1


def draw_sample_block(fontName, x, y, w, getSizeOnly):
    """Drawing of a complete sample block starts from here.

    Iterates through each sample declared in the "samples" tuple. Places one
    complete block using the font specified in fontname.
    Note top line on page is drawn outside of this function. This ensures ease
    of returning same height of every sample block. Line could have been drawn
    at top inside this function and page finalised with line at bottom instead.
    If getSizeOnly is true then returns the overall height of the entire text
    block but without actually placing it.
    """
    startPos = y
    # Note there are 2 points of space before horizontal line at bottom of block.
    # This 2 points will not appear at top of page so need to add it.

    # set space below horizontal line to the top of the text block
    y = y + 4

    # (note there is one extra point inserted by addSampleRow() for each row generated)...

    # first need a header...
    y = draw_sample_row(dD['bookstylePropFont'], samplesHeader['fontSize'], samplesHeader['lineSpace'], fontName, x, y, w, samplesHeader['textHeight'], getSizeOnly)

    if userPrefs['wantAlphabet']:
        y = draw_sample_row(fontName, sampleAlphabet['fontSize'], sampleAlphabet['lineSpace'], sampleAlphabet['textString'], x, y, w, sampleAlphabet['textHeight'], getSizeOnly)

    if userPrefs['want6Point']:
        y = draw_sample_row(fontName, sample6Point['fontSize'], sample6Point['lineSpace'], sample6Point['textString'], x, y, w, sample6Point['textHeight'], getSizeOnly)

    if userPrefs['want8Point']:
        y = draw_sample_row(fontName, sample8Point['fontSize'], sample8Point['lineSpace'], sample8Point['textString'], x, y, w, sample8Point['textHeight'], getSizeOnly)

    if userPrefs['want10Point']:
        y = draw_sample_row(fontName, sample10Point['fontSize'], sample10Point['lineSpace'], sample10Point['textString'], x, y, w, sample10Point['textHeight'], getSizeOnly)

    if userPrefs['want12Point']:
        y = draw_sample_row(fontName, sample12Point['fontSize'], sample12Point['lineSpace'], sample12Point['textString'], x, y, w, sample12Point['textHeight'], getSizeOnly)

    if userPrefs['want16Point']:
        y = draw_sample_row(fontName, sample16Point['fontSize'], sample16Point['lineSpace'], sample16Point['textString'], x, y, w, sample16Point['textHeight'], getSizeOnly)

    if userPrefs['want20Point']:
        y = draw_sample_row(fontName, sample20Point['fontSize'], sample20Point['lineSpace'], sample20Point['textString'], x, y, w, sample20Point['textHeight'], getSizeOnly)

    if userPrefs['want32Point']:
        y = draw_sample_row(fontName, sample32Point['fontSize'], sample32Point['lineSpace'], sample32Point['textString'], x, y, w, sample32Point['textHeight'], getSizeOnly)

    if userPrefs['wantParagraph']:
        y = draw_sample_row(fontName, sampleParagraph['fontSize'], sampleParagraph['lineSpace'], sampleParagraphText, x, y, w, sampleParagraph['textHeight'], getSizeOnly)

    y = y + 1   # one extra point of space above bottom Horiz. line

    lineHeight = draw_horiz_line(y, x, w + x, getSizeOnly)
    y = y + lineHeight

    return y - startPos


def insert_toc_row(fontName, pageNum, yPos, frame):
    """Called once for each content line to be drawn in the text frame."""
    dotLine = ""
    dotQuant = dD['tocCharsInRow'] - len(fontName) - len(str(pageNum)) + 1
    for i in range(dotQuant):
        dotLine = dotLine + '.'
    oneLine = fontName + dotLine + str(pageNum) + "\n"
    scribus.insertText(oneLine, yPos, frame)
    yPos = yPos + len(oneLine) + 0
    return yPos


def build_toc_page_template():
    """Inserts empty toc template into the currently selected page."""
    # first put a header on the empty page...
    textstring = dD['tocHeaderTitle']
    yPos = dD['paperTopMargin'] + 1
    header = scribus.createText(dD['paperLeftSide'], yPos, dD['paperTextWidth'], 35)
    scribus.insertText(textstring, 0, header)
    scribus.setFont(dD['bookstylePropFont'], header)
    scribus.setFontSize(24, header)
    scribus.setTextAlignment(1, header)
    # now create a text frame for the table of contents...
    yPos = yPos + 36
    body = scribus.createText(dD['paperLeftSide'], yPos, dD['paperTextWidth'], dD['paperHeight'] - yPos - dD['paperBottomMargin'] - 1)
    scribus.setFont(dD['bookstyleFixedFont'], body)
    scribus.setFontSize(10, body)
    scribus.setLineSpacing(12, body)
    return body


def build_toc(tocList):
    """Creates all the Table of Contents pages.

    Calls tocPageFramesBuild() to write the header and empty frame for the
    toc rows each time a new page is added.
    Then calls tocRowAdd() to add each line to the toc frame. Creates new page
    each time it completes last row on page.
    """
    rowCount = 0
    yPos = 0
    tocPageNum = 1
    tocPageCount = 1

    scribus.newPage(tocPageNum)
    isEvenPage = set_odd_even(tocPageNum)
    body = build_toc_page_template()             # create frames for new empty page
    if isEvenPage == 0:
        scribus.setTextAlignment(2, body)
    else:
        scribus.setTextAlignment(0, body)
    for i in tocList:
        if rowCount == dD['tocRowsPerPage']: # Need to build a new TOC page (count is from zero, not one)
            tocPageNum = tocPageNum + 1
            scribus.newPage(tocPageNum)
            isEvenPage = set_odd_even(tocPageNum)
            body = build_toc_page_template()
            if not isEvenPage:
                scribus.setTextAlignment(2, body)
            else:
                scribus.setTextAlignment(0, body)
            rowCount = 0
            yPos = 0
            tocPageCount = tocPageCount + 1
        yPos = insert_toc_row(i[0], i[1], yPos, body)
        rowCount = rowCount + 1
    if userPrefs['wantDoubleSided']:
        if tocPageCount % 2 != 0:           # Odd page
            tocPageNum = tocPageNum + 1
            scribus.newPage(tocPageNum)     # Add an extra page if odd number


def add_page_num(pageNum):
    yPos = dD['paperHeight'] - \
           dD['paperBottomMargin'] - \
           dD['paperPageNumVertOffset']
    footer = scribus.createText(dD['paperLeftSide'], yPos, dD['paperTextWidth'], 15)
    scribus.insertText('%s' % pageNum, 0, footer)
    scribus.setFont(dD['bookstylePropFont'], footer)
    scribus.setFontSize(9, footer)
    scribus.setTextAlignment(1, footer)
    scribus.setLineSpacing(10, footer)


def create_empty_samplepage(pageNum, getSizeOnly):
    """Creates a new page and increments page number by one.

    Note getSizeOnly is now evaluated. Will still generate page number increment
    but will not actually create the new page or place the number on the page."""
    if not getSizeOnly:
        scribus.newPage(-1)
    pageNum = pageNum + 1
    set_odd_even(pageNum)
    if not getSizeOnly:
        if userPrefs['wantPageNumbers']:
            add_page_num(pageNum)
    return pageNum


def draw_horiz_line(yPos, xStart, xEnd, getSizeOnly):
    """Draws a line and returns the height.

    If getSizeOnly is set then returns the height it would have
    used but without actually creating a line.
    """
    lineWidth = 1
    if not getSizeOnly:
        newLine = scribus.createLine(xStart, yPos, xEnd, yPos)
        scribus.setLineWidth(lineWidth, newLine)
    return lineWidth


def draw_selection(fontList, getSizeOnly):
    """Draws the sample blocks onto the Scribus canvas.

    Measure one font sample block including any horizontal lines and extra
    vertical spaces.
    Get the amount of vertical space available for the text area between the
    top line and top of page number area.
    Use these two values to calculate how many complete sample blocks will fit
    in the space available. This is the "pageBlocks"
    Note we always draw the top horizontal line before placing the blocks. This
    is taken into account when calculating the available text area.
    Next, if "getSizeOnly" is false we create a page then create the sample
    blocks while incrementing a counter until it matches the "pageBlocks".
    Reset the counter and create new page. We keep going until we have processed
    all the fonts in the selection list.
    We update the Scribus progress bar as we create each font sample block.
    The returned result is used to update some values in the status bar.
    """
    progress = 1
    scribus.progressReset()
    scribus.progressTotal(len(fontList))
    tocList = []
    pageNum = 1
    blockCounter = 0
    counter = 0
    facingPages = scribus.NOFACINGPAGES
    
    # Just get blocks per page value...
    set_odd_even(pageNum)
    lineHeight = 1 # include the one point of space below top margin
    lineHeight = lineHeight + draw_horiz_line(0, dD['paperLeftSide'], dD['paperLeftSide'] + dD['paperTextWidth'], 1)
    usuableArea = dD['paperHeight'] - \
                  dD['paperTopMargin'] - \
                  lineHeight - \
                  dD['paperBottomMargin'] - \
                  dD['paperPageNumVertOffset']

    blockHeight = draw_sample_block(fontList[0], dD['paperLeftSide'], 0, dD['paperTextWidth'], 1)
    pageBlocks = int(usuableArea / blockHeight)
    #print blockHeight
    #print "Usuable area %s points high" % usuableArea
    #print "Used space on page is %s points high" % (blockHeight * pageBlocks)

    if not getSizeOnly:
        # not a dummy run so start by setting up page numbering...
        if userPrefs['wantPageOneOnFirst'] and userPrefs['wantTOC']:
            tocPageCount = divmod(len(fontList), dD['tocRowsPerPage'])
            pageNum = pageNum + tocPageCount[0]
            if tocPageCount[1] != 0:
                # (adding more to page number as not whole number)
                pageNum = pageNum + 1
            if userPrefs['wantDoubleSided']:
                oddEvenTest = divmod(pageNum, 2)
                if oddEvenTest[1] == 0:
                    # (adding extra one to start number as odd amount)
                    pageNum = pageNum + 1
        if userPrefs['wantDoubleSided']:
            facingPages = scribus.FACINGPAGES
        # now create a new document with empty page and start building...
        scribus.newDocument(dD['paperSize'], dD['paperMargins'], scribus.PORTRAIT, 1, scribus.UNIT_POINTS, facingPages, scribus.FIRSTPAGERIGHT, 1)
        scribus.zoomDocument(-100)
        # A new doc gives us a new page by default so set it up first...
        set_odd_even(pageNum)
        yPos = dD['paperTopMargin'] + 1
        lineHeight = draw_horiz_line(yPos, dD['paperLeftSide'], dD['paperLeftSide'] + dD['paperTextWidth'], getSizeOnly)
        yPos = yPos + lineHeight
        if userPrefs['wantPageNumbers']:
            add_page_num(pageNum)
        for i in fontList:
            # Now place the actual sample block but create a new page if needed...
            if counter == pageBlocks:
                pageNum = create_empty_samplepage(pageNum, getSizeOnly)
                yPos = dD['paperTopMargin'] + 1
                lineHeight = draw_horiz_line(yPos, dD['paperLeftSide'], dD['paperLeftSide'] + dD['paperTextWidth'], getSizeOnly)
                yPos = yPos + lineHeight
                counter = 0
            blockHeight = draw_sample_block(i, dD['paperLeftSide'], yPos, dD['paperTextWidth'], getSizeOnly)
            yPos = yPos + blockHeight
            # and also increment the Scribus progress bar...
            scribus.progressSet(progress)
            progress = progress + 1
            # Add current font to TOC...
            tocList.append([i, pageNum])
            counter = counter + 1
        if userPrefs['wantTOC']:
            # Insert table of contents - (before page numbering)...
            build_toc(tocList)
        scribus.gotoPage(1)
    return pageBlocks


def preview_font(app, fontName):
    """Gets the named font and puts a sample in the preview panel.

    Pick up the temp sample qpixmap file and display it in a canvas object
    The temp file is put in the users ".scribus" folder and cleaned up on exit.
    We create samplePic as a global as a workaround because the reference count
    does not increase when we add the image to the canvas. Therefore python
    garbage collection removes our image before we have even displayed it.
    Note app.previewPanel is the actual canvas.
    """
    global gSamplePic
    global gPreviewId
    scribus.renderFont(fontName, os.path.join(TEMP_PATH,'temp079r.bmp'),dD['previewpanelSampleText'],dD['previewpanelFontHeight'])
    try:
        tempPic = Image.open(os.path.join(TEMP_PATH,'temp079r.bmp'))
        tempPic.save(os.path.join(TEMP_PATH,'temp079r.jpeg'),format='JPEG')
        tempImage = Image.open(os.path.join(TEMP_PATH,'temp079r.jpeg'))
        imgDimen = tempPic.getbbox()
        gSamplePic = ImageTk.PhotoImage(tempImage)
        # To center the image use "Half display height minus half the image height"
        # preview panel is allegedly 56 (60 less a 2 pixel border top and bottom)
        # need to be lower than that to look correct visually...
        topEdge = (32 - (imgDimen[3] / 2))
        gPreviewId = app.previewPanel.create_image(5, topEdge, anchor=NW, image=gSamplePic)
        os.remove(os.path.join(TEMP_PATH,'temp079r.bmp'))
        os.remove(os.path.join(TEMP_PATH,'temp079r.jpeg'))
    except IOError:
        gSamplePic = None
        gPreviewId = app.previewPanel.create_image(0, 0, anchor=NW, image=gSamplePic)
    return


class AboutDialog(Toplevel):

    def __init__(self, parent):
        Toplevel.__init__(self, parent)
        self.transient(parent)
        self.title('About')
        self.parent = parent
        self.result = None
        self.resizable(0, 0)

        infoLabel = Label(self, text=WINDOW_TITLE+'\nSupport page at %s' % SUPPORT_PAGE)
        infoLabel.pack(padx=5, pady=5)
        # now the frame for contents...
        contentFrame = Frame(self)
        self.btnOk = Button(contentFrame, text='OK', command=self.ok, default=ACTIVE)
        self.btnOk.pack(side=LEFT, padx=5, pady=5)
        contentFrame.pack()
        self.bind('<Return>', self.ok)
        self.grab_set()
        self.protocol('WM_DELETE_WINDOW', self.ok)
        self.initial_focus = self.btnOk
        self.wait_window(self)

    def ok(self, event=None):
        self.withdraw()
        self.update_idletasks()
        self.parent.focus_set()
        self.destroy()


class ConfigurationDialog(Toplevel):

    def __init__(self, parent):
        Toplevel.__init__(self, parent)
        self.transient(parent)
        self.title('Configuration')
        self.parent = parent
        self.result = None
        self.resizable(0, 0)

        # Create outer frame...
        self.topFrame = Frame(self, bd=1, relief=FLAT)
        self.topFrame.grid(row=0, column=0, padx=5, pady=5)

        self.paperSizeLabel = Label(self.topFrame, text='Sample Rows:')
        self.paperSizeLabel.grid(row=0, column=0, sticky=W)

        # This frame holds each sample selector...
        self.sampleSelectFrame = Frame(self.topFrame, bd=1, relief=RIDGE)
        self.sampleSelectFrame.grid(row=1, column=0, padx=0, pady=2)

        # now create the sample selector widgets for the frame...
        self.__wantAlphabet = IntVar()
        self.btnWantAlphabet = Checkbutton(self.sampleSelectFrame, text='want alphabet row', variable=self.__wantAlphabet, offvalue=0, onvalue=1, command=self.__sampleSelectionClick)
        self.btnWantAlphabet.grid(row=0, column=0, padx=10, pady=0, sticky=W)
        if userPrefs['wantAlphabet']:
            self.btnWantAlphabet.select()

        self.__want6Point = IntVar()
        self.btnWant6Point = Checkbutton(self.sampleSelectFrame, text='want 6 point row', variable=self.__want6Point, offvalue=0, onvalue=1, command=self.__sampleSelectionClick)
        self.btnWant6Point.grid(row=1, column=0, padx=10, pady=0, sticky=W)
        if userPrefs['want6Point']:
            self.btnWant6Point.select()

        self.__want8Point = IntVar()
        self.btnWant8Point = Checkbutton(self.sampleSelectFrame, text='want 8 point row', variable=self.__want8Point, offvalue=0, onvalue=1, command=self.__sampleSelectionClick)
        self.btnWant8Point.grid(row=2, column=0, padx=10, pady=0, sticky=W)
        if userPrefs['want8Point']:
            self.btnWant8Point.select()

        self.__want10Point = IntVar()
        self.btnWant10Point = Checkbutton(self.sampleSelectFrame, text='want 10 point row', variable=self.__want10Point, offvalue=0, onvalue=1, command=self.__sampleSelectionClick)
        self.btnWant10Point.grid(row=3, column=0, padx=10, pady=0, sticky=W)
        if userPrefs['want10Point']:
            self.btnWant10Point.select()

        self.__want12Point = IntVar()
        self.btnWant12Point = Checkbutton(self.sampleSelectFrame, text='want 12 point row', variable=self.__want12Point, offvalue=0, onvalue=1, command=self.__sampleSelectionClick)
        self.btnWant12Point.grid(row=4, column=0, padx=10, pady=0, sticky=W)
        if userPrefs['want12Point']:
            self.btnWant12Point.select()

        self.__want16Point = IntVar()
        self.btnWant16Point = Checkbutton(self.sampleSelectFrame, text='want 16 point row', variable=self.__want16Point, offvalue=0, onvalue=1, command=self.__sampleSelectionClick)
        self.btnWant16Point.grid(row=5, column=0, padx=10, pady=0, sticky=W)
        if userPrefs['want16Point']:
            self.btnWant16Point.select()

        self.__want20Point = IntVar()
        self.btnWant20Point = Checkbutton(self.sampleSelectFrame, text='want 20 point row', variable=self.__want20Point, offvalue=0, onvalue=1, command=self.__sampleSelectionClick)
        self.btnWant20Point.grid(row=6, column=0, padx=10, pady=0, sticky=W)
        if userPrefs['want20Point']:
            self.btnWant20Point.select()

        self.__want32Point = IntVar()
        self.btnWant32Point = Checkbutton(self.sampleSelectFrame, text='want 32 point row', variable=self.__want32Point, offvalue=0, onvalue=1, command=self.__sampleSelectionClick)
        self.btnWant32Point.grid(row=7, column=0, padx=10, pady=0, sticky=W)
        if userPrefs['want32Point']:
            self.btnWant32Point.select()

        self.__wantParagraph = IntVar()
        self.btnParagraphSelect = Checkbutton(self.sampleSelectFrame, text='want sample paragraph', variable=self.__wantParagraph, offvalue=0, onvalue=1, command=self.__sampleSelectionClick)
        self.btnParagraphSelect.grid(row=8, column=0, padx=10, pady=0, sticky=W)
        if userPrefs['wantParagraph']:
            self.btnParagraphSelect.select()

        self.paperSizeLabel = Label(self.topFrame, text='Paper Sizes:')
        self.paperSizeLabel.grid(row=2, column=0, sticky=W)

        self.paperSizeFrame = Frame(self.topFrame, bd=1, relief=RIDGE)
        self.paperSizeFrame.grid(row=3, column=0, padx=0, pady=2, sticky=W)

        self.__paper = StringVar()
        self.a4papersizeSelect = Radiobutton(self.paperSizeFrame, text='A4', variable=self.__paper, value='A4', command=self.__paperSelectionClick)
        self.a4papersizeSelect.grid(row=1, column=0, padx=10, sticky=W)
        self.uspapersizeSelect = Radiobutton(self.paperSizeFrame, text='US Letter', variable=self.__paper, value='US Letter', command=self.__paperSelectionClick)
        self.uspapersizeSelect.grid(row=2, column=0, padx=10, sticky=W)

        # set to match prefs...
        if userPrefs['paperSize'] == 'US Letter':
            self.uspapersizeSelect.select()
        if userPrefs['paperSize'] == 'A4':
            self.a4papersizeSelect.select()

        self.btnFrame = Frame(self.topFrame)
        self.btnFrame.grid(row=4, column=0, padx=10, pady=2)
        self.btnOk = Button(self.btnFrame, text='OK', command=self.ok)
        self.btnOk.grid(row=2, column=0, pady=5)
        self.bind('<Return>', self.ok)
        self.grab_set()
        self.initial_focus = self.btnOk
        self.wait_window(self)


    def __sampleSelectionClick(self):
        """Get and store all the selections.
        
        Just assigns the lot at once. Not worth being picky and only
        assigning values that have changed since last time.
        """
        userPrefs['wantAlphabet'] = self.__wantAlphabet.get()
        userPrefs['want6Point'] = self.__want6Point.get()
        userPrefs['want8Point'] = self.__want8Point.get()
        userPrefs['want10Point'] = self.__want10Point.get()
        userPrefs['want12Point'] = self.__want12Point.get()
        userPrefs['want16Point'] = self.__want16Point.get()
        userPrefs['want20Point'] = self.__want20Point.get()
        userPrefs['want32Point'] = self.__want32Point.get()
        userPrefs['wantParagraph'] = self.__wantParagraph.get()
        self.parent.statusbarUpdate()

    def __paperSelectionClick(self):
        userPrefs['paperSize'] = self.__paper.get()
        self.parent.statusbarUpdate()

    def ok(self, event=None):
        dD.update(set_page_geometry(dD, geometriesList, userPrefs['paperSize'], userPrefs['wantBindingOffset']))
        self.withdraw()
        self.update_idletasks()
        self.parent.focus_set()
        self.destroy()


class Application(Frame):

    def __init__(self, master = None):
        Frame.__init__(self, master)

        self.grid()

        # Remove maximise button and resize. Not good to allow resizable window
        # because the listboxes are fixed width...
        self.master.resizable(0, 0)

        # build the menu...
        menubar = Menu(self)
        settingsmenu = Menu(menubar, tearoff=0)
        settingsmenu.add_command(label='Configuration', command=self.__configurationDlgShow)
        settingsmenu.add_separator()
        settingsmenu.add_command(label='Save current settings', command=self.__saveCurrentSettingsAsDefaults)
        settingsmenu.add_command(label='Load saved settings', command=self.__restoreCurrentSettingsFromDefault)

        menubar.add_cascade(label='Settings', menu=settingsmenu)
        helpmenu = Menu(menubar, tearoff=0)
        helpmenu.add_command(label='About', command=self.__aboutDlgShow)
        menubar.add_cascade(label='Help', menu=helpmenu)
        # display menu...
        self.master.config(menu=menubar)

        # now start adding our widgets starting with the top frame...
        self.listbox_frame = Frame(self)
        self.listbox_frame.grid(row=0, column=0, sticky=EW)

        # left hand listbox assembly
        self.leftListbox_frame = Frame(self.listbox_frame, borderwidth=1, relief=SUNKEN)
        self.leftListbox_frame.grid(row=1, column=0)

        self.leftLabel = Label(self.listbox_frame, text='Available Fonts')
        self.leftLabel.grid(row=0, column=0, sticky=NS)

        self.yScroll1 = Scrollbar(self.leftListbox_frame, orient=VERTICAL)
        self.yScroll1.grid(row=0, column=1, sticky=NS)
        self.xScroll1 = Scrollbar(self.leftListbox_frame, orient=HORIZONTAL)
        self.xScroll1.grid(row=1, column=0, sticky=EW)

        self.listbox1 = Listbox(self.leftListbox_frame,
            xscrollcommand=self.xScroll1.set,
            yscrollcommand=self.yScroll1.set,
            selectmode=EXTENDED,
            height=20, width=40)
        self.listbox1.grid(row=0, column=0, sticky=NSEW)
        self.xScroll1['command'] = self.listbox1.xview
        self.yScroll1['command'] = self.listbox1.yview

        def __listbox1KeyRelease(event):
            """Check if an Up or Down key has been pressed and released and
            if so the preview panel is refreshed. If the keys are held down
            the file system slows the scroll. Need a timer here to delay
            updates."""
            if (event.keysym == 'Down' or event.keysym == 'Up'):
                __listbox1DoLogicCallback(self)

        def __listbox1SingleClick(event):
            """Call this first when listbox1 is clicked with mouse to put focus
            into the listbox. Note we call when mouse click is released, not pressed,
            due to the fact that the listbox does not change the selection until the button
            is released."""
            self.listbox1.focus_set()
            __listbox1DoLogicCallback(self)
        self.listbox1.bind('<ButtonRelease-1>', __listbox1SingleClick)

        def __listbox1DoLogicCallback(event):
            """Decides if current selection should be previewed.

            Start by counting items in selection list and if equal to one then
            show selected font, ignoring if more or less than one. Then also
            set up buttons logic depending on selection. We bind the FocusIn
            to this too so button logic and preview gets updated when focus
            enters the listbox.
            """
            # note we are not making use of "self.listbox1.get(ACTIVE)" due to
            # it not getting the real active name. Always one selection behind
            # even though we are doing all this in the ButtonRelease event.
            # Have made a change here. If more than one font name is selected
            # then we just empty the preview.
            names = self.listbox1.curselection()
            if len(names) == 1:
                selectedFont = self.listbox1.get(names[0])
                self.__curSelectedItem(selectedFont)
            else:
                try:
                    app.previewPanel.delete(previewId)
                except:
                    pass
            #else:
                #selectedFont = self.listbox1.get(ACTIVE)
                #print selectedFont  # for testing
                #if selectedFont != "":
                    #self.__curSelectedItem(selectedFont)

            # Now do the button logic...
            self.listbox2.selection_clear(0,END)
            self.__setUpDownActive(0, 0)    # Force a disable if in other box
            if self.listbox1.size() > 0:
                self.__setSelButtonsActive(0, 1)
            else:
                self.__setSelButtonsActive(0, 0)

        self.listbox1.bind('<FocusIn>', __listbox1DoLogicCallback)
        self.listbox1.bind('<Any-KeyRelease>', __listbox1KeyRelease)

        def __listbox1DoubleClickCallback(event):
            """The single click event will fire also when left listbox
            is double clicked but we are detecting the single click button up event."""
            self.__listSelectionToRight()

        self.listbox1.bind('<Double-Button-1>', __listbox1DoubleClickCallback)

        # middle button frame assembly
        self.midbutton_frame = Frame(self.listbox_frame)
        self.midbutton_frame.grid(row=0, rowspan=2, column=1, sticky=NSEW)

        self.rsingleButton = Button(self.midbutton_frame, state='disabled', text='>', command=self.__rsingleButtonClick)
        self.rsingleButton.grid(row=0, column=0, padx=5, pady=5, sticky=EW)
        self.rdoubleButton = Button(self.midbutton_frame, text='>>', command=self.__rdoubleButtonClick)
        self.rdoubleButton.grid(row=1, column=0, padx=5, pady=5, sticky=EW)

        self.itemupButton = Button(self.midbutton_frame, state='disabled', text='Up', command=self.__itemupButtonClick)
        self.itemupButton.grid(row=2, column=0, padx=5, pady=5, sticky=EW)
        self.itemdownButton = Button(self.midbutton_frame, state='disabled', text='Down', command=self.__itemdownButtonClick)
        self.itemdownButton.grid(row=3, column=0, padx=5, pady=5, sticky=EW)

        self.lsingleButton = Button(self.midbutton_frame, state='disabled', text='<', command=self.__lsingleButtonClick)
        self.lsingleButton.grid(row=4, column=0, padx=5, pady=5, sticky=EW)
        self.ldoubleButton = Button(self.midbutton_frame, state='disabled', text='<<', command=self.__ldoubleButtonClick)
        self.ldoubleButton.grid(row=5, column=0, padx=5, pady=5, sticky=EW)

        # Right hand listbox assembly
        self.rightListbox_frame = Frame(self.listbox_frame, borderwidth=1, relief=SUNKEN)
        self.rightListbox_frame.grid(row=1, column=2)

        self.rightLabel = Label(self.listbox_frame, text='Selected Fonts')
        self.rightLabel.grid(row=0, column=2, sticky=NS)

        self.yScroll2 = Scrollbar(self.rightListbox_frame, orient=VERTICAL)
        self.yScroll2.grid(row=0, column=1, sticky=NS)
        self.xScroll2 = Scrollbar(self.rightListbox_frame, orient=HORIZONTAL)
        self.xScroll2.grid(row=1, column=0, sticky=EW)

        self.listbox2 = Listbox(self.rightListbox_frame,
            xscrollcommand=self.xScroll2.set,
            yscrollcommand=self.yScroll2.set,
            selectmode=EXTENDED,
            height=20, width=40)
        self.listbox2.grid(row=0, column=0, sticky=NSEW)
        self.xScroll2['command'] = self.listbox2.xview
        self.yScroll2['command'] = self.listbox2.yview

        def __listbox2SingleClick(event):
            """Similar to __listbox1SingleClick()."""
            self.listbox2.focus_set()
            __listbox2DoLogicCallback(self)
        self.listbox2.bind('<ButtonRelease-1>', __listbox2SingleClick)

        def __listbox2KeyRelease(event):
            if (event.keysym == 'Down' or event.keysym == 'Up'):
                __listbox2DoLogicCallback(self)

        def __listbox2DoLogicCallback(event):
            """Similar to __listbox1DoLogicCallback()."""
            names = self.listbox2.curselection()
            if len(names) == 1:
                selectedFont = self.listbox2.get(names[0])
                self.__curSelectedItem(selectedFont)
            else:
                try:
                    app.previewPanel.delete(previewId)
                except:
                    pass

            # Now do the button logic...
            self.listbox1.selection_clear(0,END)
            self.__testUpDownState()
            if self.listbox2.size() > 0:
                self.__setSelButtonsActive(1, 0)
            else:
                self.__setSelButtonsActive(0, 0)
        self.listbox2.bind('<FocusIn>', __listbox2DoLogicCallback)
        self.listbox2.bind('<Any-KeyRelease>', __listbox2KeyRelease)

        def __listbox2DoubleClickCallback(event):
            """Similar to __listbox1DoubleClickCallback()."""
            self.__listSelectionToLeft()
        self.listbox2.bind('<Double-Button-1>', __listbox2DoubleClickCallback)

        # now draw the bottom font preview frame if required...
        if showPreviewPanel:
            self.preview_frame = Frame(self)
            self.preview_frame.grid(row=1, column=0, sticky=EW)
            self.previewPanel = Canvas(self.preview_frame, height=60, bg='white', bd=2, relief=SUNKEN)
            self.previewPanel.pack(fill=X)

        # now draw the bottom controls frame...
        self.controls_frame = Frame(self)
        self.controls_frame.grid(row=2, column=0, sticky=EW)

        # create a container...
        self.button_frame1 = Frame(self.controls_frame, bd=1, relief=RIDGE)
        self.button_frame1.grid(row=0, column=0, padx=10, pady=2)
        # create and add page number selection button...
        self.__wantPageNum = IntVar()
        self.pagenumSelect = Checkbutton(self.button_frame1, text='Print page numbers', variable=self.__wantPageNum, offvalue=0, onvalue=1, command=self.__pageNumberSelectButtonClick)
        self.pagenumSelect.grid(row=0, column=0, padx=0, sticky=W)

        # create a container...
        self.button_frame2 = Frame(self.controls_frame, bd=1, relief=RIDGE)
        self.button_frame2.grid(row=0, column=1, padx=10, pady=2)
        # create and add the TOC selector...
        self.__wantToc = IntVar()
        self.tocSelect = Checkbutton(self.button_frame2, text='Print table of contents', variable=self.__wantToc, offvalue=0, onvalue=1, command=self.__tocSelectButtonClick)
        self.tocSelect.grid(row=0, column=0, padx=0, sticky=W)
        # create and add page one on first selector...
        self.__wantPageOneOnFirst = IntVar()
        self.btnPageOneOnFirst = Checkbutton(self.button_frame2, text='Page count includes TOC', variable=self.__wantPageOneOnFirst, offvalue=0, onvalue=1, command=self.__wantPageOneOnFirstClick)
        self.btnPageOneOnFirst.grid(row=1, column=0, padx=0, sticky=W)

        # create a container...
        self.button_frame3 = Frame(self.controls_frame, bd=1, relief=RIDGE)
        self.button_frame3.grid(row=0, column=2, padx=10, pady=2)
        # create and add the binding offset...
        self.__wantBindingOffset = IntVar()
        self.bindingoffsetSelect = Checkbutton(self.button_frame3, text='Extra offset for binding', variable=self.__wantBindingOffset, offvalue=0, onvalue=1, command=self.__bindingoffsetSelectButtonClick)
        self.bindingoffsetSelect.grid(row=0, column=0, sticky=W)
        # create and add the double sided selection buttons...
        self.__wantDoubleSided = IntVar()
        self.doublesidedSelect = Checkbutton(self.button_frame3, text='Double sided pages', variable=self.__wantDoubleSided, offvalue=0, onvalue=1, command=self.__doubleSidedSelectButtonClick)
        self.doublesidedSelect.grid(row=1, column=0, rowspan=2, sticky=W)

        # now the ok and cancel buttons...
        self.cancelButton = Button(self.controls_frame, text='Cancel', command=self.__cancelButtonClick)
        self.cancelButton.grid(row=0, column=3, padx=5)
        self.okButton = Button(self.controls_frame, text='OK', state='disabled', command=self.__okButtonClick)
        self.okButton.grid(row=0, column=4, padx=5)

        # now create the bottom status bar frame and contents...
        self.status_frame = Frame(self)
        self.status_frame.grid(row=3, column=0, sticky=E+W)
        self.status0 = Label(self.status_frame, bd=1, relief=SUNKEN, anchor=W)
        self.status0.pack(side=LEFT)
        self.status1 = Label(self.status_frame, bd=1, relief=SUNKEN, anchor=W)
        self.status1.pack(side=LEFT)
        self.status2 = Label(self.status_frame, bd=1, relief=SUNKEN, anchor=W)
        self.status2.pack(side=LEFT)
        self.status3 = Label(self.status_frame, bd=1, relief=SUNKEN, anchor=W)
        self.status3.pack(side=LEFT)
        self.statusPaperSize = Label(self.status_frame, bd=1, relief=SUNKEN, anchor=W)
        self.statusPaperSize.pack(fill=X)

    def statusbarUpdate(self):
        """Draws the status bar contents.

        Note "draw_selection()" does a dummy run to count the amount of sample
        blocks on a sheet.
        TODO: Statusbar setting and recalculation should be separated. Just recalc
        and refresh panels as required instead of all of them each time.
        """
        available = self.listbox1.size()
        selected = self.listbox2.size()
        size = FloatType(selected)
        blocksPerSheet = draw_selection(scribus.getFontNames(), 1)
        value = size / blocksPerSheet
        pages = IntType(value)                  # Get whole part of number
        value = value - pages                   # Remove whole number part
        if value > 0:                           # Test remainder
            pages = pages + 1                   # Had remainder so add a page
        self.status0['text'] = 'Fonts available: %s   ' % (available + selected)
        self.status1['text'] = 'Fonts selected: %s   ' % selected
        self.status2['text'] = 'Sheets required: %s   ' % pages
        self.status3['text'] = 'Fonts per sheet: %s   ' % blocksPerSheet
        self.statusPaperSize['text'] = 'Paper size: %s   ' % userPrefs['paperSize']

    def __listSelectionToRight(self):
        toMoveRight = ListType(self.listbox1.curselection())
        self.listbox1.selection_clear(0,END)
        toMoveRight.reverse()   # reverse list so we delete from bottom of listbox first
        tempList = []
        for i in toMoveRight:
            tempList.insert(0,self.listbox1.get(i)) # gets the actual strings (reverse again)
            self.listbox1.delete(i)
        for j in tempList:
            self.listbox2.insert(END, j)
        self.__setButtonsState()
        self.__setSelButtonsActive(0, 0)
        self.statusbarUpdate()

    def __listSelectionToLeft(self):
        toMoveLeft = ListType(self.listbox2.curselection())
        toMoveLeft.reverse()
        self.listbox2.selection_clear(0,END)
        for i in toMoveLeft:
            self.listbox1.insert(END, self.listbox2.get(i)) # Insert it at the end
            self.listbox2.delete(i)
        fontList = ListType(self.listbox1.get(0, END))      # Copy contents to a list type
        self.listbox1.delete(0, END)                        # Remove all contents
        fontList.sort()                                     # Use sort method of list
        for j in fontList:
            self.listbox1.insert(END, j)                    # Replace with sorted version
        self.__setButtonsState()
        self.__setSelButtonsActive(0, 0)
        self.statusbarUpdate()

    def __listAllToRight(self):
        fontList = self.listbox1.get(0, END)    # Get each font name into a list
        for i in fontList:
            self.listbox2.insert(END, i)        # Copy each one
        self.listbox1.delete(0, END)            # All done so empty the left listbox
        self.__setButtonsState()
        self.__setSelButtonsActive(0, 0)
        self.statusbarUpdate()

    def __listAllToLeft(self):
        """Moves all selected fonts back to the left hand pane.

        Note we just clear both panes then reload the left listbox in correct
        order from scratch as this is probably quicker than moving each
        item individually.
        """
        self.listbox1.delete(0, END)
        fontList = scribus.getFontNames()
        fontList.sort()
        for i in fontList:
            self.listbox1.insert(END, i)
        self.listbox2.delete(0, END)
        self.__setButtonsState()
        self.__setSelButtonsActive(0, 0)
        self.statusbarUpdate()

    def __setSelButtonsActive(self, selToRight, selToLeft):
        # The "selection" buttons are the ones with ">"  and "<"  on them
        if selToRight == 1:
            self.lsingleButton['state'] = NORMAL
        else:
            self.lsingleButton['state'] = DISABLED
        if selToLeft == 1:
            self.rsingleButton['state'] = NORMAL
        else:
            self.rsingleButton['state'] = DISABLED

    def __setAllButtonsActive(self, allToRight, allToLeft):
        # The "all" buttons are the ones with ">>"  and "<<"  on them
        if allToRight == 1:
            self.rdoubleButton['state'] = NORMAL
        else:
            self.rdoubleButton['state'] = DISABLED
        if allToLeft == 1:
            self.ldoubleButton['state'] = NORMAL
        else:
            self.ldoubleButton['state'] = DISABLED

    def __setButtonsState(self):
        if self.listbox2.size() > 0 and self.listbox1.size() > 0:
            self.__setAllButtonsActive(1, 1)
            self.okButton['state'] = NORMAL
        elif self.listbox2.size() == 0:
            self.__setAllButtonsActive(1, 0)
            self.okButton['state'] = DISABLED
        elif self.listbox1.size() == 0:
            self.__setAllButtonsActive(0, 1)
            self.okButton['state'] = NORMAL

    def __itemUp(self):
        """Test if one item is selected then move it up one position."""
        selection = self.listbox2.curselection()
        if len(selection) == 1:
            indexId = IntType(selection[0]) # Get the first (only) item as integer type
            if indexId > 0:
                fontString = self.listbox2.get(indexId)
                self.listbox2.delete(indexId)
                newPos = indexId - 1
                self.listbox2.selection_clear(0, END)
                self.listbox2.insert(newPos, fontString)
                self.listbox2.see(newPos - 10)  # Scrolls listbox automatically into view if req.
                self.listbox2.selection_set(newPos)
                self.listbox2.activate(newPos)  # make focus follow selection
                self.__testUpDownState()  # note tests only after an item has been moved

    def __itemDown(self):
        """Test if one item is selected then move it down one position."""
        limit = self.listbox2.size()
        selection = self.listbox2.curselection()
        if len(selection) == 1:
            indexId = IntType(selection[0])
            if indexId < limit - 1:
                fontString = self.listbox2.get(indexId)
                self.listbox2.delete(indexId)
                newPos = indexId + 1
                self.listbox2.selection_clear(0, END)
                self.listbox2.insert(newPos, fontString)
                self.listbox2.see(newPos + 10)
                self.listbox2.selection_set(newPos)
                self.listbox2.activate(newPos)  # make focus follow selection
                self.__testUpDownState()

    def __setUpDownActive(self, up, down):
        """Just sets the buttons active or inactive.

        See testUpDown() for the actual evaluation
        """
        if up == 1:
            self.itemupButton['state'] = NORMAL
        else:
            self.itemupButton['state'] = DISABLED
        if down == 1:
            self.itemdownButton['state'] = NORMAL
        else:
            self.itemdownButton['state'] = DISABLED

    def __testUpDownState(self):
        """Only enable the up and down buttons when just a single item is selected.

        Enable should be applied to up, down or both depending on its
        position in the listbox. At all other times disable both.
        """
        # Get a count of how many items are currently selected...
        selection = list(self.listbox2.curselection())
        tcount = 0
        for sel in selection:
            tcount = tcount + 1

        position = 0
        if tcount == 1:
            position = IntType(selection[0])

        # If one selected and there is more than one item in the listbox then ok...
        if tcount == 1 and self.listbox2.size() > 1:
            # Now test the position of the selected line...
            if position > 0 and position < self.listbox2.size() - 1:    # Both
                self.__setUpDownActive(1, 1)
            # If not one less or lesser from the bottom (listbox.count-1?) then gray the down button.
            elif position == self.listbox2.size() - 1:                  # Up only
                self.__setUpDownActive(1, 0)
            # If not one or more from the top then gray up button.
            elif position == 0:                                         # Down only
                self.__setUpDownActive(0, 1)
        else:
            self.__setUpDownActive(0, 0)

    def __curSelectedItem(self, selectedFont):
        """Send the selected font to the preview function if preview available."""
        if showPreviewPanel:
            preview_font(self, selectedFont)

    # create the button events...
    def __rsingleButtonClick(self):
        self.__listSelectionToRight()

    def __rdoubleButtonClick(self):
        self.__listAllToRight()

    def __lsingleButtonClick(self):
        self.__listSelectionToLeft()
        self.__testUpDownState()

    def __ldoubleButtonClick(self):
        self.__listAllToLeft()
        self.__testUpDownState()

    def __itemupButtonClick(self):
        self.__itemUp()

    def __itemdownButtonClick(self):
        self.__itemDown()

    def __tocSelectButtonClick(self):
        userPrefs['wantTOC'] = self.__wantToc.get()
        if userPrefs['wantTOC']:
            self.btnPageOneOnFirst['state'] = NORMAL
        else:
            self.btnPageOneOnFirst['state'] = DISABLED

    def __pageNumberSelectButtonClick(self):
        userPrefs['wantPageNumbers'] = self.__wantPageNum.get()

    def __bindingoffsetSelectButtonClick(self):
        userPrefs['wantBindingOffset'] = self.__wantBindingOffset.get()
        dD.update(set_page_geometry(dD, geometriesList, userPrefs['paperSize'], userPrefs['wantBindingOffset']))

    def __doubleSidedSelectButtonClick(self):
        userPrefs['wantDoubleSided'] = self.__wantDoubleSided.get()

    def __wantPageOneOnFirstClick(self):
        userPrefs['wantPageOneOnFirst'] = self.__wantPageOneOnFirst.get()

    def __cancelButtonClick(self):
        """We exit the app here if user presses cancel."""
        self.master.destroy()

    def __okButtonClick(self):
        """User presses ok, so lets create the pages."""
        save_user_conf(CONFIG_PATH)
        draw_selection(self.listbox2.get(0, END), 0)
        self.master.destroy()

    def __configurationDlgShow(self):
        """Opens the configuration dialog where user can set up the options"""
        configs = ConfigurationDialog(self)
        self.statusbarUpdate()

    def __saveCurrentSettingsAsDefaults(self):
        """Stores current settings as defaults."""
        defaultPrefs.update(userPrefs)
        save_user_conf(CONFIG_PATH)

    def __restoreCurrentSettingsFromDefault(self):
        """Restores current settings from defaults."""
        userPrefs.update(defaultPrefs)
        self.initialiseWidgets()
        self.statusbarUpdate()

    def initialiseWidgets(self):
        if userPrefs['wantPageNumbers']:
            self.pagenumSelect.select()
        else:
            self.pagenumSelect.deselect()
        if userPrefs['wantTOC']:
            self.tocSelect.select()
            self.btnPageOneOnFirst['state'] = NORMAL
        else:
            self.tocSelect.deselect()
            self.btnPageOneOnFirst['state'] = DISABLED
        if userPrefs['wantBindingOffset']:
            self.bindingoffsetSelect.select()
        else:
            self.bindingoffsetSelect.deselect()
        if userPrefs['wantDoubleSided']:
            self.doublesidedSelect.select()
        else:
            self.doublesidedSelect.deselect()
        if userPrefs['wantPageOneOnFirst']:
            self.btnPageOneOnFirst.select()
        else:
            self.btnPageOneOnFirst.deselect()

    def __aboutDlgShow(self):
        """Brings up a dialog with support url etc."""
        about = AboutDialog(self)


def setup_tk():
    """Create and setup the Tkinter app."""
    root = Tk()
    app = Application(root)
    app.master.title(WINDOW_TITLE)

    # now get a list of all the fonts Scribus knows about...
    fontList = scribus.getFontNames()
    fontList.sort()
    # and load the list into the GUI listbox...
    for i in fontList:
        app.listbox1.insert(END, i)
    app.sampleBlocksPerPage = draw_selection(scribus.getFontNames(), 1)
    # now set the status bar message...
    app.statusbarUpdate()
    # set up widgets using data from userPrefs...
    app.initialiseWidgets()
    return app

def initialisation():
    """Test for suitable fonts and on success creates tkinter app."""
    try:
        dD['bookstyleFixedFont'] = set_font_fixed(fontsListFixed)
        dD['bookstylePropFont'] = set_font_proportional(fontsListProportional)
    except:
        scribus.messageBox('Font problem',
                           '%s' % sys.exc_info()[1],
                           scribus.ICON_WARNING)
        sys.exit(1)
    # load users saved defaults...
    restore_user_conf(CONFIG_PATH)
    # get and set the initial paper size to match default radiobutton selection...
    dD.update(set_page_geometry(dD, geometriesList, userPrefs['paperSize'], userPrefs['wantBindingOffset']))
    # Made it this far so its time to create our Tkinter app...
    app = setup_tk()
    # now show the main window and wait for user to do something...
    app.mainloop()


def main(argv):
    """Application initialization, font checks and initial setup."""
    initialisation()


def main_wrapper(argv):
    """The main_wrapper() function disables redrawing, sets a sensible generic
    status bar message, and optionally sets up the progress bar. It then runs
    the main() function. Once everything finishes it cleans up after the main()
    function, making sure everything is sane before the script terminates."""
    try:
        scribus.statusMessage('Running script...')
        scribus.progressReset()
        main(argv)
    finally:
        # Exit neatly even if the script terminated with an exception,
        # so we leave the progress bar and status bar blank and make sure
        # drawing is enabled.
        if scribus.haveDoc() > 0:
            scribus.setRedraw(True)
        scribus.statusMessage('')
        scribus.progressReset()


# This code detects if the script is being run as a script, or imported as a module.
# It only runs main() if being run as a script. This permits you to import your script
# and control it manually for debugging.
if __name__ == '__main__':
    main_wrapper(sys.argv)

