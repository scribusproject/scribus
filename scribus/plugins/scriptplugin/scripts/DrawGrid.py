#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""

AUTHOR

(C) 2006 Rüdiger Härtel <r_haertel [at] gmx [dot] de>

Craig Bradney, Scribus Team
10/3/08: Added to Scribus 1.3.3.12svn distribution "as was" from Scribus wiki for bug #6826, script is GPLd

This program is free software; you can redistribute it and/or modify
it under the terms of the  GPL, v2 (GNU General Public License as published by
the Free Software Foundation, version 2 of the License), or any later version.
See the Scribus Copyright page in the Help Browser for further informaton 
about GPL, v2.

SYNOPSIS

This script draws a grid-like structure, specifically in this case a grid formed
from individual horizontal and vertical lines.

REQUIREMENTS

You must run this from Scribus, and have a document open. You must also have tkinter
installed on your system.

USAGE

You are presented with a Tk dialog in which there are default values for X-Pos, Y-Pos 
of the left upper corner, the overall width and height of the grid, and the x and y 
spacing (X-step and Y-step). 

LIMITATIONS

Since the same defaults are used regardless of page units, for some units these may 
be unrealistic. The numbers are defined as integers, so decimals cannot be entered - 
the script stalls in this case. The grid is made up of individual lines, so be 
sure to group them to move the grid en bloc.


"""

import sys

try:
    import scribus
except ImportError,err:
    print 'This Python script is written for the Scribus scripting interface.'
    print 'It can only be run from within Scribus.'
    sys.exit(1)

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


def grid(x,y,width,height,xstep,ystep,color):
    """ """
    xend = x + width
    yend = y + height
    
    for _x in range(x,xend+1,xstep):
         line = scribus.createLine(_x,y,_x,yend)
         scribus.setLineColor(color, line)
    
    for _y in range(y,yend+1,ystep):
         line = scribus.createLine(x,_y,xend,_y)
         scribus.setLineColor(color, line)


class TkGrid(Frame):
    """ GUI interface for Scribus calendar wizard.
        It's ugly and very simple. I can say I hate Tkinter :-/"""
    
    def __init__(self, master=None):
        """ Setup the dialog """
        # reference to the localization dictionary
        self.key = 'default'
        Frame.__init__(self, master)
        self.grid()
        self.master.title('Scribus Grid Wizard')
        
        #define variables
        self.x = IntVar()
        self.y = IntVar()
        
        self.w = IntVar()
        self.h = IntVar()
        
        self.xs = IntVar()
        self.ys = IntVar()
        
        # default values
        self.x.set(15)
        self.y.set(15)
        self.w.set(100)
        self.h.set(100)
        self.xs.set(5)
        self.ys.set(5)
        
        #define widgets
        self.xLabel = Label(self, text='x-Pos')
        self.yLabel = Label(self, text='y-Pos')
        
        self.wLabel = Label(self, text='Width')
        self.hLabel = Label(self, text='Height')
        
        self.xsLabel = Label(self, text='x-Step')
        self.ysLabel = Label(self, text='y-Step')
        
        self.xEntry = Entry(self, textvariable=self.x, width=4)
        self.yEntry = Entry(self, textvariable=self.y, width=4)
        
        self.wEntry = Entry(self, textvariable=self.w, width=4)
        self.hEntry = Entry(self, textvariable=self.h, width=4)
        
        self.xsEntry = Entry(self, textvariable=self.xs, width=4)
        self.ysEntry = Entry(self, textvariable=self.ys, width=4)
        
        self.okButton = Button(self, text="   OK   ", width=6, command=self.okButon_pressed)
        self.cancelButton = Button(self, text="Cancel", command=self.quit)
        
        self.xLabel.grid(column=0,row=0,padx=5,pady=5)
        self.xEntry.grid(column=1,row=0)
        
        self.yLabel.grid(column=0,row=1,padx=5,pady=5)
        self.yEntry.grid(column=1,row=1)
        
        self.wLabel.grid(column=2,row=0,padx=5,pady=5)
        self.wEntry.grid(column=3,row=0)
        self.hLabel.grid(column=2,row=1,padx=5,pady=5)
        self.hEntry.grid(column=3,row=1)
        
        self.xsLabel.grid(column=0,row=2,padx=5,pady=5)
        self.xsEntry.grid(column=1,row=2)
        self.ysLabel.grid(column=0,row=3,padx=5,pady=5)
        self.ysEntry.grid(column=1,row=3,padx=5)
        
        self.okButton.grid(column=0,columnspan=2,row=4)
        self.cancelButton.grid(column=2,columnspan=2,row=4,padx=5,pady=5)

    def okButon_pressed(self):
        grid(self.x.get(),self.y.get(),self.w.get(),self.h.get(),self.xs.get(),self.ys.get(),"Black")
        self.quit()

    def quit(self):
        self.master.destroy()

def main():
    """ Application/Dialog loop with Scribus sauce around """
    if scribus.haveDoc() == 0:
        scribus.messageBox("Error", "Please create a Document", ICON_WARNING, BUTTON_OK)
        return
    
    try:
        scribus.statusMessage('Running script...')
        scribus.progressReset()
        root = Tk()
        app = TkGrid(root)
        root.mainloop()
    finally:
        if scribus.haveDoc():
            scribus.redrawAll()
        scribus.statusMessage('Done.')
        scribus.progressReset()

if __name__ == '__main__':
    main()
