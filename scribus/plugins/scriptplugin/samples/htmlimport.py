from scribus import *

from sgmllib import SGMLParser
from htmlentitydefs import entitydefs
import os


DEFAULT_SIZE = 10
HEADERS = {'h1': 48, 'h2': 36, 'h3': 24,
		   'h4': 18, 'h5': 14, 'h6': 12 }
NEWLINE = ['h1', 'h2', 'h3', 'h4', 'h5', 'h6',
		   'br', 'p', 'li', 'div', 'tr']


class HTMLParser(SGMLParser):

	def __init__(self, textbox):
		SGMLParser.__init__(self)
		self.in_body = 0
		self.textbox = textbox
		self.textbox.setSize(DEFAULT_SIZE)

	def start_body(self, attrs):
		self.in_body = 1

	def end_body(self):
		self.in_body = 0

	def unknown_starttag(self, name, attrs):
		if name in NEWLINE:
			self.textbox.append('\n')		
		if name in HEADERS.keys():
			self.textbox.setSize(HEADERS[name])
			
	def unknown_endtag(self, name):
		if name in HEADERS.keys():
			self.textbox.setSize(DEFAULT_SIZE)
		if name in NEWLINE:
			self.textbox.append('\n')

	def handle_data(self, raw_data):
		if self.in_body:
			data = ' '.join(
				raw_data.replace('\n', ' ').split())
			if raw_data.startswith(' '):
				data = ' ' + data
			if raw_data.endswith(' ') and len(raw_data) > 1:
				data = data + ' '
			self.textbox.append(data)

	def unknown_entityref(self, entity):
		self.handle_data(entitydefs.get(entity, ''))



def openFileDialog():
	cmd = 'kdialog --getopenfilename "." "*.html|*.htm"'
	pipe = os.popen(cmd)
	filename = pipe.read()[:-1]
	pipe.close()
	return filename


def messageBox(msg):
	cmd = 'kdialog --msgbox "%s"' % msg
	os.system(cmd)
	
	
class TextBox:

	def __init__(self, x, y, width, height):
		self.boxid = CreateText(x, y, width, height)

	def setSize(self, size):
		# XXX: disabled, doesn't work like expected
		pass #SetFontSize(size, self.boxid)

	def append(self, text):
		InsertText(text, GetTextLength(self.boxid),
				   self.boxid)		
		



def main():
	if HaveDoc():
		filename = openFileDialog()
		if filename:
			SetUnit(1)
			textbox = TextBox(20, 20, 70, 250)
			parser = HTMLParser(textbox)
			parser.feed(open(filename).read())
	else:
		messageBox("No document selected")


main()
