from scribus import *
from sgmllib import SGMLParser
from htmlentitydefs import entitydefs


TITLE = "Import HTML"
BUTTON_OK = 1
ICON_WARNING = 2
NEWLINE = ['h1', 'h2', 'h3', 'h4', 'h5', 'h6',
		   'br', 'p', 'li', 'div', 'tr']


class HTMLParser(SGMLParser):

	def __init__(self, textbox):
		SGMLParser.__init__(self)
		self.in_body = 0
		self.textbox = textbox

	def append(self, text):
		InsertText(text, GetTextLength(self.textbox), self.textbox)

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

		



if HaveDoc():
	filename = FileDialog(TITLE, "*.htm*", "", 0, 1)
	if filename:
		unit = GetUnit()
		SetUnit(Millimeters)
		textbox = CreateText(20, 20, 70, 250)
		parser = HTMLParser(textbox)
		parser.feed(open(filename).read())
		SetUnit(unit)
else:
	MessageBox(TITLE, "No document open", ICON_WARNING, BUTTON_OK)


