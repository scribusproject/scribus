#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-

""" This script changes quotation marks from " " to french style """

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

import re

TITLE = "Text quoting"

# These need to be declared as unicode strings until some
# charset issues in the scripter are worked out.
QUOTE_START = u""
QUOTE_END = u""

def quote(textobj):
    quoted_re = re.compile('"[^"]*"')
    try:
        text = getText(textobj)
    except WrongFrameTypeError:
        messageBox("quote.py", "Cannot quote text in a non-text frame", ICON_INFORMATION);
        sys.exit(1)
    if len(text) == 0:
        return 0    # We can't very well change anything in an empty frame
    count = 0
    i = 0
    selectText(0, 0, textobj)
    while i < len(text):
        match = quoted_re.match(text[i:])
        if match:
            end = match.end()
            selectText(i, 1, textobj)
            deleteText(textobj)
            insertText(QUOTE_START, i, textobj)
            selectText(i + end - 1, 1, textobj)
            deleteText(textobj)
            insertText(QUOTE_END, i + end - 1, textobj)
            count += 1
            i = i + end
        else:
            i = i + 1
    return count


def main():
    changed = 0
    sel_count = selectionCount()
    if sel_count:
        for i in range(sel_count):
            changed += quote(getSelectedObject(i))
    else:
        for page in range(pageCount()):
            gotoPage(page)
            for obj in getAllObjects():
                changed += quote(obj)
    messageBox(TITLE, "%s quotations changed" % changed,
               ICON_INFORMATION, BUTTON_OK)

if __name__ == '__main__':
    if haveDoc():
        try:
            setRedraw(False)
            main()
        finally:
            setRedraw(True)
            redrawAll()
    else:
        messageBox(TITLE, "No document open", ICON_WARNING, BUTTON_OK)
