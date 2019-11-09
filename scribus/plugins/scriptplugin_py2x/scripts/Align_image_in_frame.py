#!/usr/bin/python
# Align_image_in_frame.py
# This version 2014.04.19
"""
This script will align an image inside a frame to one of 9 possible positions: 
Top left, top center, top right; middle left, middle center, middle right; 
or bottom left, bottom center, bottom right.
 
USAGE
Select one or more image frames. Run the script, which asks for your alignment 
choice (all selected frames will need to have the same alignment). Choose the 
position in the dialog radio button grid, click Align. Image(s) are aligned, and script quits.
 
Note
There is minimal error checking, in particular no checking for frame type.

See the wiki page for further info:
wiki.scribus.net/canvas/Align_an_Image_in_its_Frame
 
"""
 
import scribus
 
try:
    from Tkinter import *
    from tkFont import Font
except ImportError:
    print "This script requires Python's Tkinter properly installed."
    scribus.messageBox('Script failed',
               'This script requires Python\'s Tkinter properly installed.',
               scribus.ICON_CRITICAL)
    sys.exit(1)
 
 
 
class TkImageAlignmentWizard(Frame):
    """ GUI interface for aligning an image in a frame"""
 
    def __init__(self, master=None):
        """ Setup the dialog """
        # refernce to the localization dictionary
        self.key = 'English'
        Frame.__init__(self, master)
        self.grid()
        self.master.geometry('120x120-80+40')
        self.master.title('Scribus Image Alignment Wizard')
        #define widgets
        # alignment options
        self.alignLabel = Label(self, text='Select alignment:')
        self.alignVar = StringVar()
        self.alignRadio1 = Radiobutton(self, text='', variable=self.alignVar, value="TL")
        self.alignRadio2 = Radiobutton(self, text='', variable=self.alignVar, value="TC")
        self.alignRadio3 = Radiobutton(self, text='', variable=self.alignVar, value="TR")
        self.alignRadio4 = Radiobutton(self, text='', variable=self.alignVar, value="ML")
        self.alignRadio5 = Radiobutton(self, text='', variable=self.alignVar, value="MC")
        self.alignRadio6 = Radiobutton(self, text='', variable=self.alignVar, value="MR")
        self.alignRadio7 = Radiobutton(self, text='', variable=self.alignVar, value="BL")
        self.alignRadio8 = Radiobutton(self, text='', variable=self.alignVar, value="BC")
        self.alignRadio9 = Radiobutton(self, text='', variable=self.alignVar, value="BR")
        self.alignButton = Button(self, text='Align', command=self.alignImage)

        # setup values
        self.alignRadio5.select()
        # make layout
        self.columnconfigure(0, pad=0)
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
#        self.doneButton.grid(column=3, row=currRow, columnspan=3)
 
    def alignImage(self):
        if scribus.haveDoc():
	    restore_units = scribus.getUnit()   # since there is an issue with units other than points,
	    scribus.setUnit(0)			# we switch to points then restore later.
            nbrSelected = scribus.selectionCount()
            objList = []
            for i in range(nbrSelected):
                objList.append(scribus.getSelectedObject(i))
            scribus.deselectAll()
            for i in range(nbrSelected):
                try:
                    obj = objList[i]
                    scribus.selectObject(obj)
                    frameW, frameH = scribus.getSize(obj)
                    saveScaleX, saveScaleY = scribus.getImageScale(obj)
                    scribus.setScaleImageToFrame(1, 0, obj)
                    fullScaleX, fullScaleY = scribus.getImageScale(obj)
                    scribus.setScaleImageToFrame(0, 0, obj)
                    scribus.setImageScale(saveScaleX, saveScaleY, obj)
                    imageW = frameW * (saveScaleX / fullScaleX)
                    imageH = frameH * (saveScaleY / fullScaleY)
                    imageX = 0.0
                    imageY = 0.0
 
                    if self.alignVar.get()[0] == "T":
                        imageY = 0.0
                    elif self.alignVar.get()[0] == "M":
                        imageY = (frameH - imageH) / 2.0
                    elif self.alignVar.get()[0] == "B":
                        imageY = (frameH - imageH)
                    if self.alignVar.get()[1] == "L":
                        imageX = 0.0
                    elif self.alignVar.get()[1] == "C":
                        imageX = (frameW - imageW) / 2.0
                    elif self.alignVar.get()[1] == "R":
                        imageX = (frameW - imageW)
 
                    scribus.setImageOffset(imageX, imageY, obj)
                    scribus.docChanged(1)
                    scribus.setRedraw(True)
                    scribus.deselectAll()
                except:
                    nothing = "nothing"
	    scribus.setUnit(restore_units)
	    
	    self.master.destroy()
 
 
def main():
    """ Application/Dialog loop with Scribus sauce around """
    try:
        root = Tk()
        app = TkImageAlignmentWizard(root)
        root.mainloop()
    finally:
        if scribus.haveDoc():
            scribus.redrawAll()
 
if __name__ == '__main__':
    main()