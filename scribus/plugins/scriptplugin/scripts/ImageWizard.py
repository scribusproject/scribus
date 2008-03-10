#!/usr/bin/env python
# -*- coding: utf-8 -*-
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

try:
    # I wish PyQt installed everywhere :-/
    from Tkinter import *
    from tkFont import Font
except ImportError:
    print "This script requires Python's Tkinter properly installed."
    messageBox('Script failed',
               'This script requires Python\'s Tkinter properly installed.',
               ICON_CRITICAL)
    sys.exit(1)



class TkImageAlignmentWizard(Frame):
    """ GUI interface for aligning an image in a frame"""
    
    selectedList = []
    objList = []
    imgList = []
    nbrSelected = 0
    nbrObjects = 0
    nbrImages = 0
    regroupObjects = False
    innerFrame = ""
    outerFrame = ""
    
    copyXPos = 0.0
    copyYPos = 0.0
    copyWidth = 0.0
    copyHeight = 0.0
    copyRotation = 0.0

    def __init__(self, master=None):
        """ Setup the dialog """
        # refernce to the localization dictionary
        self.key = 'English'
        Frame.__init__(self, master)
        self.master.resizable(0, 0)
        self.master.title('Image Wizard Advanced')
        
        #define widgets
        #define framed areas
        self.grid()
        self.select_frame = Frame(self, bd=1, relief=RIDGE)
        self.load_frame = Frame(self, bd=1, relief=RIDGE)
        self.copy_frame = Frame(self, bd=1, relief=RIDGE)
        self.align_frame = Frame(self, bd=1, relief=RIDGE)
        self.scale_frame = Frame(self, bd=1, relief=RIDGE)
        self.flip_frame = Frame(self, bd=1, relief=RIDGE)
        self.move_frame = Frame(self, bd=1, relief=RIDGE)
        
        #selection options - what images can be operated on
        self.selectLabel = Label(self.select_frame, text='What images will be modified:')
        self.selectFrameLabel1 = Label(self.select_frame, text='The options below are used for a set of 2 images that are')
        self.selectFrameLabel2 = Label(self.select_frame, text='grouped, where one image is a decorative frame, and the')
        self.selectFrameLabel3 = Label(self.select_frame, text='other image is a photo or picture.')
        self.selectVar = StringVar()
        self.selectRadio1 = Radiobutton(self.select_frame, text='A Single Image Only', variable=self.selectVar, value="Single")
        self.selectRadio2 = Radiobutton(self.select_frame, text='Multiple Images', variable=self.selectVar, value="Multiple")
        self.selectRadio3 = Radiobutton(self.select_frame, text='A Group of Two Frames - Inner Frame Only', variable=self.selectVar, value="InnerFrame2")
        self.selectRadio4 = Radiobutton(self.select_frame, text='A Group of Two Frames - Outer Frame Only', variable=self.selectVar, value="OuterFrame2")
        self.selectRadio1.select()
        
        #load options
        self.loadButton = Button(self.load_frame, text='Load image for selected frame(s)...', command=self.loadImage)
        
        #copy options - copy and paste another object's size, location, and rotation
        self.copyButton = Button(self.copy_frame, text='Copy from image', command=self.copyInformation)
        self.copyPasteButton = Button(self.copy_frame, text='Paste to image(s)', command=self.pasteInformation)
        self.copyLabel = Label(self.copy_frame, text='Copy/Paste the Location/Size/Rotation settings from image to another:')
        self.copyLabel2 = Label(self.copy_frame, text='(A Group of Two Image Frames is treated together, but without rotation.)')
        self.copyArrowLabel1 = Label(self.copy_frame, text='v')
        self.copyArrowLabel2 = Label(self.copy_frame, text='v')
        self.copyInfoVar = StringVar()
        self.copyInfoLabel = Label(self.copy_frame, textvariable=self.copyInfoVar)
        self.setCopyInfo()
        
        # alignment options
        self.alignLabel = Label(self.align_frame, text='Align Image')
        self.alignVar = StringVar()
        self.alignRadio1 = Radiobutton(self.align_frame, text='', variable=self.alignVar, value="TL")
        self.alignRadio2 = Radiobutton(self.align_frame, text='', variable=self.alignVar, value="TC")
        self.alignRadio3 = Radiobutton(self.align_frame, text='', variable=self.alignVar, value="TR")
        self.alignRadio4 = Radiobutton(self.align_frame, text='', variable=self.alignVar, value="ML")
        self.alignRadio5 = Radiobutton(self.align_frame, text='', variable=self.alignVar, value="MC")
        self.alignRadio6 = Radiobutton(self.align_frame, text='', variable=self.alignVar, value="MR")
        self.alignRadio7 = Radiobutton(self.align_frame, text='', variable=self.alignVar, value="BL")
        self.alignRadio8 = Radiobutton(self.align_frame, text='', variable=self.alignVar, value="BC")
        self.alignRadio9 = Radiobutton(self.align_frame, text='', variable=self.alignVar, value="BR")
        self.alignButton = Button(self.align_frame, text='Align', command=self.alignImage)
        self.alignRadio5.select()
        
        # scaling options
        self.scaleLabel = Label(self.scale_frame, text='Scale Image:')
        self.scaleToFillButton = Button(self.scale_frame, text='Scale to Fill Frame', command=self.scaleImageToFill)
        self.scaleToFitButton = Button(self.scale_frame, text='Scale to Fit Frame', command=self.scaleImageToFit)
        self.scaleToFitProportionallyButton = Button(self.scale_frame, text='Scale to Fit Proportionally', command=self.scaleImageToFitProportionally)
        self.scalePercentVar = StringVar()
        self.scalePercentLabel1 = Label(self.scale_frame, text='Scale Image to:')
        self.scalePercentLabel2 = Label(self.scale_frame, text='%')
        self.scalePercentEntry = Entry(self.scale_frame, textvariable=self.scalePercentVar, width=8)
        self.scalePercentButton = Button(self.scale_frame, text='Scale', command=self.scaleImageToPercent)
        
        #flip options
        self.flipLabel = Label(self.flip_frame, text='Flip Image:')
        self.flipHorizontallyButton = Button(self.flip_frame, text='< Flip Horizontally >', command=self.flipHorizontally)
        self.flipVerticallyButton = Button(self.flip_frame, text='^\nFlip Vertically\nv', command=self.flipVertically)
        self.flipUndoButton = Button(self.flip_frame, text='No Flipping', command=self.flipUndo)
        
        #move options
        self.moveVar = StringVar()
        self.moveRadio1 = Radiobutton(self.move_frame, variable=self.moveVar, value="Image", text='Move Image')
        self.moveRadio2 = Radiobutton(self.move_frame, variable=self.moveVar, value="Frame", text='Move Frame')
        self.moveUpButton = Button(self.move_frame, text='^', command=self.moveUp)
        self.moveUpLotsButton = Button(self.move_frame, text='^^', command=self.moveUpLots)
        self.moveDownButton = Button(self.move_frame, text='v', command=self.moveDown)
        self.moveDownLotsButton = Button(self.move_frame, text='vv', command=self.moveDownLots)
        self.moveLeftButton = Button(self.move_frame, text='<', command=self.moveLeft)
        self.moveLeftLotsButton = Button(self.move_frame, text='<<', command=self.moveLeftLots)
        self.moveRightButton = Button(self.move_frame, text='>', command=self.moveRight)
        self.moveRightLotsButton = Button(self.move_frame, text='>>', command=self.moveRightLots)
        self.moveRadio1.select()
        
        # closing/running
        self.doneButton = Button(self, text="Done", command=self.quit)
        
        #status label
        self.statusVar = StringVar()
        self.statusLabel = Label(self, textvariable=self.statusVar)
        self.statusVar.set('Status:')
        
        #make select layout
        currRow = 0
        self.selectLabel.grid(column=0, row=currRow)
        currRow += 1
        self.selectRadio1.grid(column=0, row=currRow, sticky=W)
        currRow += 1
        self.selectRadio2.grid(column=0, row=currRow, sticky=W)
        currRow += 1
        self.selectFrameLabel1.grid(column=0, row=currRow, sticky=W)
        currRow += 1
        self.selectFrameLabel2.grid(column=0, row=currRow, sticky=W)
        currRow += 1
        self.selectFrameLabel3.grid(column=0, row=currRow, sticky=W)
        currRow += 1
        self.selectRadio3.grid(column=0, row=currRow, sticky=W)
        currRow += 1
        self.selectRadio4.grid(column=0, row=currRow, sticky=W)
        
        #make load layout
        currRow = 0
        self.loadButton.grid(column=0, row=currRow)        
        
        #make copy layout
        currRow = 0
        self.copyLabel.grid(column=0, row=currRow)
        currRow += 1
        self.copyLabel2.grid(column=0, row=currRow)
        currRow += 1
        self.copyButton.grid(column=0, row=currRow)
        currRow += 1
        self.copyArrowLabel1.grid(column=0, row=currRow)
        currRow += 1
        self.copyInfoLabel.grid(column=0, row=currRow)
        currRow += 1
        self.copyArrowLabel2.grid(column=0, row=currRow)
        currRow += 1
        self.copyPasteButton.grid(column=0, row=currRow)

        # make align layout
        currRow = 0
        self.alignLabel.grid(column=0, row=currRow, columnspan=3)
        currRow += 1
        self.alignRadio1.grid(column=0, row=currRow)
        self.alignRadio2.grid(column=1, row=currRow)
        self.alignRadio3.grid(column=2, row=currRow)
        currRow += 1
        self.alignRadio4.grid(column=0, row=currRow)
        self.alignRadio5.grid(column=1, row=currRow)
        self.alignRadio6.grid(column=2, row=currRow)
        currRow += 1
        self.alignRadio7.grid(column=0, row=currRow)
        self.alignRadio8.grid(column=1, row=currRow)
        self.alignRadio9.grid(column=2, row=currRow)
        currRow += 1
        self.alignButton.grid(column=0, row=currRow, columnspan=3)
        self.doneButton.grid(column=3, row=currRow, columnspan=3)
        
        #make scale layout
        currRow = 0
        self.scaleLabel.grid(column=0, row=currRow, columnspan=3)
        currRow += 1
        self.scaleToFillButton.grid(column=0, row=currRow, columnspan=3)
        currRow += 1
        self.scaleToFitButton.grid(column=0, row=currRow, columnspan=3)
        currRow += 1
        self.scaleToFitProportionallyButton.grid(column=0, row=currRow, columnspan=3)
        currRow += 1
        self.scalePercentLabel1.grid(column=0, row=currRow, columnspan=3)
        currRow += 1
        self.scalePercentEntry.grid(column=0, row=currRow)
        self.scalePercentLabel2.grid(column=1, row=currRow)
        self.scalePercentButton.grid(column=2, row=currRow)
        
        #make flip layout
        currRow = 0
        self.flipLabel.grid(column=0, row=currRow)
        currRow += 1
        self.flipHorizontallyButton.grid(column=0, row=currRow)
        currRow += 1
        self.flipVerticallyButton.grid(column=0, row=currRow)
        currRow += 1
        self.flipUndoButton.grid(column=0, row=currRow)
        
        #make move layout
        currRow = 0
        self.moveRadio1.grid(column=0, row=currRow, columnspan=5)
        currRow += 1
        self.moveRadio2.grid(column=0, row=currRow, columnspan=5)
        currRow += 1
        self.moveUpLotsButton.grid(column=2, row=currRow)
        currRow += 1
        self.moveUpButton.grid(column=2, row=currRow)
        currRow += 1
        self.moveLeftLotsButton.grid(column=0, row=currRow)
        self.moveLeftButton.grid(column=1, row=currRow)
        self.moveRightButton.grid(column=3, row=currRow)
        self.moveRightLotsButton.grid(column=4, row=currRow)
        currRow += 1
        self.moveDownButton.grid(column=2, row=currRow)
        currRow += 1
        self.moveDownLotsButton.grid(column=2, row=currRow)

        
        #make overall layout
        self.grid()
        currRow = 0
        self.select_frame.grid(row=currRow, column=0, columnspan=2, sticky=NSEW)
        currRow += 1
        self.load_frame.grid(row=currRow, column=0, columnspan=2, sticky=NSEW)
        currRow += 1
        self.copy_frame.grid(row=currRow, column=0, columnspan=2, sticky=NSEW)
        currRow += 1
        self.scale_frame.grid(row=currRow, column=0, sticky=NSEW)
        self.flip_frame.grid(row=currRow, column=1, sticky=NSEW)
        currRow += 1
        self.align_frame.grid(row=currRow, column=0, sticky=NSEW)
        self.move_frame.grid(row=currRow, column=1, sticky=NSEW)
        currRow += 1
        self.doneButton.grid(row=currRow, column=0, columnspan=2)
        currRow += 1
        self.statusLabel.grid(row=currRow, column=0, columnspan=2, stick=W)

    def startImageProcess(self):
        self.selectedList = []
        self.objList = []
        self.imgList = []
        self.nbrSelected = 0
        self.nbrObjects = 0
        self.nbrImages = 0
        self.regroupObjects = False
        self.innerFrame = ""
        self.outerFrame = ""
    
        #get number of selected items
        self.nbrSelected = selectionCount()
        
        #get list of selected items
        for i in range(self.nbrSelected):
            self.selectedList.append(getSelectedObject(i))
        
        #get list of selected images
        for i in range(self.nbrSelected):
            obj = self.selectedList[i]
            if(getProperty(obj, "itemType") == 2):
                self.imgList.append(obj)
                self.nbrImages += 1
        
        #get selection type: Single, Multiple, InnerFrame2, or OuterFrame2
        #then determine whether the current selection matches that type or not
        #if it does, then proceed, otherwise stop processing
        selectType = self.selectVar.get()
        continueProcessing = False
        if selectType == "Single" and self.nbrImages == 1:
            continueProcessing = True
        elif selectType == "Multiple" and self.nbrImages > 0:
            continueProcessing = True
        elif selectType == "InnerFrame2" and self.nbrImages == 2:
            continueProcessing = True
        elif selectType == "OuterFrame2" and self.nbrImages == 2:
            continueProcessing = True
        
        #if we're not supposed to continueProcessing, return to end the script
        if not continueProcessing:
            return
        
        #build objList, putting all the images that should be modified into it
        if selectType == "Single" or selectType == "Multiple":
            for i in range(self.nbrImages):
                obj = self.imgList[i]
                self.objList.append(obj)
                self.nbrObjects += 1
        elif selectType == "InnerFrame2" or selectType == "OuterFrame2":
            #The InnerFrame2 selection tries to determine the smallest frame, the OuterFrame selection tries to determine the largest frame.
            #The single frame chosen will be the only one that has an image operation performed on it
            firstObject = True
            selectedObj = ""
            otherObj = ""
            w = 0.0
            h = 0.0
            for i in range(2):
                obj = self.imgList[i]
                #try to ungroup the images if they are in a 2-Image group, it will be re-grouped in endImageProcess
                unGroupObject(obj)
                self.regroupObjects = True
                if firstObject:
                    firstObject = False
                    w = getProperty(obj, "width")
                    h = getProperty(obj, "height")
                    selectedObj = obj
                else:
                    tempw = getProperty(obj, "width")
                    temph = getProperty(obj, "height")
                    otherObj = obj
                    if selectType == "InnerFrame2" and (tempw*temph) <= (w*h):
                        w = tempw
                        h = temph
                        otherObj = selectedObj
                        selectedObj = obj
                    elif selectType == "OuterFrame2" and (tempw*temph) > (w*h):
                        w = tempw
                        h = temph
                        otherObj = selectedObj
                        selectedObj = obj
            if selectedObj != "":
                self.objList.append(selectedObj)
                self.nbrObjects = 1
                if selectType == "InnerFrame2":
                    self.innerFrame = selectedObj
                    self.outerFrame = otherObj
                elif selectType == "OuterFrame2":
                    self.outerFrame = selectedObj
                    self.innerFrame = otherObj
        

    def endImageProcess(self):
        #this tries to group objects back together if regroupObjects is True, otherwise it doesn't do anything
        if self.regroupObjects:
            groupObjects(self.imgList)
    
    def alignImage(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            for i in range(self.nbrObjects):
                try:
                    obj = self.objList[i]
                    frameW = getProperty(obj, "width")
                    frameH = getProperty(obj, "height")
                    saveScaleX = getProperty(obj, "imageXScale")
                    saveScaleY = getProperty(obj, "imageYScale")
                    setScaleImageToFrame(True, False, obj)
                    fullScaleX = getProperty(obj, "imageXScale")
                    fullScaleY = getProperty(obj, "imageYScale")
                    setScaleImageToFrame(False, False, obj)
                    #scaleImage(saveScaleX, saveScaleY, obj)
                    #don't use scaleImage(...) because it seems to scale all selected items by the same amount
                    setProperty(obj, "imageXScale", saveScaleX)
                    setProperty(obj, "imageYScale", saveScaleY)
                    imageW = frameW * (saveScaleX / fullScaleX)
                    imageH = frameH * (saveScaleY / fullScaleY)
                    imageX = 0.0
                    imageY = 0.0
                    
                    if self.alignVar.get()[0] == "T":
                        imageY = 0.0
                    elif self.alignVar.get()[0] == "M":
                        imageY = (frameH - imageH) / 2.0 / saveScaleY
                    elif self.alignVar.get()[0] == "B":
                        imageY = (frameH - imageH) / saveScaleY
                    if self.alignVar.get()[1] == "L":
                        imageX = 0.0
                    elif self.alignVar.get()[1] == "C":
                        imageX = (frameW - imageW) / 2.0 / saveScaleX
                    elif self.alignVar.get()[1] == "R":
                        imageX = (frameW - imageW) / saveScaleX

                    setProperty(obj, "imageXOffset", imageX)
                    setProperty(obj, "imageYOffset", imageY)
                    docChanged(1)
                    setRedraw(True)
                except:
                    nothing = "nothing"
        self.endImageProcess()

    def scaleImageToFill(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            for i in range(self.nbrObjects):
                try:
                    obj = self.objList[i]
                    setScaleImageToFrame(True, False, obj)
                    scaleX, scaleY = getImageScale(obj)
                    setScaleImageToFrame(False, False, obj)
                    if scaleX > scaleY:
                        scale = scaleX
                        #scaleImage(scale, scale, obj)
                        setProperty(obj, "imageXScale", scale)
                        setProperty(obj, "imageYScale", scale)
                    elif scaleY > scaleX:
                        scale = scaleY
                        #scaleImage(scale, scale, obj)
                        setProperty(obj, "imageXScale", scale)
                        setProperty(obj, "imageYScale", scale)
                    docChanged(1)
                    setRedraw(True)
                except:
                    nothing = "nothing"
        self.endImageProcess()

    def scaleImageToFit(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            for i in range(self.nbrObjects):
                try:
                    obj = self.objList[i]
                    setScaleImageToFrame(True, False, obj)
                    setScaleImageToFrame(False, False, obj)
                    docChanged(1)
                    setRedraw(True)
                except:
                    nothing = "nothing"
        self.endImageProcess()

    def scaleImageToFitProportionally(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            for i in range(self.nbrObjects):
                try:
                    obj = self.objList[i]
                    setScaleImageToFrame(True, True, obj)
                    setScaleImageToFrame(False, False, obj)
                    docChanged(1)
                    setRedraw(True)
                except:
                    nothing = "nothing"
        self.endImageProcess()
    
    def scaleImageToPercent(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            for i in range(self.nbrObjects):
                try:
                    tempScale = float(self.scalePercentVar.get())
                    tempScale = tempScale / 100.0
                    obj = self.objList[i]
                    setScaleImageToFrame(False, False, obj)
                    #scaleImage(tempScale, tempScale, obj)
                    #don't use scaleImage(...) because it seems to scale all selected items
                    setProperty(obj, "imageXScale", tempScale)
                    setProperty(obj, "imageYScale", tempScale)
                    setRedraw(True)
                    docChanged(1)
                except:
                    nothing = "nothing"
        self.endImageProcess()

    def flipHorizontally(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            for i in range(self.nbrObjects):
                try:
                    obj = self.objList[i]
                    setProperty(obj, "m_ImageIsFlippedH", not getProperty(obj, "m_ImageIsFlippedH"))
                    self.forceRefresh(obj)
                except:
                    nothing = "nothing"
        self.endImageProcess()
                
    def flipVertically(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            for i in range(self.nbrObjects):
                try:
                    obj = self.objList[i]
                    setProperty(obj, "m_ImageIsFlippedV", not getProperty(obj, "m_ImageIsFlippedV"))
                    self.forceRefresh(obj)
                except:
                    nothing = "nothing"
        self.endImageProcess()
                
    def flipUndo(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            for i in range(self.nbrObjects):
                try:
                    obj = self.objList[i]
                    setProperty(obj, "m_ImageIsFlippedH", False)
                    setProperty(obj, "m_ImageIsFlippedV", False)
                    self.forceRefresh(obj)
                except:
                    nothing = "nothing"
        self.endImageProcess()
                
    def moveUp(self):
        self.moveImage(0, -10.0)
    
    def moveUpLots(self):
        self.moveImage(0, -100.0)
        
    def moveDown(self):
        self.moveImage(0, 10.0)
        
    def moveDownLots(self):
        self.moveImage(0, 100.0)
        
    def moveLeft(self):
        self.moveImage(-10.0, 0)
        
    def moveLeftLots(self):
        self.moveImage(-100.0, 0)
        
    def moveRight(self):
        self.moveImage(10.0, 0)
    
    def moveRightLots(self):
        self.moveImage(100.0, 0)
        
    def moveImage(self, dx, dy):
        self.startImageProcess()
        if self.nbrObjects > 0:
            for i in range(self.nbrObjects):
                try:
                    obj = self.objList[i]
                    #based on which radio button is selected, we either move the image frame on the canvas, or move the image in its frame
                    if str(self.moveVar.get()) == "Frame":
                        #moveObject(dx, dy, obj)
                        setProperty(obj, "xPos", getProperty(obj, "xPos") + dx)
                        setProperty(obj, "yPos", getProperty(obj, "yPos") + dy)
                        self.forceRefresh(obj)
                    else:
                        #if the image is flipped, we need to reverse the delta-X or delta-Y in order to keep moving
                        #the image in the direction that the user expects
                        if getProperty(obj, "m_ImageIsFlippedH"):
                            dx = dx * -1
                        if getProperty(obj, "m_ImageIsFlippedV"):
                            dy = dy * -1
                        setProperty(obj, "imageXOffset", getProperty(obj, "imageXOffset") + dx)
                        setProperty(obj, "imageYOffset", getProperty(obj, "imageYOffset") + dy)
                        self.forceRefresh(obj)
                    docChanged(1)
                    setRedraw(True)
                except:
                    nothing = "nothing"
        self.endImageProcess()
        
    def loadImage(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            imageFileName = fileDialog("Load Image", "All Supported Formats (*.gif *.GIF *.jpg *.jpeg *.JPG *.JPEG *.png *.PNG *.xpm *.XPM *.tif *.tiff *.TIF *.TIFF *.psd *.PSD *.epsi *.EPSI *.eps *.EPS *.pdf *.PDF)", "", True, False, False)
            if imageFileName != "":
                for i in range(self.nbrObjects):
                    try:
                        obj = self.objList[i]
                        loadImage(imageFileName, obj)
                    except:
                        nothing = "nothing"
        self.endImageProcess()

    def copyInformation(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            obj = self.objList[0]

            #if the selected frame is part of a two-frame group, select the outer frame instead
            selectType = self.selectVar.get()
            if selectType == "InnerFrame2" or selectType == "OuterFrame2":
                obj = self.outerFrame
                
            try:
                self.copyXPos = getProperty(obj, "xPos")
                self.copyYPos = getProperty(obj, "yPos")
                self.copyWidth = getProperty(obj, "width")
                self.copyHeight = getProperty(obj, "height")
                self.copyRotation = getProperty(obj, "rotation")
                self.setCopyInfo()
            except:
                nothing = "nothing"
        self.endImageProcess()
    
    def pasteInformation(self):
        self.startImageProcess()
        if self.nbrObjects > 0:
            unitType = getUnit()
            selectType = self.selectVar.get()
            if selectType == "InnerFrame2" or selectType == "OuterFrame2":
                #get values to calculate inner frame changes
                xPercent = self.copyWidth / getProperty(self.outerFrame, "width")
                yPercent = self.copyHeight / getProperty(self.outerFrame, "height")
                xOffset = getProperty(self.innerFrame, "xPos") - getProperty(self.outerFrame, "xPos")
                yOffset = getProperty(self.innerFrame, "yPos") - getProperty(self.outerFrame, "yPos")
                xOffset *= xPercent
                yOffset *= yPercent
                xSize = getProperty(self.innerFrame, "width") * xPercent
                ySize = getProperty(self.innerFrame, "height") * yPercent
                
                #set outer frame position
                #since the sizeObject function expects its size units in the unit-type of the current document, we have to convert our points to the document's current units
                sizeObject(self.pointsToUnits(self.copyWidth, unitType), self.pointsToUnits(self.copyHeight, unitType), self.outerFrame)
                setProperty(self.outerFrame, "xPos", self.copyXPos)
                setProperty(self.outerFrame, "yPos", self.copyYPos)
                
                #set inner frame position
                #since the sizeObject function expects its size units in the unit-type of the current document, we have to convert our points to the document's current units
                sizeObject(self.pointsToUnits(xSize, unitType), self.pointsToUnits(ySize, unitType), self.innerFrame)
                setProperty(self.innerFrame, "xPos", self.copyXPos+xOffset)
                setProperty(self.innerFrame, "yPos", self.copyYPos+yOffset)
            else:
                for i in range(self.nbrObjects):
                    try:
                        obj = self.objList[i]
                        #since the sizeObject function expects its size units in the unit-type of the current document, we have to convert our points to the document's current units
                        sizeObject(self.pointsToUnits(self.copyWidth, unitType), self.pointsToUnits(self.copyHeight, unitType), obj)
                        setProperty(obj, "xPos", self.copyXPos)
                        setProperty(obj, "yPos", self.copyYPos)
                        rotateObjectAbs(self.copyRotation*-1.0, obj)
                    except:
                        nothing = "nothing"
        self.endImageProcess()
                    
    def setCopyInfo(self):
        self.copyInfoVar.set(str(self.copyWidth) + 'w x ' + str(self.copyHeight) + 'h  @  (' + str(self.copyXPos) + 'x, ' + str(self.copyYPos) + 'y),  ' + str(self.copyRotation * -1.0) + ' deg.')

                
    def forceRefresh(self, obj):
        #changing certain properties doesn't actually redraw the image until the script is terminated
        #to fix this, we move the image and then move it back to the same spot, thus triggering a refresh of the image
        moveObject(1, 0, obj)
        moveObject(-1, 0, obj)
        docChanged(1)
        setRedraw(True)
        
    def setStatus(self, string):
        self.statusVar.set("Status: " + str(string))
        
    def unitsToPoints(self, units, unitType):
        return units / self.getUnitValue(unitType)

    def pointsToUnits(self, points, unitType):
        return points * self.getUnitValue(unitType)
    
    def getUnitValue(self, unitType):
        if unitType == UNIT_POINTS:
            return pt
        elif unitType == UNIT_MILLIMETERS:
            return mm
        elif unitType == UNIT_INCHES:
            return inch
        elif unitType == UNIT_PICAS:
            return p
        elif unitType == UNIT_CENTIMETRES:
            return cm
        elif unitType == UNIT_CICERO:
            return c

    def quit(self):
        self.master.destroy()

                
def main():
    """ Application/Dialog loop with Scribus sauce around """
    try:
        if haveDoc():
            root = Tk()
            app = TkImageAlignmentWizard(root)
            root.mainloop()
    finally:
        if haveDoc():
            redrawAll()

if __name__ == '__main__':
    main()