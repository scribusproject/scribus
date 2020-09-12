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

#ifndef GTPARAGRAPHSTYLE_H
#define GTPARAGRAPHSTYLE_H

#include "scribusapi.h"
#include "gtstyle.h"
#include "sctextstruct.h"

enum Alignment {
	LEFT,
	CENTER,
	RIGHT,
	BLOCK,
	FORCED,
	AlignmentMAX
};

enum TabType {
	LEFT_T,
	RIGHT_T,
	FULL_STOP_T,
	COMMA_T,
	CENTER_T
};

class SCRIBUS_API gtParagraphStyle : public gtStyle
{
public:
	gtParagraphStyle(const QString& name);
	gtParagraphStyle(const gtParagraphStyle& p);
	gtParagraphStyle(const gtStyle& s);
	~gtParagraphStyle() {};

	typedef enum
	{
		lineSpacingWasSet = 1,
		alignmentWasSet = 2,
		indentWasSet = 4,
		firstIndentWasSet = 8,
		spaceAboveWasSet  = 16,
		spaceBelowWasSet = 32,
		tabValueWasSet = 64,
		fillShadeWasSet = 128,
		dropCapWasSet = 256,
		dropCapHeightWasSet = 512,
		adjToBaselineWasSet = 1024,
		autoLineSpacingWasSet  = 2048,
		bulletWasSet = 4096,
		numWasSet = 8192
	} wasSetFlags;

	gtStyle& asGtStyle() { return dynamic_cast<gtStyle&>(*this); }
	const gtStyle& asGtStyle() const { return dynamic_cast<const gtStyle&>(*this); }

	QString target() const override;

	int    getFlags() const;
	bool   isDefaultStyle() const;
	void   setDefaultStyle(bool defStyle);
	double getLineSpacing() const;
	void   setLineSpacing(double newLineSpacing);
	bool   getAutoLineSpacing() const;
	void   setAutoLineSpacing(bool newALS);
	int    getAlignment() const;
	void   setAlignment(Alignment newAlignment);
	void   setAlignment(int newAlignment);
	double getIndent() const;
	void   setIndent(double newIndent);
	double getFirstLineIndent() const;
	void   setFirstLineIndent(double newFirstLineIndent);
	double getSpaceAbove() const;
	void   setSpaceAbove(double newSpaceAbove);
	double getSpaceBelow() const;
	void   setSpaceBelow(double newSpaceBelow);
	const QList<ParagraphStyle::TabRecord>& getTabValues() const;
	void   setTabValue(double newTabValue, TabType ttype = LEFT_T);
	bool   hasDropCap() const;
	void   setDropCap(bool newDropCap);
	void   setDropCap(int newHeight);
	int    getDropCapHeight() const;
	void   setDropCapHeight(int newHeight);
	bool   isAdjToBaseline() const;
	void   setAdjToBaseline(bool newAdjToBaseline);
	void   getStyle(gtStyle* style);
	bool hasBullet() const;
	void setBullet(bool newBullet, const QString& str);
	QString getBullet() const;
	bool hasNum() const;
	void setNum(bool newNum, int format=0, int level=0, int start = 1, const QString& prefix = QString(), const QString& suffix = QString());
	int getNumLevel() const;
	int getNumFormat() const;
	int getNumStart() const;
	QString getNumPrefix() const;
	QString getNumSuffix() const;

protected:
	int    flags { 0 };
	bool   defaultStyle { false };
	double lineSpacing { 15 };
	int    alignment { LEFT };
	double indent { 0.0 };
	double firstLineIndent { 0.0 };
	double spaceAbove { 0.0 };
	double spaceBelow { 0.0 };
	QList<ParagraphStyle::TabRecord> tabValues;
	bool dropCap { false };
	int  dropCapHeight { 2 };
	bool m_bullet { false }; ///< Is style using bullet?
	QString m_bulletStr { QChar(0x2022) }; ///< String used as bullet
	bool m_numeration { false }; ///< Is style using numeration?
	int m_numLevel { 0 }; ///< Level in hierarchical numbering
	int m_numFormat { 0 }; ///< 1_2_3, i_ii_ii, a_b_c
	int m_numStart { 1 }; ///< Numeration starts at number
	QString m_numPrefix; ///< Numeration prefix of style
	QString m_numSuffix; ///< Numeration sufffix of style
	bool adjToBaseline { false };
	bool autoLineSpacing { false };
	bool isVisible { true };

private:
	void init();
};

#endif // GTPARAGRAPHSTYLE_H
