#!/usr/bin/env python2.3
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
#  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
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
This release     : v0.7.3tk (released 13th Aug 2005)
Copyright        : (C) 2003 - 2005 Steve Callcott
Latest releases
and support      : www.firstwish.co.uk/sjc/scribus/index.php
Maintainer       : Steve Callcott 2003 - 2005
Email            : stotte@firstwish.co.uk

For revision history see the ChangeLog file.
Bugs and future plans are listed in the TODO file.
See NEWS for new features since last version.

WHATS NEW v0.7.3tk
Fix typo in exception code.
Modified case of some script variables to make compatible with changes
in Scribus 1.3 scriptor.
Removed the reduntant "self.master.maxsize(1, 1)" from the application class.

WHATS NEW v0.7.2tk
More cleanups in font preview code. If a font cannot be displayed
then the preview panel is cleared. Removed many error messages returned
to the console.

WHATS NEW v0.7.1tk
Removed discontinued email address.

WHATS NEW v0.7tk
Added a preview panel so user can see a sample of what a font may look like
before selecting it to use.
Detects failure of Python Imaging Library module to load and tests for the
ability to write to .scribus folder then disables preview if necessary.
Incorporated Craig Ringers boilerplate and Scribus function case changes.
Put labels on the left and right listboxes describing what they do.
Listboxes now get focus when selected with the mouse. This allows Up Down
keys to be used to scroll through font names.
When selecting a single item in a listbox, the font highlighted will be
displayed in a panel.
Some function names have changed and some docstrings added.
The main window should no longer be expandable.

******************************************************************************
"""

import sys
import os

try:
    import scribus
except ImportError,err:
    print "This Python script is written for the Scribus scripting interface."
    print "It can only be run from within Scribus."
    sys.exit(1)

try:
    from Tkinter import *
except ImportError,err:
    print "This script will not work without Tkinter"
    sys.exit(1)


WINDOWTITLE = "Font Sampler v0.7.3tk - Steve Callcott"
TEMPPATH = os.path.join(os.path.expanduser("~"), ".scribus")

showPreviewPanel = 1  # change to 0 to permanently hide the preview

if showPreviewPanel:
    if not os.path.exists(TEMPPATH):
        print ".scribus folder not found, disabling font preview panel"
        showPreviewPanel = 0

if showPreviewPanel:
    if not os.access(TEMPPATH, os.W_OK):   
        print "Unable to write to .scribus folder, disabling font preview panel"
        showPreviewPanel = 0


if showPreviewPanel:
    try:
        import Image
    except ImportError,err:
        print "You need to install Python Imaging Library (PIL)."
        print "If using gentoo then you need to emerge /dev-python/imaging"
        print "If using an RPM based linux distribution then you add python-imaging or similar."
        print "Script will continue without the font preview panel."
        showPreviewPanel = 0


if showPreviewPanel:
    try:
        import ImageTk
    except ImportError,err:
        print "Module ImageTk not found, font preview disabled"
        showPreviewPanel = 0


# Define some empty containers for later...
app = None
root = None
samplePic = None
previewId = None

class BookStyle:
    pass
bookstyle = BookStyle()


class TocStyle:
    pass
tocstyle = TocStyle()


class Paper:
    pass
paper = Paper()


bookstyle.fixedfont = ""
bookstyle.propfont = ""
tocstyle.charsInRow = 75


def setFixedFont():
    fixed = (
    "Luxi Mono Regular",
    "Nimbus Mono L Regular",
    "Courier 10 Pitch Regular",
    "Courier New Regular",
    "Courier Regular",
    "Andale Mono Regular",
    "Larabiefont Regular"
    )

    fontList = scribus.getFontNames()
    found = 0
    for f in fixed:
        if found == 1:
            break
        for i in fontList:
            if found == 0:
                if f == i:
                    bookstyle.fixedfont = f
                    found = 1
                    break
    if found == 0:
        print "No suitable fixed width font found."
        print "Please install at least one of these fixed width fonts:"
        print fixed


def setPropFont():
    proportional = (
    "Nimbus Sans L Regular",
    "Luxi Sans Regular",
    "Bitstream Vera Sans",
    "Helvetica",
    "Arial Regular"
    )

    fontList = scribus.getFontNames()
    found = 0
    for p in proportional:
        if found == 1:
            break
        for i in fontList:
            if found == 0:
                if p == i:
                    bookstyle.propfont = p
                    found = 1
                    break
    if found == 0:
        print "No suitable proportional font found."
        print "Please install at least one of these proportional fonts:"
        print proportional


def setPaperSize(paperSize):
    if paperSize == 1:              # A4 - 595 x 842 Points
        paper.size = scribus.PAPER_A4
        paper.width = 595
        paper.height = 842
        paper.tmargin = 60
        paper.bmargin = 50
        paper.lmargin = 50
        paper.rmargin = 50
        paper.binding = 16
        tocstyle.tocRows = 56
        paper.pagenumvoffset = 16
        paper.textwidth = paper.width - paper.lmargin - paper.rmargin - 2
        paper.lmarginOdd = paper.lmargin + paper.binding
        paper.rmarginEven = paper.rmargin + paper.binding
        paper.textheight = paper.height - paper.tmargin - paper.bmargin
        paper.margins = paper.lmargin, paper.rmargin, paper.tmargin, paper.bmargin
    if paperSize == 2:              # US Letter - 612 x 792 Points
        paper.size = scribus.PAPER_LETTER
        paper.width = 612
        paper.height = 792
        paper.tmargin = 27
        paper.bmargin = 45
        paper.lmargin = 50
        paper.rmargin = 50
        paper.binding = 18
        tocstyle.tocRows = 55
        paper.pagenumvoffset = 16
        paper.textwidth = paper.width - paper.lmargin - paper.rmargin - 2
        paper.lmarginOdd = paper.lmargin + paper.binding
        paper.rmarginEven = paper.rmargin + paper.binding
        paper.textheight = paper.height - paper.tmargin - paper.bmargin
        paper.margins = paper.lmargin, paper.rmargin, paper.tmargin, paper.bmargin


def setTextWidthForBinding(wantBindingOffset):
    # if we are adding a binding offset to the margins then we will have less width for our text.
    if wantBindingOffset == 1:
        paper.textwidth = paper.width - paper.lmargin - paper.rmargin - paper.binding - 2
    else:
        paper.textwidth = paper.width - paper.lmargin - paper.rmargin -2


def setPageGeometries():
    setPaperSize(app.paper.get())
    setTextWidthForBinding(app.wantBindingOffset.get())


def setOddEven(pageNum):
    """ Sets the left and right margins.

    Checks the number passed to it and sets left and right
    margins accordingly. Call once after each new page is created.
    Returns 1 if even and 0 if odd page.
    """
    if pageNum % 2 == 0:
        isEvenPage = 1                                          # Even side
    else:
        isEvenPage = 0                                          # Odd side

    if app.wantBindingOffset.get() == 1:
        if isEvenPage == 1 and app.wantDoubleSided.get() == 1:  # Even (when double sided)
            paper.leftSide = paper.lmargin + 1
            paper.rightSide = paper.rmarginEven + 1
        else:                                                   # Odd side
            paper.leftSide = paper.lmarginOdd + 1
            paper.rightSide = paper.rmargin + 1
    else:                                                       # No binding
        paper.leftSide = paper.lmargin + 1
        paper.rightSide = paper.rmargin + 1

    return isEvenPage


def addSampleRow(font, fontSize, lineSpace, textString, x, y, w, h, style, getSizeOnly):
    if getSizeOnly == 0:
        f = scribus.createText(x, y, w, h)
        scribus.insertText(textString, 0, f)
        if style == "l":
            scribus.setFont(bookstyle.propfont, f)
        else:
            scribus.setFont(font, f)
        scribus.setFontSize(fontSize, f)
        scribus.setLineSpacing(lineSpace, f)
        scribus.setTextAlignment(0, f)
    return y + h + 1


def drawSampleBlock(fontName, x, y, w, getSizeOnly):
    startPos = y

    # An "l" as last parameter will result in that line being in a plain font.
    # Use \xBC etc to insert Hex ascii chars into the sample strings below.

    samples = (
    # Template: fontsize, linespace, textstring, height, style
    # comment out any sample lines below that you do not require...
    [16, 15, fontName + '\n', 23, "l"],
    [10.5, 12, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#?$*&\n", 18, ""],
    [6, 6, "This line is in 6 point\n", 13, ""],
    [8, 8, "This line is in 8 point\n", 16, ""],
    [10, 11, "This line is in 10 point\n", 19, ""],
    [12, 11, "This line is in 12 point\n", 21, ""],
    [16, 13, "This line is in 16 point\n", 26, ""],
    [20, 16, "This line is in 20 point\n", 31, ""],
    [32, 29, "This line is in 32 point\n", 49, ""]
    )

    for i in samples:
        # (note there is one extra point inserted by addSampleRow() for each row generated)...
        y = addSampleRow(fontName, i[0], i[1], i[2], x, y, w, i[3], i[4], getSizeOnly)
    y = y + 1

    if getSizeOnly == 0:
        scribus.createLine(x, y, (w + x), y)
    else:
        y = y + 1   # if changing line above then check this, it should be exactly the same as the line height
    y = y + 5
    return y - startPos


def addTocRow(fontName, pageNum, yPos, frame):
    dotLine = ""
    dotQuant = tocstyle.charsInRow - len(fontName) - len(str(pageNum)) + 1
    for i in range(dotQuant):
        dotLine = dotLine + '.'
    oneLine = fontName + dotLine + str(pageNum) + "\n"
    scribus.insertText(oneLine, yPos, frame)
    yPos = yPos + len(oneLine) + 0
    return yPos


def buildTocPageFrames():
    # first put a header on the empty page...
    textstring = "Table of Contents"
    yPos = paper.tmargin + 1
    header = scribus.createText(paper.leftSide, yPos, paper.textwidth, 35)
    scribus.insertText(textstring, 0, header)
    scribus.setFont(bookstyle.propfont, header)
    scribus.setFontSize(24, header)
    scribus.setTextAlignment(1, header)

    # now create a text frame for the table of contents...
    yPos = yPos + 36
    body = scribus.createText(paper.leftSide, yPos, paper.textwidth, paper.height - yPos - paper.bmargin - 1)
    scribus.setFont(bookstyle.fixedfont, body)
    scribus.setFontSize(10, body)
    scribus.setLineSpacing(12, body)
    return body


def addToc(tocList):
    rowCount = 0
    yPos = 0
    tocPageNum = 1
    tocPageCount = 1

    scribus.newPage(tocPageNum)
    isEvenPage = setOddEven(tocPageNum)
    body = buildTocPageFrames()             # create frames for new empty page
    if isEvenPage == 0:
        scribus.setTextAlignment(2, body)
    else:
        scribus.setTextAlignment(0, body)
    for i in tocList:
        yPos = addTocRow(i[0], i[1], yPos, body)
        rowCount = rowCount + 1
        if rowCount > tocstyle.tocRows:     # Need to build a new TOC page (started from zero, not one)
            tocPageNum = tocPageNum + 1
            scribus.newPage(tocPageNum)
            isEvenPage = setOddEven(tocPageNum)
            body = buildTocPageFrames()
            if isEvenPage == 0:
                scribus.setTextAlignment(2, body)
            else:
                scribus.setTextAlignment(0, body)
            rowCount = 0
            yPos = 0
            tocPageCount = tocPageCount + 1
    if app.wantDoubleSided.get() == 1:
        if tocPageCount % 2 != 0:           # Odd page
            tocPageNum = tocPageNum + 1
            scribus.newPage(tocPageNum)     # Add an extra page if odd number


def addPageNum(pageNum):
    yPos = paper.height - paper.bmargin - paper.pagenumvoffset
    footer = scribus.createText(paper.leftSide, yPos, paper.textwidth, 15)
    scribus.insertText("%s" % pageNum, 0, footer)
    scribus.setFont(bookstyle.propfont, footer)
    scribus.setFontSize(9, footer)
    scribus.setTextAlignment(1, footer)
    scribus.setLineSpacing(10, footer)


def useSelection(fontList):
    tocList = []
    yPos = paper.tmargin + 1
    pageNum = 1
    if scribus.newDoc(paper.size, paper.margins, scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.NOFACINGPAGES, scribus.FIRSTPAGERIGHT):
        # We have a new page by default so set it up first...
        setOddEven(pageNum)
        if app.wantPageNum.get() == 1:
            addPageNum(pageNum)
        scribus.createLine(paper.leftSide, yPos, paper.leftSide + paper.textwidth, yPos)
        yPos = yPos + 5
        for i in fontList:
            # Test if fits...
            blockHeight = drawSampleBlock(i, paper.leftSide, yPos, paper.textwidth, 1)
            if yPos + blockHeight > paper.height - paper.bmargin - paper.pagenumvoffset:
                # Not enough room so create a new page first...
                scribus.newPage(-1)
                pageNum = pageNum + 1
                setOddEven(pageNum)
                yPos = paper.tmargin +1     # Reset y position back to top of page
                if app.wantPageNum.get() == 1:
                    addPageNum(pageNum)
                scribus.createLine(paper.leftSide, yPos, paper.leftSide + paper.textwidth, yPos)
                yPos = yPos + 5
            # Now place the actual sample block...
            blockHeight = drawSampleBlock(i, paper.leftSide, yPos, paper.textwidth, 0)
            yPos = yPos + blockHeight
            tocList.append([i, pageNum])    # Add to TOC
        if app.wantToc.get() == 1:
            addToc(tocList)                 # Insert table of contents - (before page numbering)
        scribus.gotoPage(1)
    app.quit()


def fontPreview(fontName):
    """Gets the named font and puts a sample in the preview panel.

    Pick up the temp sample qpixmap file and display it in a canvas object
    The temp file is put in the users ".scribus" folder and cleaned up on exit.
    We create samplePic as a global as a workaround because the reference count
    does not increase when we add the image to the canvas. Therefore python
    garbage collection removes our image before we have even displayed it.
    Note app.previewPanel is the actual canvas.
    """
    global samplePic
    global previewId
    scribus.renderFont(fontName, os.path.join(TEMPPATH,"temp079r.bmp"),"Woven silk pyjamas exchanged for blue quartz",28)    
    try:
        tempPic = Image.open(os.path.join(TEMPPATH,"temp079r.bmp"))
        tempPic.save(os.path.join(TEMPPATH,"temp079r.jpeg"),format="JPEG")
        tempImage = Image.open(os.path.join(TEMPPATH,"temp079r.jpeg"))
        imgDimen = tempPic.getbbox()
        samplePic = ImageTk.PhotoImage(tempImage)
        # To center the image use "Half display height minus half the image height"
        # preview panel is allegedly 56 (60 less a 2 pixel border top and bottom)
        # need to be lower than that to look correct visually... 
        topEdge = (32 - (imgDimen[3] / 2))
        previewId = app.previewPanel.create_image(5, topEdge, anchor=NW, image=samplePic)
        os.remove(os.path.join(TEMPPATH,"temp079r.bmp"))
        os.remove(os.path.join(TEMPPATH,"temp079r.jpeg"))
    except IOError:
        samplePic = None
        previewId = app.previewPanel.create_image(0, 0, anchor=NW, image=samplePic)
    return


class Application(Frame):
    def __init__(self, master = None):
        Frame.__init__(self, master)

        self.grid()

        # Remove maximise button and resize. Not good to allow resizable window
        # because the listboxes are fixed width...
        self.master.resizable(0, 0)   

        # now start adding our widgets starting with the top frame...
        self.listbox_frame = Frame(self)
        self.listbox_frame.grid(row=0, column=0, sticky=EW)

        # left hand listbox assembly
        self.leftListbox_frame = Frame(self.listbox_frame, borderwidth=1, relief=SUNKEN)
        self.leftListbox_frame.grid(row=1, column=0)

        self.leftLabel = Label(self.listbox_frame, text="Available Fonts")
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
        self.xScroll1["command"] = self.listbox1.xview
        self.yScroll1["command"] = self.listbox1.yview

        def __listbox1KeyRelease(event):
            """Check if an Up or Down key has been pressed and released and
            if so the preview panel is refreshed. If the keys are held down 
            the file system slows the scroll. Need a timer here to delay 
            updates."""
            if (event.keysym == "Down" or event.keysym == "Up"):
                __listbox1DoLogicCallback(self)

        def __listbox1SingleClick(event):
            """Call this first when listbox1 is clicked with mouse to put focus
            into the listbox. Note we call when mouse click is released, not pressed,
            due to the fact that the listbox does not change the selection until the button
            is released."""
            self.listbox1.focus_set()
            __listbox1DoLogicCallback(self)
        self.listbox1.bind("<ButtonRelease-1>", __listbox1SingleClick)

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
        self.listbox1.bind("<FocusIn>", __listbox1DoLogicCallback)
        self.listbox1.bind('<Any-KeyRelease>', __listbox1KeyRelease)

        def __listbox1DoubleClickCallback(event):
            """The single click event will fire also when left listbox
            is double clicked but we are detecting the single click button up event."""
            self.__listSelectionToRight()
        self.listbox1.bind("<Double-Button-1>", __listbox1DoubleClickCallback)

        # middle button frame assembly
        self.midbutton_frame = Frame(self.listbox_frame)
        self.midbutton_frame.grid(row=0, rowspan=2, column=1, sticky=NSEW)

        self.rsingleButton = Button(self.midbutton_frame, state='disabled', text=">", command=self.__rsingleButtonClick)
        self.rsingleButton.grid(row=0, column=0, padx=5, pady=5, sticky=EW)
        self.rdoubleButton = Button(self.midbutton_frame, text=">>", command=self.__rdoubleButtonClick)
        self.rdoubleButton.grid(row=1, column=0, padx=5, pady=5, sticky=EW)

        self.itemupButton = Button(self.midbutton_frame, state='disabled', text="Up", command=self.__itemupButtonClick)
        self.itemupButton.grid(row=2, column=0, padx=5, pady=5, sticky=EW)
        self.itemdownButton = Button(self.midbutton_frame, state='disabled', text="Down", command=self.__itemdownButtonClick)
        self.itemdownButton.grid(row=3, column=0, padx=5, pady=5, sticky=EW)

        self.lsingleButton = Button(self.midbutton_frame, state='disabled', text="<", command=self.__lsingleButtonClick)
        self.lsingleButton.grid(row=4, column=0, padx=5, pady=5, sticky=EW)
        self.ldoubleButton = Button(self.midbutton_frame, state='disabled', text="<<", command=self.__ldoubleButtonClick)
        self.ldoubleButton.grid(row=5, column=0, padx=5, pady=5, sticky=EW)

        # Right hand listbox assembly
        self.rightListbox_frame = Frame(self.listbox_frame, borderwidth=1, relief=SUNKEN)
        self.rightListbox_frame.grid(row=1, column=2)

        self.rightLabel = Label(self.listbox_frame, text="Selected Fonts")
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
        self.xScroll2["command"] = self.listbox2.xview
        self.yScroll2["command"] = self.listbox2.yview

        def __listbox2SingleClick(event):
            """Similar to __listbox1SingleClick()."""
            self.listbox2.focus_set()
            __listbox2DoLogicCallback(self)
        self.listbox2.bind("<ButtonRelease-1>", __listbox2SingleClick)

        def __listbox2KeyRelease(event):
            if (event.keysym == "Down" or event.keysym == "Up"):
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
        self.listbox2.bind("<FocusIn>", __listbox2DoLogicCallback)
        self.listbox2.bind('<Any-KeyRelease>', __listbox2KeyRelease)

        def __listbox2DoubleClickCallback(event):
            """Similar to __listbox1DoubleClickCallback()."""
            self.__listSelectionToLeft()
        self.listbox2.bind("<Double-Button-1>", __listbox2DoubleClickCallback)

        # now draw the bottom font preview frame if required...
        if showPreviewPanel:
            self.preview_frame = Frame(self)
            self.preview_frame.grid(row=1, column=0, sticky=EW)
            self.previewPanel = Canvas(self.preview_frame, height=60, bg="white", bd=2, relief=SUNKEN)
            self.previewPanel.pack(fill=X)

        # now draw the bottom controls frame...
        self.controls_frame = Frame(self)
        self.controls_frame.grid(row=2, column=0, sticky=EW)

        # now the paper size radio buttons...
        self.paper = IntVar()
        self.radiobutton_frame = Frame(self.controls_frame, bd=1, relief=RIDGE)
        self.radiobutton_frame.grid(row=0, column=0, padx=10, pady=2)
        self.a4papersizeSelect = Radiobutton(self.radiobutton_frame, text="A4", variable=self.paper, value=1, command=self.__a4paperButtonClick)
        self.a4papersizeSelect.grid(row=0, column=0, padx=0, sticky=W)
        self.uspapersizeSelect = Radiobutton(self.radiobutton_frame, text="US Letter", variable=self.paper, value=2, command=self.__uspaperButtonClick)
        self.uspapersizeSelect.grid(row=1, column=0, padx=0, sticky=W)
        self.a4papersizeSelect.select()

        # now the TOC and page number selection buttons...
        self.wantToc = IntVar()
        self.wantPageNum = IntVar()
        self.togglebutton_frame = Frame(self.controls_frame, bd=1, relief=RIDGE)
        self.togglebutton_frame.grid(row=0, column=1, padx=10, pady=2)
        self.pagenumSelect = Checkbutton(self.togglebutton_frame, text="want page numbers", variable=self.wantPageNum, offvalue=0, onvalue=1)
        self.pagenumSelect.grid(row=0, column=0, padx=0, sticky=W)
        self.tocSelect = Checkbutton(self.togglebutton_frame, text="Print TOC", variable=self.wantToc, offvalue=0, onvalue=1)
        self.tocSelect.grid(row=1, column=0, padx=0, sticky=W)
        self.pagenumSelect.select()
        self.tocSelect.select()

        # now the binding offset and double sided selection buttons...
        self.wantBindingOffset = IntVar()
        self.wantDoubleSided = IntVar()
        self.bindingbutton_frame = Frame(self.controls_frame, bd=1, relief=RIDGE)
        self.bindingbutton_frame.grid(row=0, column=2, padx=10, pady=2)
        self.bindingoffsetSelect = Checkbutton(self.bindingbutton_frame, text="Extra offset for binding", variable=self.wantBindingOffset, offvalue=0, onvalue=1, command=self.__bindingoffsetSelectButtonClick)
        self.bindingoffsetSelect.grid(row=0, column=0, sticky=W)
        self.doublesidedSelect = Checkbutton(self.bindingbutton_frame, text="Double sided pages", variable=self.wantDoubleSided, offvalue=0, onvalue=1)
        self.doublesidedSelect.grid(row=1, column=0, rowspan=2, sticky=W)

        # now the ok and cancel buttons...
        self.cancelButton = Button(self.controls_frame, text="Cancel", command=self.__cancelButtonClick)
        self.cancelButton.grid(row=0, column=3, padx=5)
        self.okButton = Button(self.controls_frame, text="OK", state='disabled', command=self.__okButtonClick)
        self.okButton.grid(row=0, column=4, padx=5)

        # now create the bottom status bar frame and contents...
        self.status_frame = Frame(self)
        self.status_frame.grid(row=3, column=0, sticky=E+W)
        self.status0 = Label(self.status_frame, bd=1, relief=SUNKEN, anchor=W)
        self.status0.pack(side=LEFT)
        self.status1 = Label(self.status_frame, bd=1, relief=SUNKEN, anchor=W)
        self.status1.pack(side=LEFT)
        self.status2 = Label(self.status_frame, bd=1, relief=SUNKEN, anchor=W)
        self.status2.pack(fill=X)

    def statusbarUpdate(self):
        available = self.listbox1.size()
        selected = self.listbox2.size()
        size = FloatType(selected)
        value = size / 3
        pages = IntType(value)                  # Get whole part of number
        value = value - pages                   # Remove whole number part
        if value > 0:                           # Test remainder
            pages = pages + 1                   # Had remainder so add a page
        self.status0['text'] = "Fonts available: %s    " % (available + selected)
        self.status1['text'] = "Fonts selected: %s    " % selected
        self.status2['text'] = "Sheets required: %s    " % pages

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
        # This quick way just clears both sides and reloads left listbox in correct order from scratch
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
        """Just sets the buttons active or inactive
        See testUpDown() for actual evaluation"""
        if up == 1:
            self.itemupButton['state'] = NORMAL
        else:
            self.itemupButton['state'] = DISABLED
        if down == 1:
            self.itemdownButton['state'] = NORMAL
        else:
            self.itemdownButton['state'] = DISABLED

    def __testUpDownState(self):
        # Only ungray the up and down buttons when just a single item is
        # selected and then it should be up, down or both depending on its
        # position in the listbox. At all other times gray out both.

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
            fontPreview(selectedFont)

    def quit(self):
        self.master.destroy()

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

    def __cancelButtonClick(self):
        self.master.destroy()

    def __bindingoffsetSelectButtonClick(self):
        setPageGeometries()

    def __a4paperButtonClick(self):
        setPageGeometries()

    def __uspaperButtonClick(self):
        setPageGeometries()

    def __okButtonClick(event):
        useSelection(app.listbox2.get(0, END))


def setupTk():
    """Create and setup the Tkinter app."""
    global root
    global app
    root = Tk()
    app = Application(root)
    app.master.title(WINDOWTITLE)


def initialiseGui():
    """Setup the default settings for the window."""
    # get and set the initial paper size to match default radiobutton selection
    setPageGeometries() 
    # now get a list of all the fonts...
    fontList = scribus.getFontNames()
    fontList.sort()
    # and put the list in the GUI listbox...
    for i in fontList:
        app.listbox1.insert(END, i)
    setFixedFont()
    setPropFont()
    app.statusbarUpdate()    


def main(argv):
    """Application initialization, font checks and initial setup."""
    # first create our Tkinter app...
    setupTk()

    # Set up defaults...
    initialiseGui()

    # now show the fonts list and wait for user to make a choice...
    app.mainloop()


def main_wrapper(argv):
    """The main_wrapper() function disables redrawing, sets a sensible generic
    status bar message, and optionally sets up the progress bar. It then runs
    the main() function. Once everything finishes it cleans up after the main()
    function, making sure everything is sane before the script terminates."""
    try:
        scribus.statusMessage("Running script...")
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

