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
	text = GetText(textobj)
	count = 0
	i = 0
	SelectText(0, 0, textobj)
	while i < len(text):
		match = quoted_re.match(text[i:])
		if match:
			end = match.end()
			SelectText(i, 1, textobj)
			DeleteText(textobj)
			InsertText(QUOTE_START, i, textobj)
			SelectText(i + end - 1, 1, textobj)
			DeleteText(textobj)
			InsertText(QUOTE_END, i + end - 1, textobj)
			count += 1
			i = i + end
		else:
			i = i + 1
	return count


if HaveDoc():
	changed = 0
	sel_count = SelectionCount()
	SetRedraw(0)
	if sel_count:
		for i in range(sel_count):
			changed += quote(GetSelectedObject(i))
	else:
		for page in range(PageCount()):
			GotoPage(page)
			for obj in GetAllObjects():
				changed += quote(obj)
	SetRedraw(1)
	RedrawAll()
	MessageBox(TITLE, "%s quotations changed" % changed,
			   ICON_INFORMATION, BUTTON_OK)
	
else:
	msgBox(TITLE, "No document open", ICON_WARNING, BUTTON_OK)
