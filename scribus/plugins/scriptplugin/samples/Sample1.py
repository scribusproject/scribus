""" A sample script """
from scribus import *
Margins = (10, 10, 10, 30)
if NewDoc(Paper_A4, Margins, Portrait, 1, Points, NoFacingPages, FirstPageRight):
	a = CreateText(50, 50, 200, 80)
	SetText("A Test for Scribus", a)
	SetFontSize(20, a)
	b = CreateEllipse(267, 391, 60, 60)
	SetFillColor("Red", b)
	SaveDocAs("Sample1.sla")
	CloseDoc()
