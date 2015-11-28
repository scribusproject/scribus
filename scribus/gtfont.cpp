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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
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
	m_setflags = 0;
	noEffects();
	m_name        = "";
	m_family      = "";
	m_weight      = "";
	m_slant       = "";
	m_width       = "";
	m_append      = "";
	m_size        = 120;
	m_color       = "Black";
	m_shade       = 100;
	m_strokeColor = "Black";
	m_strokeShade = 100;
	m_hscale      = 1000;
	m_kerning     = 0;
	m_useFullName = true;
	m_weightIndex = 0;
	m_slantIndex  = 1;
	m_widthIndex  = 2;
	m_smallestIndex = -1;
	m_biggestIndex = - 1;
	m_index = -1;
	m_tmpWeightIndex = -1;
	m_tmpSlantIndex = -1;
	m_tmpWidthIndex = -1;
}

gtFont::gtFont(const gtFont& f)
{
	m_name        = f.m_name;
	m_family      = f.m_family;
	m_weight      = f.m_weight;
	m_slant       = f.m_slant;
	m_width       = f.m_width;
	m_append      = f.m_append;
	m_size        = f.m_size;
	m_color       = f.m_color;
	m_shade       = f.m_shade;
	m_strokeColor = f.m_strokeColor;
	m_strokeShade = f.m_strokeShade;
	m_hscale      = f.m_hscale;
	m_kerning     = f.m_kerning;
	m_useFullName = f.m_useFullName;
	m_weightIndex = f.m_weightIndex;
	m_slantIndex  = f.m_slantIndex;
	m_widthIndex  = f.m_widthIndex;
	m_smallestIndex  = f.m_smallestIndex;
	m_biggestIndex   = f.m_biggestIndex;
	m_index          = f.m_index;
	m_tmpWeightIndex = f.m_tmpWeightIndex;
	m_tmpSlantIndex  = f.m_tmpSlantIndex;
	m_tmpWidthIndex  = f.m_tmpWidthIndex;
	m_fontEffects[NORMAL]        = f.m_fontEffects[NORMAL];
	m_fontEffects[UNDERLINE]     = f.m_fontEffects[UNDERLINE];
	m_fontEffects[STRIKETHROUGH] = f.m_fontEffects[STRIKETHROUGH];
	m_fontEffects[SMALL_CAPS]    = f.m_fontEffects[SMALL_CAPS];
	m_fontEffects[SUPERSCRIPT]   = f.m_fontEffects[SUPERSCRIPT];
	m_fontEffects[SUBSCRIPT]     = f.m_fontEffects[SUBSCRIPT];
	m_fontEffects[OUTLINE]       = f.m_fontEffects[OUTLINE];
	m_setflags = f.m_setflags;
}

int gtFont::getFlags()
{
	return m_setflags;
}

bool gtFont::isToggled(FontEffect fe)
{
	return m_fontEffects[fe];
}

bool gtFont::toggleEffect(FontEffect fe)
{
	switch (fe)
	{
		case NORMAL:
			if (!m_fontEffects[NORMAL])
				noEffects();
			break;
		case SUPERSCRIPT:
			m_fontEffects[SUPERSCRIPT] = !m_fontEffects[SUPERSCRIPT];
			if (m_fontEffects[SUPERSCRIPT])
			{
				m_fontEffects[SUBSCRIPT] = false;
				m_fontEffects[NORMAL] = false;	
			}
			break;
		case SUBSCRIPT:
			m_fontEffects[SUBSCRIPT] = !m_fontEffects[SUBSCRIPT];
			if (m_fontEffects[SUBSCRIPT])
			{
				m_fontEffects[SUPERSCRIPT] = false;
				m_fontEffects[NORMAL] = false;	
			}
			break;
		default:
			m_fontEffects[fe] = !m_fontEffects[fe];
			if (m_fontEffects[fe])
				m_fontEffects[NORMAL] = false;
	}
	m_setflags |= effectWasSet;
	return m_fontEffects[fe];
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
	m_name = newName;
	setWeight(NO_WEIGHT);
	setSlant(NO_SLANT);
	setWidth(NO_WIDTH);
	parseName();
	m_useFullName = true;
	m_setflags |= familyWasSet;
}

void gtFont::setFamily(QString newFamily)
{
	m_family = newFamily;
	m_useFullName = false;
	m_setflags |= familyWasSet;
}

QString gtFont::getFamily()
{
	return m_family;
}

void gtFont::setWeight(FontWeight newWeight)
{
	m_weight = fontWeights[newWeight];
	m_useFullName = false;
	m_setflags |= weightWasSet;
	if ((newWeight == ROMAN) || (newWeight == REGULAR))
	{
		setSlant(NO_SLANT);
		setWidth(NO_WIDTH);
		m_setflags &= ~weightWasSet;
	}
	if (m_weightIndex < 0)
	{
		m_weightIndex = 0;
		m_slantIndex = 1;
		m_widthIndex = 2;
	}
}

void gtFont::setWeight(QString newWeight)
{
	m_weight = newWeight;
	m_useFullName = false;
	m_setflags |= weightWasSet;
	if ((newWeight == fontWeights[ROMAN]) || 
		(newWeight == fontWeights[REGULAR]))
	{
		setSlant(NO_SLANT);
		setWidth(NO_WIDTH);
		m_setflags &= ~weightWasSet;
	}
	if (m_weightIndex < 0)
	{
		m_weightIndex = 0;
		m_slantIndex = 1;
		m_widthIndex = 2;
	}
}

QString gtFont::getWeight()
{
	return m_weight;
}

void gtFont::setSlant(FontSlant newSlant)
{
	m_slant = fontSlants[newSlant];
	m_useFullName = false;
	m_setflags &= ~slantWasSet;
	if (newSlant != NO_SLANT)
	{
		if (m_weight == fontWeights[REGULAR])
			setWeight(NO_WEIGHT);
		else if (m_weight == fontWeights[ROMAN])
			setWeight(NO_WEIGHT);
		m_setflags |= slantWasSet;
	}
	if (m_slantIndex < 0)
	{
		m_weightIndex = 0;
		m_slantIndex = 1;
		m_widthIndex = 2;
	}
}

void gtFont::setSlant(QString newSlant)
{
	m_slant = newSlant;
	m_useFullName = false;
	m_setflags &= ~slantWasSet;
	if (!newSlant.isEmpty())
	{
		if (m_weight == fontWeights[REGULAR])
			setWeight(NO_WEIGHT);
		else if (m_weight == fontWeights[ROMAN])
			setWeight(NO_WEIGHT);
		m_setflags |= slantWasSet;
	}
	if (m_slantIndex < 0)
	{
		m_weightIndex = 0;
		m_slantIndex = 1;
		m_widthIndex = 2;
	}
}

QString gtFont::getSlant()
{
	return m_slant;
}

void gtFont::setWidth(FontWidth newWidth)
{
	m_width = fontWidths[newWidth];
	m_useFullName = false;
	m_setflags &= ~widthWasSet;
	if (newWidth != NO_WIDTH)
	{
		if (m_weight == fontWeights[REGULAR])
			setWeight(NO_WEIGHT);
		else if (m_weight == fontWeights[ROMAN])
			setWeight(NO_WEIGHT);
		m_setflags |= widthWasSet;
	}
	if (m_widthIndex < 0)
	{
		m_weightIndex = 0;
		m_slantIndex = 1;
		m_widthIndex = 2;
	}
}

void gtFont::setWidth(QString newWidth)
{
	m_width = newWidth;
	m_useFullName = false;
	m_setflags &= ~widthWasSet;
	if (!newWidth.isEmpty())
	{
		if (m_weight == fontWeights[REGULAR])
			setWeight(NO_WEIGHT);
		else if (m_weight == fontWeights[ROMAN])
			setWeight(NO_WEIGHT);
		m_setflags |= widthWasSet;
	}
	if (m_widthIndex < 0)
	{
		m_weightIndex = 0;
		m_slantIndex = 1;
		m_widthIndex = 2;
	}
}

QString gtFont::getWidth()
{
	return m_width;
}

void gtFont::setSize(int newSize)
{
	m_size = newSize;
	m_setflags |= sizeWasSet;
}

void gtFont::setSize(double newSize)
{
	m_size = static_cast<int>(newSize);
	m_setflags |= sizeWasSet;
}

void gtFont::setColor(QString newColor)
{
	m_color = newColor;
	m_setflags |= fillColorWasSet;
}

void gtFont::setShade(int newShade)
{
	m_shade = newShade;
	m_setflags |= fillShadeWasSet;
}

void gtFont::setStrokeColor(QString newColor)
{
	m_strokeColor = newColor;
	m_setflags |= strokeColorWasSet;
}

void gtFont::setStrokeShade(int newShade)
{
	m_strokeShade = newShade;
	m_setflags |= strokeShadeWasSet;
}

QString gtFont::getName()
{
	if (m_useFullName)
		return m_name;

	QString name2 = m_family + " ";

	if (m_weightIndex == 0)
		name2 += m_weight + " ";
	else if (m_slantIndex == 0) 
		name2 += m_slant + " ";
	else if (m_widthIndex == 0)
		name2 += m_width + " ";

	if (m_weightIndex == 1)
		name2 += m_weight + " ";
	else if (m_slantIndex == 1) 
		name2 += m_slant + " ";
	else if (m_widthIndex == 1)
		name2 += m_width + " ";

	if (m_weightIndex == 2)
		name2 += m_weight + " ";
	else if (m_slantIndex == 2) 
		name2 += m_slant + " ";
	else if (m_widthIndex == 2)
		name2 += m_width + " ";

	name2 += m_append;
	name2 = name2.simplified();
	return name2;
}

QString gtFont::getName(uint i)
{
	QString fname = m_family + " ";
	switch (i)
	{
		case 0:
			fname = fname + m_weight + " " + m_slant + " " + m_width + " " + m_append;
			break;
		case 1:
			fname = fname + m_weight + " " + m_width + " " + m_slant + " " + m_append;
			break;
		case 2:
			fname = fname + m_slant + " " + m_weight + " " + m_width + " " + m_append;
			break;
		case 3:
			fname = fname + m_slant + " " + m_width + " " + m_weight + " " + m_append;
			break;
		case 4:
			fname = fname + m_width + " " + m_weight + " " + m_slant + " " + m_append;
			break;
		case 5:
			fname = fname + m_width + " " + m_slant + " " + m_weight + " " + m_append;
			break;
		case 6:
			fname = fname + " " + m_append + " " + m_weight + " " + m_slant + " " + m_width;
			break;
		case 7:
			fname = fname + " " + m_append + " " + m_weight + " " + m_width + " " + m_slant;
			break;
		case 8:
			fname = fname + " " + m_append + " " + m_slant + " " + m_weight + " " + m_width;
			break;
		case 9:
			fname = fname + " " + m_append + " " + m_slant + " " + m_width + " " + m_weight;
			break;
		case 10:
			fname = fname + " " + m_append + " " + m_width + " " + m_weight + " " + m_slant;
			break;
		case 11:
			fname = fname + " " + m_append + " " + m_width + " " + m_slant + " " + m_weight;
			break;
		case 12:
			if ((m_append.isEmpty()) && (m_weight.isEmpty()) && (m_slant.isEmpty()) && (m_width.isEmpty()))
				fname = fname + " " + fontWeights[REGULAR];
			break;
		case 13:
			if ((m_append.isEmpty()) && (m_weight.isEmpty()) && (m_slant.isEmpty()) && (m_width.isEmpty()))
				fname = fname + " " + fontWeights[ROMAN];
			break;
	}
	fname = fname.simplified();
	return fname;
}

int gtFont::getSize()
{
	return m_size;
}

QString gtFont::getColor()
{
	return m_color;
}

int gtFont::getShade()
{
	return m_shade;
}

QString gtFont::getStrokeColor()
{
	return m_strokeColor;
}

int gtFont::getStrokeShade()
{
	return m_strokeShade;
}

void gtFont::noEffects()
{
	m_fontEffects[NORMAL]        = true;
	m_fontEffects[UNDERLINE]     = false;
	m_fontEffects[STRIKETHROUGH] = false;
	m_fontEffects[SMALL_CAPS]    = false;
	m_fontEffects[SUPERSCRIPT]   = false;
	m_fontEffects[SUBSCRIPT]     = false;
	m_fontEffects[OUTLINE]       = false;
	m_setflags &= ~effectWasSet;
}

int gtFont::getHscale()
{
	return m_hscale;
}

void gtFont::setHscale(int newHscale)
{
	m_hscale = newHscale;
	m_setflags |= hscaleWasSet;
}

int gtFont::getKerning()
{
	return m_kerning;
}

void gtFont::setKerning(int newKerning)
{
	m_kerning = newKerning;
	m_setflags |= kerningWasSet;
}

void gtFont::parseName()
{
	m_smallestIndex = -1;
	m_biggestIndex = - 1;
	m_index = -1;
	m_tmpWeightIndex = -1;
	m_tmpSlantIndex = -1;
	m_tmpWidthIndex = -1;
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
		m_index = m_name.indexOf(fontWeights[i]); // f.e. Demi Bold
		QString tmpWeight = "";
		if ((m_index == -1) && (fontWeights[i].indexOf(" ") != -1) && (fontWeights[i].indexOf(" ") != 1))
		{
			QString fw2 = fontWeights[i];
			fw2.replace(" ", "-"); // f.e. Demi-Bold
			m_index = m_name.indexOf(fw2);
			if (m_index == -1)
			{
				fw2 = fontWeights[i];
				fw2.replace(" ", ""); // f.e. DemiBold
				m_index = m_name.indexOf(fw2);
				if (m_index == -1)
				{
					fw2 = fontWeights[i];
					fw2.replace(" B", " b"); // f.e. Demibold
					fw2.replace(" C", " c");
					fw2.replace(" H", " h");
					fw2.replace(" L", " l");
					fw2.replace(" ", "");
					m_index = m_name.indexOf(fw2);
					if (m_index != -1)
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
		if (m_index != -1)
		{
			m_weight = tmpWeight;
			if (m_smallestIndex == -1 || m_smallestIndex > m_index)
				m_smallestIndex = m_index;
			if ((m_biggestIndex == -1) || (m_biggestIndex < m_index + static_cast<int>(tmpWeight.length()) - 1))
				m_biggestIndex = m_index + tmpWeight.length();
			found = true;
			m_tmpWeightIndex = m_index;
			break;
		}
	}
	if (!found)
		m_weight = fontWeights[NO_WEIGHT];
}

void gtFont::parseSlant()
{
	bool found = false;
	for (int i = 1; i < FontSlantMAX; ++i)
	{
		m_index = m_name.indexOf(fontSlants[i]);
		if (m_index != -1)
		{
			m_slant = fontSlants[i];
			if (m_smallestIndex == -1 || m_smallestIndex > m_index)
				m_smallestIndex = m_index;
			if ((m_biggestIndex == -1) || (m_biggestIndex < m_index + static_cast<int>(m_slant.length()) - 1))
				m_biggestIndex = m_index + m_slant.length();
			found = true;
			m_tmpSlantIndex = m_index;
			break;
		}
	}
	if (!found)
		m_slant = fontSlants[NO_SLANT];
}

void gtFont::parseWidth()
{
	bool found = false;
	for (int i = 1; i < FontWidthMAX; ++i)
	{
		m_index = m_name.indexOf(fontWidths[i]);
		QString tmpWidth = "";
		if ((m_index == -1) && (fontWidths[i].indexOf(" ") != -1) && (fontWidths[i].indexOf(" ") != 1))
		{
			QString fw2 = fontWidths[i];
			fw2.replace(" ", "-");
			m_index = m_name.indexOf(fw2);
			if (m_index == -1)
			{
				fw2 = fontWidths[i];
				fw2.replace(" ", "");
				m_index = m_name.indexOf(fw2);
				if (m_index == -1)
				{
					fw2 = fontWidths[i];
					fw2.replace(" B", " b");
					fw2.replace(" C", " c");
					fw2.replace(" H", " h");
					fw2.replace(" L", " l");
					fw2.replace(" ", "");
					m_index = m_name.indexOf(fw2);
					if (m_index != -1)
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
		if (m_index != -1)
		{
			m_width = tmpWidth;
			if (m_smallestIndex == -1 || m_smallestIndex > m_index)
				m_smallestIndex = m_index;
			if ((m_biggestIndex == -1) || (m_biggestIndex < m_index + static_cast<int>(tmpWidth.length()) - 1))
				m_biggestIndex = m_index + tmpWidth.length();
			found = true;
			m_tmpWidthIndex = m_index;
			break;
		}
	}
	if (!found)
		m_width = fontWidths[NO_WIDTH];
}
	
void gtFont::parseFamily()
{
	if (m_tmpWeightIndex < m_tmpSlantIndex)
	{
		m_weightIndex = 0;
		m_slantIndex  = 1;
		if (m_tmpWidthIndex < m_tmpWeightIndex)
		{
			m_widthIndex = 0;
			m_weightIndex = 1;
			m_slantIndex = 2;
		}
		else if (m_tmpWidthIndex < m_tmpSlantIndex)
		{
			m_widthIndex = 1;
			m_slantIndex = 2;
		}
		else
			m_widthIndex = 2;
	}
	else if (m_tmpWeightIndex > m_tmpSlantIndex)
	{
		m_slantIndex = 0;
		m_weightIndex = 1;
		if (m_tmpWidthIndex < m_tmpSlantIndex)
		{
			m_widthIndex = 0;
			m_slantIndex = 1;
			m_weightIndex = 2;
		}
		else if (m_tmpWidthIndex < m_tmpWeightIndex)
		{
			m_widthIndex = 1;
			m_weightIndex = 2;
		}
		else
			m_widthIndex = 2;
	}
	else
	{
		m_weightIndex = -2;
		m_widthIndex = -1;
		m_slantIndex = 0;
	}

	if (m_smallestIndex == -1)
		m_family = m_name;
	else
		m_family = m_name.left(m_smallestIndex);
	if (m_biggestIndex == -1 || m_biggestIndex >= m_name.length())
		m_append = "";
	else
		m_append = m_name.right(m_name.length() - m_biggestIndex - 1);
	m_family = m_family.trimmed();
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
