/***************************************************************************
 *   Copyright (C) 2004 by Riku Leino                                      *
 *   tsoots@welho.com                                                      *
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
	"Medium",
	"Regular"
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
	"Condensed"
};

gtFont::gtFont()
{
	noEffects();
	name        = "";
	family      = "";
	weight      = "";
	slant       = "";
	width       = "";
	size        = 12;
	color       = "Black";
	shade       = 100;
	strokeColor = "Black";
	strokeShade = 100;
	hscale      = 100;
	kerning     = 0.0;
}

gtFont::gtFont(const gtFont& f)
{
	name        = f.name;
	family      = f.family;
	weight      = f.weight;
	slant       = f.slant;
	width       = f.width;
	size        = f.size;
	color       = f.color;
	shade       = f.shade;
	strokeColor = f.strokeColor;
	strokeShade = f.strokeShade;
	hscale      = f.hscale;
	kerning     = f.kerning;
	fontEffects[NORMAL]        = f.fontEffects[NORMAL];
	fontEffects[UNDERLINE]     = f.fontEffects[UNDERLINE];
	fontEffects[STRIKETHROUGH] = f.fontEffects[STRIKETHROUGH];
	fontEffects[SMALL_CAPS]    = f.fontEffects[SMALL_CAPS];
	fontEffects[SUPERSCRIPT]   = f.fontEffects[SUPERSCRIPT];
	fontEffects[SUBSCRIPT]     = f.fontEffects[SUBSCRIPT];
	fontEffects[OUTLINE]       = f.fontEffects[OUTLINE];
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
}

void gtFont::setFamily(QString newFamily)
{
	family = newFamily;
}

void gtFont::setWeight(FontWeight newWeight)
{
	weight = fontWeights[newWeight];
}

void gtFont::setSlant(FontSlant newSlant)
{
	slant = fontSlants[newSlant];
}

void gtFont::setWidth(FontWidth newWidth)
{
	width = fontWidths[newWidth];
}

void gtFont::setWeight(QString newWeight)
{
	weight = newWeight;
}

void gtFont::setSlant(QString newSlant)
{
	slant = newSlant;
}

void gtFont::setWidth(QString newWidth)
{
	width = newWidth;
}

void gtFont::setSize(int newSize)
{
	size = newSize;
}

void gtFont::setSize(double newSize)
{
	newSize = newSize * 10;
	size = static_cast<int>(newSize);
}

void gtFont::setColor(QString newColor)
{
	color = newColor;
}

void gtFont::setShade(int newShade)
{
	shade = newShade;
}

void gtFont::setStrokeColor(QString newColor)
{
	strokeColor = newColor;
}

void gtFont::setStrokeShade(int newShade)
{
	strokeShade = newShade;
}

QString gtFont::getName()
{
	QString name2 = family + " " + weight + " " + slant + " " + width;
	name2 = name2.simplifyWhiteSpace();
	return name2;

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
}

int gtFont::getHscale()
{
	return hscale;
}

void    gtFont::setHscale(int newHscale)
{
	hscale = newHscale;
}

double gtFont::getKerning()
{
	return kerning;
}

void gtFont::setKerning(double newKerning)
{
	kerning = newKerning;
}

void gtFont::parseName()
{
	int smallestIndex = -1;
	int index = -1;
	bool found = false;
	for (int i = 1; i < FontWeightMAX; ++i)
	{
		index = name.find(fontWeights[i]); // f.e. Demi Bold
		QString tmpWeight = "";
		if ((index == -1) && (fontWeights[i].find(" ") != -1))
		{
			QString fw2 = fontWeights[i];
			fw2.replace(" ", "-"); // f.e. Demi-Bold
			index = name.find(fw2);
			if (index == -1)
			{
				fw2 = fontWeights[i];
				fw2.replace(" ", ""); // f.e. DemiBold
				index = name.find(fw2);
				if (index == -1)
				{
					fw2 = fontWeights[i];
					fw2.replace(" B", " b"); // f.e. Demibold
					fw2.replace(" C", " c");
					fw2.replace(" H", " h");
					fw2.replace(" L", " l");
					fw2.replace(" ", "");
					index = name.find(fw2);
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
			found = true;
			break;
		}
	}
	if (!found)
		weight = fontWeights[NO_WEIGHT];

	found = false;
	for (int i = 1; i < FontSlantMAX; ++i)
	{
		index = name.find(fontSlants[i]);
		if (index != -1)
		{
			slant = fontSlants[i];
			if (smallestIndex == -1 || smallestIndex > index)
				smallestIndex = index;
			found = true;
			break;
		}
	}
	if (!found)
		slant = fontSlants[NO_SLANT];

	found = false;
	for (int i = 1; i < FontWidthMAX; ++i)
	{
		index = name.find(fontWidths[i]);
		QString tmpWidth = "";
		if ((index == -1) && (fontWidths[i].find(" ") != -1))
		{
			QString fw2 = fontWidths[i];
			fw2.replace(" ", "-");
			index = name.find(fw2);
			if (index == -1)
			{
				fw2 = fontWidths[i];
				fw2.replace(" ", "");
				index = name.find(fw2);
				if (index == -1)
				{
					fw2 = fontWidths[i];
					fw2.replace(" B", " b"); // f.e. Demibold
					fw2.replace(" C", " c");
					fw2.replace(" H", " h");
					fw2.replace(" L", " l");
					fw2.replace(" ", "");
					index = name.find(fw2);
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
			found = true;
			break;
		}
	}
	if (!found)
		width = fontWidths[NO_WIDTH];

	if (smallestIndex == -1)
		family = name;
	else
		family = name.left(smallestIndex);
	family = family.stripWhiteSpace();
	
}

gtFont::~gtFont()
{

}
