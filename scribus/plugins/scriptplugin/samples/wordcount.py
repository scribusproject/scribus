""" Counts the words in the whole document or in a textframe """

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


if haveDoc():
	words = 0
	sel_count = selectionCount()
	if sel_count:
		source = "selected textframe"
		if sel_count > 1: source += "s" #plural
		for i in range(sel_count):
			text = getText(getSelectedObject(i))
			words += len(wordsplit(text))
	else:
		source = "whole document"
		for page in range(pageCount()):
			gotoPage(page)
			for obj in getAllObjects():
				text = getText(obj)
				words += len(wordsplit(text))

	if words == 0: words = "No"
	messageBox(TITLE, "%s words counted in %s" % (words, source),
			   ICON_INFORMATION, BUTTON_OK)

else:
	messageBox(TITLE, "No document open", ICON_WARNING, BUTTON_OK)
