""" DESCRIPTION & USAGE:
This script needs Tkinter. It will create a GUI with an alphabetical list
of fonts using the names as they will be shown in Scribus. User can select
one or more fonts and create an example sheet(s) to print or create a PDF
from. It is heavily commented to make it easier for the user to adjust it
for his / her own needs.

First release	: 30/12/2003

This release     : v0.6.1 (released 24th Feb 2004)

Copyright		: (C) 2003 - 2004 Steve Callcott

email			: stotte@ntlworld.com

Latest releases	: www.

Maintainer		: Steve Callcott 2003 - 2004

Feature requests and bug reports to stotte@ntlworld.com
For revision history see the ChangeLog file.
Bugs and future plans are listed in the TODO file.
See NEWS for new features since last version.

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
"""

from Tkinter import *
from scribus import *


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
	"Nimbus Mono L Regular"
	"Courier 10 Pitch Regular"
	"Courier New Regular"
	"Courier Regular"
	"Andale Mono Regular"
	"Larabiefont Regular"
	)

	fontlist = GetFontNames()
	found = 0
	for f in fixed:
		if found == 1:
			break
		for i in fontlist:
			if found == 0:
				if f == i:
					bookstyle.fixedfont = f
					found = 1
					break
	if found == 0:
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

	fontlist = GetFontNames()
	found = 0
	for p in proportional:
		if found == 1:
			break
		for i in fontlist:
			if found == 0:
				if p == i:
					bookstyle.propfont = p
					found = 1
					break
	if found == 0:
		print "Please install at least one of these proportional fonts:"
		print proportional


def setPaperSize(paperSize):
	if paperSize == 1:				# A4 - 595 x 842 Points
		paper.size = Paper_A4
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
	if paperSize == 2:				# US Letter - 612 x 792 Points
		paper.size = Paper_Letter
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
	# This function checks the number passed to it and sets left and right
	# margins accordingly.Call it once after each new page is created.
	# Also it will return 1 if even and 0 if odd page.
	if pageNum % 2 == 0:
		isEvenPage = 1											# Even side
	else:
		isEvenPage = 0											# Odd side

	if app.wantBindingOffset.get() == 1:
		if isEvenPage == 1 and app.wantDoubleSided.get() == 1:	# Even (when double sided)
			paper.leftSide = paper.lmargin + 1
			paper.rightSide = paper.rmarginEven + 1
		else:													# Odd side
			paper.leftSide = paper.lmarginOdd + 1
			paper.rightSide = paper.rmargin + 1
	else:														# No binding
		paper.leftSide = paper.lmargin + 1
		paper.rightSide = paper.rmargin + 1

	return isEvenPage


def addSampleRow(font, fontsize, linespace, textstring, x, y, w, h, style, getSizeOnly):
	if getSizeOnly == 0:
		f = CreateText(x, y, w, h)
		InsertText(textstring, 0, f)
		if style == "l":
			SetFont(bookstyle.propfont, f)
		else:
			SetFont(font, f)
		SetFontSize(fontsize, f)
		SetLineSpacing(linespace, f)
		SetTextAlignment(0, f)
	return y + h + 1


def drawSampleBlock(fontname, x, y, w, getSizeOnly):
	startPos = y

	# An "l" as last parameter will result in that line being in a plain font.
	# Use \xBC etc to insert Hex ascii chars into the sample strings below.

	samples = (
	# Key: fontsize, linespace, textstring, height, style
	# comment out any sample lines below that you do not require...
	[16, 15, fontname + '\n', 23, "l"],
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
		y = addSampleRow(fontname, i[0], i[1], i[2], x, y, w, i[3], i[4], getSizeOnly)
	y = y + 1

	if getSizeOnly == 0:
		CreateLine(x, y, (w + x), y)
	else:
		y = y + 1	# if changing line above then check this, it should be exactly  the same as the line height
	y = y + 5
	return y - startPos


def addTocRow(fontname, pageNum, ypos, frame):
	dotline = ""
	dotquant = tocstyle.charsInRow - len(fontname) - len(str(pageNum)) + 1
	for i in range(dotquant):
		dotline = dotline + '.'
	oneline = fontname + dotline + str(pageNum) + "\n"
	InsertText(oneline, ypos, frame)
	ypos = ypos + len(oneline) + 0
	return ypos


def buildTocPageFrames():
	# first put a header on the empty page...
	textstring = "Table of Contents"
	ypos = paper.tmargin + 1
	header = CreateText(paper.leftSide, ypos, paper.textwidth, 35)
	InsertText(textstring, 0, header)
	SetFont(bookstyle.propfont, header)
	SetFontSize(24, header)
	SetTextAlignment(1, header)

	# now create a text frame for the table of contents...
	ypos = ypos + 36
	body = CreateText(paper.leftSide, ypos, paper.textwidth, paper.height - ypos - paper.bmargin - 1)
	SetFont(bookstyle.fixedfont, body)
	SetFontSize(10, body)
	SetLineSpacing(12, body)
	return body


def addToc(tocList):
	rowCount = 0
	ypos = 0
	tocPageNum = 1
	tocPageCount = 1

	NewPage(tocPageNum)
	isEvenPage = setOddEven(tocPageNum)
	body = buildTocPageFrames()				# create frames for new empty page
	if isEvenPage == 0:
		SetTextAlignment(2, body)
	else:
		SetTextAlignment(0, body)
	for i in tocList:
		ypos = addTocRow(i[0], i[1], ypos, body)
		rowCount = rowCount + 1
		if rowCount > tocstyle.tocRows:		# Need to build a new TOC page (started from zero, not one)
			tocPageNum = tocPageNum + 1
			NewPage(tocPageNum)
			isEvenPage = setOddEven(tocPageNum)
			body = buildTocPageFrames()
			if isEvenPage == 0:
				SetTextAlignment(2, body)
			else:
				SetTextAlignment(0, body)
			rowCount = 0
			ypos = 0
			tocPageCount = tocPageCount + 1
	if app.wantDoubleSided.get() == 1:
		if tocPageCount % 2 != 0:			# Odd page
			tocPageNum = tocPageNum + 1
			NewPage(tocPageNum)				# Add an extra page if odd number


def addPageNum(pagenum):
	ypos = paper.height - paper.bmargin - paper.pagenumvoffset
	footer = CreateText(paper.leftSide, ypos, paper.textwidth, 15)
	InsertText("%s" % pagenum, 0, footer)
	SetFont(bookstyle.propfont, footer)
	SetFontSize(9, footer)
	SetTextAlignment(1, footer)
	SetLineSpacing(10, footer)


def useSelection(fontlist):
	tocList = []
	ypos = paper.tmargin + 1
	pageNum = 1
	if NewDoc(paper.size, paper.margins, Portrait, 1, Points, NoFacingPages, FirstPageRight):
		SetRedraw(0)						# Disable redrawing
		# We have a new page by default so set it up first...
		setOddEven(pageNum)
		if app.wantPageNum.get() == 1:
			addPageNum(pageNum)
		CreateLine(paper.leftSide, ypos, paper.leftSide + paper.textwidth, ypos)
		ypos = ypos + 5
		for i in fontlist:
			# Test if fits...
			blockHeight = drawSampleBlock(i, paper.leftSide, ypos, paper.textwidth, 1)
			if ypos + blockHeight > paper.height - paper.bmargin - paper.pagenumvoffset:
				# Not enough room so create a new page first...
				NewPage(-1)
				pageNum = pageNum + 1
				setOddEven(pageNum)
				ypos = paper.tmargin +1		# Reset y position back to top of page
				if app.wantPageNum.get() == 1:
					addPageNum(pageNum)
				CreateLine(paper.leftSide, ypos, paper.leftSide + paper.textwidth, ypos)
				ypos = ypos + 5
			# Now place the actual sample block...
			blockHeight = drawSampleBlock(i, paper.leftSide, ypos, paper.textwidth, 0)
			ypos = ypos + blockHeight
			tocList.append([i, pageNum])	# Add to TOC
		if app.wantToc.get() == 1:
			addToc(tocList)					# Insert table of contents - (before page numbering)
		GotoPage(1)
		SetRedraw(1)						# Enable redraws again
	app.quit()


class Application(Frame):
	def __init__(self, master = None):
		Frame.__init__(self, master)
		self.pack(fill=BOTH, expand=1)
		self.grid_rowconfigure(0, weight=1)
		self.grid_columnconfigure(0, weight=1)

		# now start adding our widgets starting with the top frame...
		self.listbox_frame = Frame(self)
		self.listbox_frame.grid()

		# left hand listbox
		self.yScroll1 = Scrollbar(self.listbox_frame, orient=VERTICAL)
		self.yScroll1.grid(row=0, column=1, sticky=NS)
		self.xScroll1 = Scrollbar(self.listbox_frame, orient=HORIZONTAL)
		self.xScroll1.grid(row=1, column=0, sticky=EW)

		self.listbox1 = Listbox(self.listbox_frame,
			xscrollcommand=self.xScroll1.set,
			yscrollcommand=self.yScroll1.set,
			selectmode=EXTENDED,
			height=20, width=40)
		self.listbox1.grid(row=0, column=0, sticky=NSEW)
		self.xScroll1["command"] = self.listbox1.xview
		self.yScroll1["command"] = self.listbox1.yview

		def __lbox1Callback(event):
			self.__listSelectionToRight()
		self.listbox1.bind("<Double-Button-1>", __lbox1Callback)

		def __lbox1ClickCallback(event):
			self.listbox2.selection_clear(0,END)
			self.__setUpDownActive(0, 0)	# Force a disable if in other box
			if self.listbox1.size() > 0:
				self.__setSelButtonsActive(0, 1)
			else:
				self.__setSelButtonsActive(0, 0)
		self.listbox1.bind("<Button-1>", __lbox1ClickCallback)
		self.listbox1.bind("<FocusIn>", __lbox1ClickCallback)
		self.listbox1.bind("<FocusOut>", __lbox1ClickCallback)

		# middle button frame
		self.midbutton_frame = Frame(self.listbox_frame)
		self.midbutton_frame.grid(row=0, column=2, sticky=NSEW)

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

		# Right hand listbox
		self.yScroll2 = Scrollbar(self.listbox_frame, orient=VERTICAL)
		self.yScroll2.grid(row=0, column=4, sticky=NS)
		self.xScroll2 = Scrollbar(self.listbox_frame, orient=HORIZONTAL)
		self.xScroll2.grid(row=1, column=3, sticky=EW)

		self.listbox2 = Listbox(self.listbox_frame,
			xscrollcommand=self.xScroll2.set,
			yscrollcommand=self.yScroll2.set,
			selectmode=EXTENDED,
			height=20, width=40)
		self.listbox2.grid(row=0, column=3, sticky=NSEW)
		self.xScroll2["command"] = self.listbox2.xview
		self.yScroll2["command"] = self.listbox2.yview

		def __lbox2callback(event):
			self.__listSelectionToLeft()
			self.__testUpDownState()	# Really want to get at the listbox2 onchange event
		self.listbox2.bind("<Double-Button-1>", __lbox2callback)

		def __lbox2ClickCallback(event):
			self.listbox1.selection_clear(0,END)
			self.__testUpDownState()
			if self.listbox2.size() > 0:
				self.__setSelButtonsActive(1, 0)
			else:
				self.__setSelButtonsActive(0, 0)
		self.listbox2.bind("<Button-1>", __lbox2ClickCallback)
		self.listbox2.bind("<FocusIn>", __lbox2ClickCallback)
		self.listbox2.bind("<FocusOut>", __lbox2ClickCallback)

		def __lbox2ButtonUpCallback(event):
			self.__testUpDownState()	# Catches a mouse drag highlight by testing on button release
		self.listbox2.bind("<ButtonRelease>", __lbox2ButtonUpCallback)

		# now draw the bottom controls frame...
		self.controls_frame = Frame(self)
		self.controls_frame.grid(row=1, column=0, sticky=EW)

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
		self.status_frame.grid(row=2, column=0, sticky=E+W)
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
		pages = IntType(value)					# Get whole part of number
		value = value - pages					# Remove whole number part
		if value > 0:							# Test remainder
			pages = pages + 1    				# Had remainder so add a page
		self.status0['text'] = "Fonts available: %s    " % (available + selected)
		self.status1['text'] = "Fonts selected: %s    " % selected
		self.status2['text'] = "Sheets required: %s    " % pages

	def __listSelectionToRight(self):
		toMoveRight = ListType(self.listbox1.curselection())
		self.listbox1.selection_clear(0,END)
		toMoveRight.reverse()	# reverse list so we delete from bottom of listbox first
		templist = []
		for i in toMoveRight:
			templist.insert(0,self.listbox1.get(i))	# gets the actual strings (reverse again)
			self.listbox1.delete(i)
		for j in templist:
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
		fontlist = ListType(self.listbox1.get(0, END))		# Copy contents to a list type
		self.listbox1.delete(0, END)						# Remove all contents
		fontlist.sort()										# Use sort method of list
		for j in fontlist:
			self.listbox1.insert(END, j)					# Replace with sorted version
		self.__setButtonsState()
		self.__setSelButtonsActive(0, 0)
		self.statusbarUpdate()

	def __listAllToRight(self):
		fontlist = self.listbox1.get(0, END)	# Get each font name into a list
		for i in fontlist:
			self.listbox2.insert(END, i)		# Copy each one
		self.listbox1.delete(0, END)			# All done so empty the left listbox
		self.__setButtonsState()
		self.__setSelButtonsActive(0, 0)
		self.statusbarUpdate()

	def __listAllToLeft(self):
		# This quick way just clears both sides and reloads left listbox in correct order from scratch
		self.listbox1.delete(0, END)
		fontlist = GetFontNames()
		fontlist.sort()
		for i in fontlist:
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
		# This function is intended to be used with one item selected
		selection = self.listbox2.curselection()	# Returns tuple, we only want one item
		indexid = IntType(selection[0])				# Get the first in the list, convert to integer type
		if indexid > 0:
			fontstring = self.listbox2.get(indexid)
			self.listbox2.delete(indexid)
			newpos = indexid - 1
			self.listbox2.selection_clear(0, END)
			self.listbox2.insert(newpos, fontstring)
			self.listbox2.see(newpos - 10)			# Scrolls listbox automatically into view if req.
			self.listbox2.selection_set(newpos)
			self.__testUpDownState()

	def __itemDown(self):
		# This function is intended to be used with one item selected
		limit = self.listbox2.size()
		selection = self.listbox2.curselection()
		indexid = IntType(selection[0])
		if indexid < limit - 1:
			fontstring = self.listbox2.get(indexid)
			self.listbox2.delete(indexid)
			newpos = indexid + 1
			self.listbox2.selection_clear(0, END)
			self.listbox2.insert(newpos, fontstring)
			self.listbox2.see(newpos + 10)
			self.listbox2.selection_set(newpos)
			self.__testUpDownState()

	def __setUpDownActive(self, up, down):
	# This function just sets the buttons active or inactive
	# See testUpDown() for actual evaluation
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
			elif position == self.listbox2.size() - 1:					# Up only
				self.__setUpDownActive(1, 0)
			# If not one or more from the top then gray up button.
			elif position == 0:											# Down only
				self.__setUpDownActive(0, 1)
		else:
			self.__setUpDownActive(0, 0)

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


root = Tk()
app = Application(root)
app.master.resizable(0, 0)
app.master.title("(v0.6.1) Select fonts to use")
# get and set the initial paper size to match default radiobutton selection
setPageGeometries()
# now get a list of all the fonts...
fontlist = GetFontNames()
fontlist.sort()
# and put the list in the GUI listbox...
for i in fontlist:
	app.listbox1.insert(END, i)
setFixedFont()
setPropFont()
app.statusbarUpdate()
# now show the fonts list and wait for user to make a choice...
app.mainloop()
