#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)
from sgmllib import SGMLParser
from htmlentitydefs import entitydefs

""" You can import some HTML files using this script.

ENCODING has to be edited for each file you're trying to parse
utf8 - is the 'universal' solution but you can get error:
'UnicodeDecodeError: 'utf8' codec can't decode byte 0xbe in position 0: unexpected code byte'
then you shall edit right encoding of the file e.g. ISO-8859-2
"""
ENCODING = "utf8"
TITLE = "Import HTML"
BUTTON_OK = 1
ICON_WARNING = 2
NEWLINE = ['h1', 'h2', 'h3', 'h4', 'h5', 'h6',
           'br', 'p', 'li', 'div', 'tr']

class HTMLParser(SGMLParser):

    def __init__(self, textbox):
        self.encoding = valueDialog(TITLE, 'Set encoding of the imported file', ENCODING)
        SGMLParser.__init__(self)
        self.in_body = 0
        self.textbox = textbox

    def append(self, text):
        insertText(unicode(text, self.encoding), getTextLength(self.textbox), self.textbox)

    def start_body(self, attrs):
        self.in_body = 1

    def end_body(self):
        self.in_body = 0

    def unknown_starttag(self, name, attrs):
        if name in NEWLINE:
            self.append('\n')

    def unknown_endtag(self, name):
        if name in NEWLINE:
            self.append('\n')

    def handle_data(self, raw_data):
        if self.in_body:
            data = ' '.join(
                raw_data.replace('\n', ' ').split())
            if raw_data.startswith(' '):
                data = ' ' + data
            if raw_data.endswith(' ') and len(raw_data) > 1:
                data = data + ' '
            self.append(data)

    def unknown_entityref(self, entity):
        self.handle_data(entitydefs.get(entity, ''))


if haveDoc():
    filename = fileDialog(TITLE, "*.htm*", "", 0, 1)
    if filename:
        unit = getUnit()
        setUnit(UNIT_MILLIMETERS)
        textbox = createText(20, 20, 70, 250)
        parser = HTMLParser(textbox)
        parser.feed(open(filename).read())
        setUnit(unit)
else:
    messageBox(TITLE, "No document open", ICON_WARNING, BUTTON_OK)


