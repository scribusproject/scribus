""" When you have an image selected this script creates small text legende
below the image. The new textframe contains name of the file. """

from scribus import *
import os

userUnit = getUnit()
setUnit(1)
x,y = getPosition()
l,h = getSize()
texte = getImageFile()
image = os.path.basename(texte)
a = createText(x,y+h+2,l,8)
insertText(image,0,a)
setTextAlignment(2,a)
setFontSize(7,a)
setUnit(userUnit)
