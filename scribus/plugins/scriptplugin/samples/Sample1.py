""" A sample script """

from scribus import *
margins = (10, 10, 10, 30)
if newDoc(PAPER_A4, margins, PORTRAIT, 1, UNIT_POINTS, NOFACINGPAGES, FIRSTPAGERIGHT):
	a = createText(50, 50, 200, 80)
	setText("A Test for Scribus", a)
	setFontSize(20, a)
	b = createEllipse(267, 391, 60, 60)
	setFillColor("Red", b)
	saveDocAs("Sample1.sla")
	closeDoc()
