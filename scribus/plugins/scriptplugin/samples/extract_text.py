#!/usr/bin/env python
# File: frameslist.py
# 2006.03.04  Gregory Pittman
# this version 2007.10.27
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

import scribus 

if scribus.haveDoc():
    textfile = scribus.valueDialog('Filename','Enter name of file to save to\n".txt" will be appended')
    textfile = textfile + '.txt'

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
    scribus.messageBox("Finished",endmessage,icon=0,button1=1)
