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

#ifndef GTFONT_H
#define GTFONT_H

#include <vector>

#include <qstring.h>

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
	CONDENSED,
	FontWidthMAX
};

/*
	Font will do the font search in Scribus and in case a font
	cannot be found it will launch the font substitution dialog.
*/
class gtFont
{
	static const QString fontWeights[];
	static const QString fontSlants[];
	static const QString fontWidths[];
private:
	QString name;
	QString family;
	QString weight;
	QString slant;
	QString width;
	int     size;
	bool    fontEffects[FontEffectMAX];
	QString color;
	int     shade;
	QString strokeColor;
	int     strokeShade;
	/* Width of a character in percentages to it's "real width" */
	int     hscale;
	double  kerning;
	void initArrays();
	void parseName();
public:
	void    noEffects();
	bool    isToggled(FontEffect fe);
	bool    toggleEffect(FontEffect fe);
	int     getEffectsValue();
	void	setName(QString newName);
	void    setFamily(QString newFamily);
	void    setWeight(FontWeight newWeight);
	void    setSlant(FontSlant newSlant);
	void    setWidth(FontWidth newWidth);
	void    setWeight(QString newWeight);
	void    setSlant(QString newSlant);
	void    setWidth(QString newWidth);
	void    setSize(int newSize);
	void    setSize(double newSize);
	void    setColor(QString newColor);
	void    setShade(int newShade);
	void    setStrokeColor(QString newColor);
	void    setStrokeShade(int newShade);
	QString getName();
	int     getSize();
	QString getColor();
	int     getShade();
	QString getStrokeColor();
	int     getStrokeShade();
	int     getHscale();
	void    setHscale(int newHscale);
	double  getKerning();
	void    setKerning(double newKerning);
	gtFont();
	gtFont(const gtFont& f);
	~gtFont();
};

#endif // FONT_H
