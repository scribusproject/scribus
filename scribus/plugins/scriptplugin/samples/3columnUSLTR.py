""" Creates 3 column layout on Letter paper and save it under 3columnUS.sla filename"""

from scribus import *
margins = (50, 50, 50, 50)
size = (612, 792)
if newDoc(PAPER_LETTER, margins, LANDSCAPE, 1, UNIT_POINTS, NOFACINGPAGES, FIRSTPAGELEFT):
	a = createText(50, 50, 230, 512)
	setTextAlignment(1,a)
	setText("Column A", a)
	setFontSize(12, a)
	b = createText(280, 50, 230, 512)
	setTextAlignment(1,b)
	setText("Column B", b)
	setFontSize(12, b)
	c = createText(510, 50, 230, 512)
	setTextAlignment(1,b)
	setText("Column C", c)
	setFontSize(12, c)
	saveDocAs("3columnUS.sla")
	closeDoc()

