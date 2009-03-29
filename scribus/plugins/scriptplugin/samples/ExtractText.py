#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""

(C)2006.03.04 Gregory Pittman

(C)2008.02.28 Petr Vanek - fileDialog replaces valueDialog

this version 2008.02.28

This program is free software; you can redistribute it and/or modify
it under the terms of the  GPL, v2 (GNU General Public License as published by
the Free Software Foundation, version 2 of the License), or any later version.
See the Scribus Copyright page in the Help Browser for further informaton 
about GPL, v2.

SYNOPSIS

This script takes the current document and extracts all the text from text frames,
and also gets the pathnames to all images. This is then saved to a file named
by the user.

REQUIREMENTS

You must run from Scribus and must have a file open.

USAGE

Start the script. A file dialog appears for the name of the file 
to save to. The above information is saved to the file.

"""
# Craig Bradney, Scribus Team
# 10/3/08: Added to Scribus 1.3.3.12svn distribution "as was" from Scribus wiki for bug #6826, script is GPLd

import scribus


def exportText(textfile):
    page = 1
    pagenum = scribus.pageCount()
    T = []
    content = []
    while (page <= pagenum):
        scribus.gotoPage(page)
        d = scribus.getPageItems()
        strpage = str(page)
        T.append('Page '+ strpage + '\n\n')
        for item in d:
            if (item[1] == 4):
                contents = scribus.getAllText(item[0])
                if (contents in content):
                    contents = 'Duplication, perhaps linked-to frame'
                T.append(item[0]+': '+ contents + '\n\n')
                content.append(contents)
            elif (item[1] == 2):
                imgname = scribus.getImageFile(item[0])
                T.append(item[0]+': ' + imgname + '\n')
        page += 1
        T.append('\n')
    output_file = open(textfile,'w')
    output_file.writelines(T)
    output_file.close()
    endmessage = textfile + ' was created'
    scribus.messageBox("Finished", endmessage,icon=0,button1=1)


if scribus.haveDoc():
    textfile = scribus.fileDialog('Enter name of file to save to', \
                                  filter='Text Files (*.txt);;All Files (*)')
    try:
        if textfile == '':
            raise Exception
        exportText(textfile)
    except Exception, e:
        print e

else:
    scribus.messageBox('Export Error', 'You need a Document open, and a frame selected.', \
                       icon=0, button1=1)
