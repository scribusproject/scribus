from scribus import *
import os
SetUnit(1)
x,y = GetPosition()
l,h = GetSize()
texte = GetImageFile()
image = os.path.basename(texte)
a = CreateText(x,y+h+2,l,8)
InsertText(image,0,a)
SetTextAlignment(2,a)
SetFontSize(7,a)