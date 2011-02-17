#!/usr/bin/env python
# -*- coding: utf-8 -*-
# File: quotes.py - changes typewriter quotes to typographic quotes
# © 2010.08.28 Gregory Pittman
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
"""
USAGE

You must have a document open, and a text frame selected.
There will be a valueDialog asking for your language for the quotes, 
the default is 'en', but change the default to suit your needs.
Detected errors shut down the script with an appropriate message.

"""
import scribus

if scribus.haveDoc():
    c = 0
    lang = scribus.valueDialog("Choose by language or country", 'Language: af, be, ch, cs, de, en, es, et, fi, fr,\n hu, is, lt, mk, nl, pl, ru, se, sk, sl, sq and uk\n are current choices','en')
    if (lang == 'en'):
        lead_double = u"\u201c"
        follow_double = u"\u201d"
        lead_single = u"\u2018"
        follow_single = u"\u2019"
    elif (lang == 'de'):
        lead_double = u"\u201e"
        follow_double = u"\u201c"
        lead_single = u"\u2019"
        follow_single = u"\u201a"
    elif (lang == 'fr'):      
        lead_double = u"\u00ab"
        follow_double = u"\u00bb"
        lead_single = u"\u2018"
        follow_single = u"\u2019"  # am hoping this will cover contractions like je t'aime
    elif (lang == 'pl'):
        lead_double = u"\u201e"
        follow_double = u"\u201d"
        lead_single = u"\u201a"
        follow_single = u"\u2019"
    elif ((lang == 'se') or (lang == 'fi')):
        lead_double = u"\u201d"
        follow_double = u"\u201d"
        lead_single = u"\u2019"
        follow_single = u"\u2019"
    elif (lang == 'af'):
        lead_double = u"\u201c"
        follow_double = u"\u201d"
        lead_single = u"\u2018"
        follow_single = u"\u2019"
    elif (lang == 'sq'):
        lead_double = u"\u201e"
        follow_double = u"\u201c"
	lead_single = u"\u2018"
        follow_single = u"\u2019"
    elif ((lang == 'be') or (lang == 'ch') or (lang == 'uk') or (lang == 'ru')):
        lead_double = u"\u00ab"
        follow_double = u"\u00bb"
        lead_single = u"\u2039"
        follow_single = u"\u203a"
    elif (lang == 'uk'):
        lead_double = u"\u00ab"
        follow_double = u"\u00bb"
        lead_single = u"\u2039"
        follow_single = u"\u203a"
    elif (lang == 'es'):
        lead_double = u"\u00ab"
        follow_double = u"\u00bb"
	follow_double = u"\u201d"
        lead_single = u"\u2018"
    elif ((lang == 'lt') or (lang == 'mk') or (lang == 'is') or (lang == 'sk') or (lang == 'sl') or (lang == 'cs') or (lang == 'et')):
        lead_double = u"\u201e"
        follow_double = u"\u201c"
        lead_single = u"\u2019"
        follow_single = u"\u201a"
    elif ((lang == 'hu') or (lang == 'nl')):
	lead_double = u"\u201e"
	follow_double = u"\u201d"
	lead_single = u"\u00bb"
	follow_single = u"\u00ab"
    else:
        scribus.messageBox('Language Error', 'You need to choose an available language', icon=0, button1=1)
        sys.exit(2)
        
else:
    scribus.messageBox('Usage Error', 'You need a Document open', icon=0, button1=1)
    sys.exit(2)

if scribus.selectionCount() == 0:
    scribus.messageBox('Scribus - Usage Error',
        "There is no object selected.\nPlease select a text frame and try again.",
        scribus.ICON_WARNING, scribus.BUTTON_OK)
    sys.exit(2)
if scribus.selectionCount() > 1:
    scribus.messageBox('Scribus - Usage Error',
        "You have more than one object selected.\nPlease select one text frame and try again.", scribus.ICON_WARNING, scribus.BUTTON_OK)
    sys.exit(2)
textbox = scribus.getSelectedObject()
pageitems = scribus.getPageItems()
boxcount = 1
for item in pageitems:
    if (item[0] == textbox):
        if (item[1] != 4):
            scribus.messageBox('Scribus - Usage Error', "This is not a textframe. Try again.", scribus.ICON_WARNING, scribus.BUTTON_OK)
            sys.exit(2)
contents = scribus.getTextLength(textbox)
while c <= (contents -1):
    if ((c + 1) > contents - 1):
        nextchar = ' '
    else:
        scribus.selectText(c+1, 1, textbox)
        nextchar = scribus.getText(textbox)
    scribus.selectText(c, 1, textbox)
    char = scribus.getText(textbox)
    if (len(char) != 1):
        c += 1
        continue
    if ((ord(char) == 34) and (c == 0)):
        scribus.deleteText(textbox)
        scribus.insertText(lead_double, c, textbox)
    elif (ord(char) == 34):
        if ((prevchar == '.') or (prevchar == ',') or (prevchar == '?') or (prevchar == '!')):
            scribus.deleteText(textbox)
            scribus.insertText(follow_double, c, textbox)
        elif ((ord(prevchar) == 39) and ((nextchar != ' ') and (nextchar != ',') and (nextchar != '.'))):
            scribus.deleteText(textbox)
            scribus.insertText(lead_double, c, textbox)
        elif ((nextchar == '.') or (nextchar == ',')):
            scribus.deleteText(textbox)
            scribus.insertText(follow_double, c, textbox)

        elif ((prevchar == ' ') or ((nextchar != ' ') and (ord(nextchar) != 39))):
            scribus.deleteText(textbox)
            scribus.insertText(lead_double, c, textbox)
        else:
            scribus.deleteText(textbox)
            scribus.insertText(follow_double, c, textbox)
            
    if ((ord(char) == 39) and (c == 0)):
        scribus.deleteText(textbox)
        scribus.insertText(lead_single, c, textbox)
    elif (ord(char) == 39):
        if ((prevchar == '.') or (prevchar == ',') or (prevchar == '?') or (prevchar == '!')):
            scribus.deleteText(textbox)
            scribus.insertText(follow_single, c, textbox)
        elif ((ord(prevchar) == 34) and ((nextchar != ' ') and (nextchar != ',') and (nextchar != '.'))):
            scribus.deleteText(textbox)
            scribus.insertText(lead_single, c, textbox)
        elif ((prevchar != ' ') and (ord(prevchar) != 34) and (nextchar != ' ')):
            scribus.deleteText(textbox)
            scribus.insertText(follow_single, c, textbox)
        elif ((prevchar == ' ') or ((nextchar != ' ') and (ord(nextchar) != 34))):
            scribus.deleteText(textbox)
            scribus.insertText(lead_single, c, textbox)
        else:
            scribus.deleteText(textbox)
            scribus.insertText(follow_single, c, textbox)
            
    c += 1
    prevchar = char

scribus.setRedraw(1)
scribus.docChanged(1)
endmessage = 'Successfully ran script\n Last character read was '+str(char) # Change this message to your liking
scribus.messageBox("Finished", endmessage,icon=0,button1=1)