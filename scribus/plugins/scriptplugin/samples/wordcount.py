#!/usr/bin/python
# -*- coding: ISO-8859-1 -*-
# DO NOT remove above line
from scribus import *
import os, re


def msgBox(msg):
	# replace with "gdialog" or "xterm -e dialog" if needed
	# it would be nice, if the plugin would provide a msgbox
	# and a file selector
	cmd = 'kdialog --msgbox "%s"'
	os.system(cmd % msg)


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
	msgBox("%s words counted in %s" % (words, source))

else:
	msgBox("Not document open")
