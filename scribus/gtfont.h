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

#ifndef GTFONT_H
#define GTFONT_H

#include <vector>

#include <QString>
#include "scribusapi.h"

enum FontEffect {
	NORMAL,
	UNDERLINE,
	STRIKETHROUGH,
	SMALL_CAPS,
	SUPERSCRIPT,
	SUBSCRIPT,
	OUTLINE,
	FontEffectMAX
};

enum FontWeight {
	NO_WEIGHT,
	DEMIBOLD,
	EXTRABLACK,
	EXTRABOLD,
	EXTRAHEAVY,
	EXTRALIGHT,
	SEMIBOLD,
	BLACK,
	BOLD,
	BOOK,
	DEMI,
	HEAVY,
	LIGHT,
	LITE,
	MEDIUM,
	REGULAR,
	ROMAN,
	FontWeightMAX
};

enum FontSlant {
	NO_SLANT,
	ITALIC,
	OBLIQUE,
	FontSlantMAX
};

enum FontWidth {
	NO_WIDTH,
	EXTRACONDENSED,
	SEMICONDENSED,
	ULTRACONDENSED,
	EXTRACOMPRESSED,
	SEMICOMPRESSED,
	ULTRACOMPRESSED,
	CONDENSED,
	COMPRESSED,
	FontWidthMAX
};

/*
	Font will do the font search in Scribus and in case a font
	cannot be found it will launch the font substitution dialog.
*/
class SCRIBUS_API gtFont
{
private:
	int     setflags;
	QString name;
	QString family;
	QString weight;
	QString slant;
	QString width;
	QString append;
	int     size;
	bool    fontEffects[FontEffectMAX];
	QString color;
	int     shade;
	QString strokeColor;
	int     strokeShade;
	/* Width of a character in percentages to it's "real width" */
	int     hscale;
	int  kerning;
	bool useFullName;
	int  weightIndex;
	int  slantIndex;
	int  widthIndex;
	int smallestIndex;
	int biggestIndex;
	int index;
	int tmpWeightIndex;
	int tmpSlantIndex;
	int tmpWidthIndex;
	void initArrays();
	void parseName();
	void parseWeight();
	void parseSlant();
	void parseWidth();
	void parseFamily();
	int  find(const QString& where, const QString& what);

public:

	typedef enum
	{
		familyWasSet = 1,
		weightWasSet = 2,
		slantWasSet = 4,
		widthWasSet = 8,
		sizeWasSet  = 16,
		effectWasSet = 32,
		fillColorWasSet = 64,
		fillShadeWasSet = 128,
		strokeColorWasSet = 256,
		strokeShadeWasSet = 512,
		hscaleWasSet  = 1024,
		kerningWasSet = 2048
	} wasSetFlags;

	static const QString fontWeights[];
	static const QString fontSlants[];
	static const QString fontWidths[];

	void    noEffects();
	bool    isToggled(FontEffect fe);
	bool    toggleEffect(FontEffect fe);
	int     getFlags();
	int     getEffectsValue();
	void	setName(QString newName);
	void    setFamily(QString newFamily);
	QString getFamily();
	void    setWeight(FontWeight newWeight);
	void    setWeight(QString newWeight);
	QString getWeight();
	void    setSlant(FontSlant newSlant);
	void    setSlant(QString newSlant);
	QString getSlant();
	void    setWidth(FontWidth newWidth);
	void    setWidth(QString newWidth);
	QString getWidth();
	void    setSize(int newSize);
	void    setSize(double newSize);
	void    setColor(QString newColor);
	void    setShade(int newShade);
	void    setStrokeColor(QString newColor);
	void    setStrokeShade(int newShade);
	QString getName();
	QString getName(uint i);
	static const int NAMECOUNT = 14;
	int     getSize();
	QString getColor();
	int     getShade();
	QString getStrokeColor();
	int     getStrokeShade();
	int     getHscale();
	void    setHscale(int newHscale);
	int     getKerning();
	void    setKerning(int newKerning);
	gtFont();
	gtFont(const gtFont& f);
	~gtFont();
};

#endif // GTFONT_H
