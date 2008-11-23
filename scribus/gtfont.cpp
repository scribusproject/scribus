/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "gtfont.h"

const QString gtFont::fontWeights[FontWeightMAX] = 
{
	"",
	"Demi Bold",
	"Extra Black",
	"Extra Bold",
	"Extra Heavy",
	"Extra Light",
	"Semi Bold",
	"Black",
	"Bold",
	"Book",
	"Demi",
	"Heavy",
	"Light",
	"Lite",
	"Medium",
	"Regular",
	"Roman"
};

const QString gtFont::fontSlants[FontSlantMAX] =
{
	"",
	"Italic",
	"Oblique"
};

const QString gtFont::fontWidths[FontWidthMAX] =
{
	"",
	"Extra Condensed",
	"Semi Condensed",
	"Ultra Condensed",
	"Extra Compressed",
	"Semi Compressed",
	"Ultra Compressed",
	"Condensed",
	"Compressed"
};

gtFont::gtFont()
{
	setflags = 0;
	noEffects();
	name        = "";
	family      = "";
	weight      = "";
	slant       = "";
	width       = "";
	append      = "";
	size        = 12;
	color       = "Black";
	shade       = 100;
	strokeColor = "Black";
	strokeShade = 100;
	hscale      = 1000;
	kerning     = 0;
	useFullName = true;
	weightIndex = 0;
	slantIndex  = 1;
	widthIndex  = 2;
	smallestIndex = -1;
	biggestIndex = - 1;
	index = -1;
	tmpWeightIndex = -1;
	tmpSlantIndex = -1;
	tmpWidthIndex = -1;
}

gtFont::gtFont(const gtFont& f)
{
	name        = f.name;
	family      = f.family;
	weight      = f.weight;
	slant       = f.slant;
	width       = f.width;
	append      = f.append;
	size        = f.size;
	color       = f.color;
	shade       = f.shade;
	strokeColor = f.strokeColor;
	strokeShade = f.strokeShade;
	hscale      = f.hscale;
	kerning     = f.kerning;
	useFullName = f.useFullName;
	weightIndex = f.weightIndex;
	slantIndex  = f.slantIndex;
	widthIndex  = f.widthIndex;
	smallestIndex  = f.smallestIndex;
	biggestIndex   = f.biggestIndex;
	index          = f.index;
	tmpWeightIndex = f.tmpWeightIndex;
	tmpSlantIndex  = f.tmpSlantIndex;
	tmpWidthIndex  = f.tmpWidthIndex;
	fontEffects[NORMAL]        = f.fontEffects[NORMAL];
	fontEffects[UNDERLINE]     = f.fontEffects[UNDERLINE];
	fontEffects[STRIKETHROUGH] = f.fontEffects[STRIKETHROUGH];
	fontEffects[SMALL_CAPS]    = f.fontEffects[SMALL_CAPS];
	fontEffects[SUPERSCRIPT]   = f.fontEffects[SUPERSCRIPT];
	fontEffects[SUBSCRIPT]     = f.fontEffects[SUBSCRIPT];
	fontEffects[OUTLINE]       = f.fontEffects[OUTLINE];
	setflags = f.setflags;
}

int gtFont::getFlags()
{
	return setflags;
}

bool gtFont::isToggled(FontEffect fe)
{
	return fontEffects[fe];
}

bool gtFont::toggleEffect(FontEffect fe)
{
	switch (fe)
	{
		case NORMAL:
			if (!fontEffects[NORMAL])
				noEffects();
			break;
		case SUPERSCRIPT:
			fontEffects[SUPERSCRIPT] = !fontEffects[SUPERSCRIPT];
			if (fontEffects[SUPERSCRIPT])
			{
				fontEffects[SUBSCRIPT] = false;
				fontEffects[NORMAL] = false;	
			}
			break;
		case SUBSCRIPT:
			fontEffects[SUBSCRIPT] = !fontEffects[SUBSCRIPT];
			if (fontEffects[SUBSCRIPT])
			{
				fontEffects[SUPERSCRIPT] = false;
				fontEffects[NORMAL] = false;	
			}
			break;
		default:
			fontEffects[fe] = !fontEffects[fe];
			if (fontEffects[fe])
				fontEffects[NORMAL] = false;
	}
	setflags |= effectWasSet;
	return fontEffects[fe];
}

int gtFont::getEffectsValue()
{
	int b = 0;
	if (isToggled(NORMAL))
		b = 0;
	if (isToggled(UNDERLINE))
		b |= 8;
	if (isToggled(STRIKETHROUGH))
		b |= 16;
	if (isToggled(SMALL_CAPS))
		b |= 64;
	if (isToggled(SUPERSCRIPT))
		b |= 1;
	if (isToggled(SUBSCRIPT))
		b |= 2;
	if (isToggled(OUTLINE))
		b |= 4;
	return b;
}

void gtFont::setName(QString newName)
{
	name = newName;
	setWeight(NO_WEIGHT);
	setSlant(NO_SLANT);
	setWidth(NO_WIDTH);
	parseName();
	useFullName = true;
	setflags |= familyWasSet;
}

void gtFont::setFamily(QString newFamily)
{
	family = newFamily;
	useFullName = false;
	setflags |= familyWasSet;
}

QString gtFont::getFamily()
{
	return family;
}

void gtFont::setWeight(FontWeight newWeight)
{
	weight = fontWeights[newWeight];
	useFullName = false;
	setflags |= weightWasSet;
	if ((newWeight == ROMAN) || (newWeight == REGULAR))
	{
		setSlant(NO_SLANT);
		setWidth(NO_WIDTH);
		setflags &= ~weightWasSet;
	}
	if (weightIndex < 0)
	{
		weightIndex = 0;
		slantIndex = 1;
		widthIndex = 2;
	}
}

void gtFont::setWeight(QString newWeight)
{
	weight = newWeight;
	useFullName = false;
	setflags |= weightWasSet;
	if ((newWeight == fontWeights[ROMAN]) || 
		(newWeight == fontWeights[REGULAR]))
	{
		setSlant(NO_SLANT);
		setWidth(NO_WIDTH);
		setflags |= ~weightWasSet;
	}
	if (weightIndex < 0)
	{
		weightIndex = 0;
		slantIndex = 1;
		widthIndex = 2;
	}
}

QString gtFont::getWeight()
{
	return weight;
}

void gtFont::setSlant(FontSlant newSlant)
{
	slant = fontSlants[newSlant];
	useFullName = false;
	setflags &= ~slantWasSet;
	if (newSlant != NO_SLANT)
	{
		if (weight == fontWeights[REGULAR])
			setWeight(NO_WEIGHT);
		else if (weight == fontWeights[ROMAN])
			setWeight(NO_WEIGHT);
		setflags |= slantWasSet;
	}
	if (slantIndex < 0)
	{
		weightIndex = 0;
		slantIndex = 1;
		widthIndex = 2;
	}
}

void gtFont::setSlant(QString newSlant)
{
	slant = newSlant;
	useFullName = false;
	setflags &= ~slantWasSet;
	if (!newSlant.isEmpty())
	{
		if (weight == fontWeights[REGULAR])
			setWeight(NO_WEIGHT);
		else if (weight == fontWeights[ROMAN])
			setWeight(NO_WEIGHT);
		setflags |= slantWasSet;
	}
	if (slantIndex < 0)
	{
		weightIndex = 0;
		slantIndex = 1;
		widthIndex = 2;
	}
}

QString gtFont::getSlant()
{
	return slant;
}

void gtFont::setWidth(FontWidth newWidth)
{
	width = fontWidths[newWidth];
	useFullName = false;
	setflags &= ~widthWasSet;
	if (newWidth != NO_WIDTH)
	{
		if (weight == fontWeights[REGULAR])
			setWeight(NO_WEIGHT);
		else if (weight == fontWeights[ROMAN])
			setWeight(NO_WEIGHT);
		setflags |= widthWasSet;
	}
	if (widthIndex < 0)
	{
		weightIndex = 0;
		slantIndex = 1;
		widthIndex = 2;
	}
}

void gtFont::setWidth(QString newWidth)
{
	width = newWidth;
	useFullName = false;
	setflags &= ~widthWasSet;
	if (!newWidth.isEmpty())
	{
		if (weight == fontWeights[REGULAR])
			setWeight(NO_WEIGHT);
		else if (weight == fontWeights[ROMAN])
			setWeight(NO_WEIGHT);
		setflags |= widthWasSet;
	}
	if (widthIndex < 0)
	{
		weightIndex = 0;
		slantIndex = 1;
		widthIndex = 2;
	}
}

QString gtFont::getWidth()
{
	return width;
}

void gtFont::setSize(int newSize)
{
	size = newSize;
	setflags |= sizeWasSet;
}

void gtFont::setSize(double newSize)
{
	size = static_cast<int>(newSize);
	setflags |= sizeWasSet;
}

void gtFont::setColor(QString newColor)
{
	color = newColor;
	setflags |= fillColorWasSet;
}

void gtFont::setShade(int newShade)
{
	shade = newShade;
	setflags |= fillShadeWasSet;
}

void gtFont::setStrokeColor(QString newColor)
{
	strokeColor = newColor;
	setflags |= strokeColorWasSet;
}

void gtFont::setStrokeShade(int newShade)
{
	strokeShade = newShade;
	setflags |= strokeShadeWasSet;
}

QString gtFont::getName()
{
	if (useFullName)
		return name;

	QString name2 = family + " ";

	if (weightIndex == 0)
		name2 += weight + " ";
	else if (slantIndex == 0) 
		name2 += slant + " ";
	else if (widthIndex == 0)
		name2 += width + " ";

	if (weightIndex == 1)
		name2 += weight + " ";
	else if (slantIndex == 1) 
		name2 += slant + " ";
	else if (widthIndex == 1)
		name2 += width + " ";

	if (weightIndex == 2)
		name2 += weight + " ";
	else if (slantIndex == 2) 
		name2 += slant + " ";
	else if (widthIndex == 2)
		name2 += width + " ";

	name2 += append;
	name2 = name2.simplified();
	return name2;
}

QString gtFont::getName(uint i)
{
	QString fname = family + " ";
	switch (i)
	{
		case 0:
			fname = fname + weight + " " + slant + " " + width + " " + append;
			break;
		case 1:
			fname = fname + weight + " " + width + " " + slant + " " + append;
			break;
		case 2:
			fname = fname + slant + " " + weight + " " + width + " " + append;
			break;
		case 3:
			fname = fname + slant + " " + width + " " + weight + " " + append;
			break;
		case 4:
			fname = fname + width + " " + weight + " " + slant + " " + append;
			break;
		case 5:
			fname = fname + width + " " + slant + " " + weight + " " + append;
			break;
		case 6:
			fname = fname + " " + append + " " + weight + " " + slant + " " + width;
			break;
		case 7:
			fname = fname + " " + append + " " + weight + " " + width + " " + slant;
			break;
		case 8:
			fname = fname + " " + append + " " + slant + " " + weight + " " + width;
			break;
		case 9:
			fname = fname + " " + append + " " + slant + " " + width + " " + weight;
			break;
		case 10:
			fname = fname + " " + append + " " + width + " " + weight + " " + slant;
			break;
		case 11:
			fname = fname + " " + append + " " + width + " " + slant + " " + weight;
			break;
		case 12:
			if ((append.isEmpty()) && (weight.isEmpty()) && (slant.isEmpty()) && (width.isEmpty()))
				fname = fname + " " + fontWeights[REGULAR];
			break;
		case 13:
			if ((append.isEmpty()) && (weight.isEmpty()) && (slant.isEmpty()) && (width.isEmpty()))
				fname = fname + " " + fontWeights[ROMAN];
			break;
	}
	fname = fname.simplified();
	return fname;
}

int gtFont::getSize()
{
	return size;
}

QString gtFont::getColor()
{
	return color;
}

int gtFont::getShade()
{
	return shade;
}

QString gtFont::getStrokeColor()
{
	return strokeColor;
}

int gtFont::getStrokeShade()
{
	return strokeShade;
}

void gtFont::noEffects()
{
	fontEffects[NORMAL]        = true;
	fontEffects[UNDERLINE]     = false;
	fontEffects[STRIKETHROUGH] = false;
	fontEffects[SMALL_CAPS]    = false;
	fontEffects[SUPERSCRIPT]   = false;
	fontEffects[SUBSCRIPT]     = false;
	fontEffects[OUTLINE]       = false;
	setflags &= ~effectWasSet;
}

int gtFont::getHscale()
{
	return hscale;
}

void gtFont::setHscale(int newHscale)
{
	hscale = newHscale;
	setflags |= hscaleWasSet;
}

int gtFont::getKerning()
{
	return kerning;
}

void gtFont::setKerning(int newKerning)
{
	kerning = newKerning;
	setflags |= kerningWasSet;
}

void gtFont::parseName()
{
	smallestIndex = -1;
	biggestIndex = - 1;
	index = -1;
	tmpWeightIndex = -1;
	tmpSlantIndex = -1;
	tmpWidthIndex = -1;
	parseWeight();
	parseSlant();
	parseWidth();
	parseFamily();
}

void gtFont::parseWeight()
{
	bool found = false;
	for (int i = 1; i < FontWeightMAX; ++i)
	{
		index = name.indexOf(fontWeights[i]); // f.e. Demi Bold
		QString tmpWeight = "";
		if ((index == -1) && (fontWeights[i].indexOf(" ") != -1) && (fontWeights[i].indexOf(" ") != 1))
		{
			QString fw2 = fontWeights[i];
			fw2.replace(" ", "-"); // f.e. Demi-Bold
			index = name.indexOf(fw2);
			if (index == -1)
			{
				fw2 = fontWeights[i];
				fw2.replace(" ", ""); // f.e. DemiBold
				index = name.indexOf(fw2);
				if (index == -1)
				{
					fw2 = fontWeights[i];
					fw2.replace(" B", " b"); // f.e. Demibold
					fw2.replace(" C", " c");
					fw2.replace(" H", " h");
					fw2.replace(" L", " l");
					fw2.replace(" ", "");
					index = name.indexOf(fw2);
					if (index != -1)
						tmpWeight = fw2;
				}
				else
					tmpWeight = fw2;
			}
			else
			{
				tmpWeight = fw2;
			}
		}
		else
			tmpWeight = fontWeights[i];
		if (index != -1)
		{
			weight = tmpWeight;
			if (smallestIndex == -1 || smallestIndex > index)
				smallestIndex = index;
			if ((biggestIndex == -1) || (biggestIndex < index + static_cast<int>(tmpWeight.length()) - 1))
				biggestIndex = index + tmpWeight.length();
			found = true;
			tmpWeightIndex = index;
			break;
		}
	}
	if (!found)
		weight = fontWeights[NO_WEIGHT];
}

void gtFont::parseSlant()
{
	bool found = false;
	for (int i = 1; i < FontSlantMAX; ++i)
	{
		index = name.indexOf(fontSlants[i]);
		if (index != -1)
		{
			slant = fontSlants[i];
			if (smallestIndex == -1 || smallestIndex > index)
				smallestIndex = index;
			if ((biggestIndex == -1) || (biggestIndex < index + static_cast<int>(slant.length()) - 1))
				biggestIndex = index + slant.length();
			found = true;
			tmpSlantIndex = index;
			break;
		}
	}
	if (!found)
		slant = fontSlants[NO_SLANT];
}

void gtFont::parseWidth()
{
	bool found = false;
	for (int i = 1; i < FontWidthMAX; ++i)
	{
		index = name.indexOf(fontWidths[i]);
		QString tmpWidth = "";
		if ((index == -1) && (fontWidths[i].indexOf(" ") != -1) && (fontWidths[i].indexOf(" ") != 1))
		{
			QString fw2 = fontWidths[i];
			fw2.replace(" ", "-");
			index = name.indexOf(fw2);
			if (index == -1)
			{
				fw2 = fontWidths[i];
				fw2.replace(" ", "");
				index = name.indexOf(fw2);
				if (index == -1)
				{
					fw2 = fontWidths[i];
					fw2.replace(" B", " b");
					fw2.replace(" C", " c");
					fw2.replace(" H", " h");
					fw2.replace(" L", " l");
					fw2.replace(" ", "");
					index = name.indexOf(fw2);
					if (index != -1)
						tmpWidth = fw2;
				}
				else
					tmpWidth = fw2;
			}
			else
			{
				tmpWidth = fw2;
			}
		}
		else
			tmpWidth = fontWidths[i];
		if (index != -1)
		{
			width = tmpWidth;
			if (smallestIndex == -1 || smallestIndex > index)
				smallestIndex = index;
			if ((biggestIndex == -1) || (biggestIndex < index + static_cast<int>(tmpWidth.length()) - 1))
				biggestIndex = index + tmpWidth.length();
			found = true;
			tmpWidthIndex = index;
			break;
		}
	}
	if (!found)
		width = fontWidths[NO_WIDTH];
}
	
void gtFont::parseFamily()
{
	if (tmpWeightIndex < tmpSlantIndex)
	{
		weightIndex = 0;
		slantIndex  = 1;
		if (tmpWidthIndex < tmpWeightIndex)
		{
			widthIndex = 0;
			weightIndex = 1;
			slantIndex = 2;
		}
		else if (tmpWidthIndex < tmpSlantIndex)
		{
			widthIndex = 1;
			slantIndex = 2;
		}
		else
			widthIndex = 2;
	}
	else if (tmpWeightIndex > tmpSlantIndex)
	{
		slantIndex = 0;
		weightIndex = 1;
		if (tmpWidthIndex < tmpSlantIndex)
		{
			widthIndex = 0;
			slantIndex = 1;
			weightIndex = 2;
		}
		else if (tmpWidthIndex < tmpWeightIndex)
		{
			widthIndex = 1;
			weightIndex = 2;
		}
		else
			widthIndex = 2;
	}
	else
	{
		weightIndex = -2;
		widthIndex = -1;
		slantIndex = 0;
	}

	if (smallestIndex == -1)
		family = name;
	else
		family = name.left(smallestIndex);
	if (biggestIndex == -1 || biggestIndex >= name.length())
		append = "";
	else
		append = name.right(name.length() - biggestIndex - 1);
	family = family.trimmed();
}

int gtFont::find(const QString& where, const QString& what)
{
	QString realWhat = " " + what;
	int index = where.lastIndexOf(realWhat); // f.e. Demi Bold
	if (index != -1)
	{
		if (index + realWhat.length() != where.length())
			if (where[index + realWhat.length() + 1] != ' ')
				index = -1;
	}
	return index;
}

gtFont::~gtFont()
{

}
