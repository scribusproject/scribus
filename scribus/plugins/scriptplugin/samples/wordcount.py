# -*- coding: ISO-8859-1 -*-
from scribus import *
import re


TITLE = "Word count"
BUTTON_OK = 1
ICON_INFORMATION = 1
ICON_WARNING = 2



def wordsplit(text):
	word_pattern = "([A-Za-zäöüÄÖÜß]+)"
	words = []
	for x in re.split(word_pattern, text):
		if re.match(word_pattern, x):
			words.append(x)
	return words


if HaveDoc():
	words = 0
	sel_count = SelectionCount()
	if sel_count:
		source = "selected textframe"
		if sel_count > 1: source += "s" #plural
		for i in range(sel_count):
			text = GetText(GetSelectedObject(i))
			words += len(wordsplit(text))
	else:
		source = "whole document"
		for page in range(PageCount()):
			GotoPage(page)
			for obj in GetAllObjects():
				text = GetText(obj)
				words += len(wordsplit(text))
				
	if words == 0: words = "No"	
	MessageBox(TITLE, "%s words counted in %s" % (words, source),
			   ICON_INFORMATION, BUTTON_OK)

else:
	MessageBox(TITLE, "Not document open", ICON_WARNING, BUTTON_OK)
