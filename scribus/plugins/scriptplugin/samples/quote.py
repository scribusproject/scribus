""" This script changes quotation marks from " " to french style """

# -*- coding: ISO-8859-1 -*-
from scribus import *
import re

TITLE = "Text quoting"
BUTTON_OK = 1
ICON_INFORMATION = 1
ICON_WARNING = 2
QUOTE_START = "»"
QUOTE_END = "«"


def quote(textobj):
	quoted_re = re.compile('"[^"]*"')
	text = getText(textobj)
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


if haveDoc():
	changed = 0
	sel_count = selectionCount()
	setRedraw(0)
	if sel_count:
		for i in range(sel_count):
			changed += quote(getSelectedObject(i))
	else:
		for page in range(pageCount()):
			gotoPage(page)
			for obj in getAllObjects():
				changed += quote(obj)
	setRedraw(1)
	redrawAll()
	messageBox(TITLE, "%s quotations changed" % changed,
			   ICON_INFORMATION, BUTTON_OK)

else:
	messageBox(TITLE, "No document open", ICON_WARNING, BUTTON_OK)
