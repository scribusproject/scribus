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

#include "gtparagraphstyle.h"
#include "scribusstructs.h"

gtParagraphStyle::gtParagraphStyle(const QString& name) :
	gtStyle(name)
{
	init();
}

gtParagraphStyle::gtParagraphStyle(const gtParagraphStyle& p) : gtStyle(p)
{
	defaultStyle    = false;
	lineSpacing     = p.lineSpacing;
	alignment       = p.alignment;
	indent          = p.indent;
	firstLineIndent = p.firstLineIndent;
	spaceAbove      = p.spaceAbove;
	spaceBelow      = p.spaceBelow;
	dropCap         = p.dropCap;
	m_bullet          = p.m_bullet;
	m_bulletStr       = p.m_bulletStr;
	dropCapHeight   = p.dropCapHeight;
	m_numeration      = p.m_numeration;
	m_numFormat       = p.m_numFormat;
	m_numLevel        = p.m_numLevel;
	m_numStart        = p.m_numStart;
	m_numPrefix       = p.m_numPrefix;
	m_numSuffix       = p.m_numSuffix;
	adjToBaseline   = p.adjToBaseline;
	autoLineSpacing = p.autoLineSpacing;
	isVisible       = p.isVisible;
	flags           = p.flags;
}

gtParagraphStyle::gtParagraphStyle(const gtStyle& s) : gtStyle(s)
{
	init();
}

void gtParagraphStyle::init()
{
	defaultStyle    = false;
	lineSpacing     = 15;
	alignment       = LEFT;
	indent          = 0;
	firstLineIndent = 0;
	spaceAbove      = 0;
	spaceBelow      = 0;
	dropCap         = false;
	dropCapHeight   = 2;
	m_bullet          = false;
	m_bulletStr       = QString(QChar(0x2022));
	m_numeration      = false;
	m_numFormat       = 0;
	m_numLevel        = 0;
	m_numStart        = 1;
	m_numPrefix.clear();
	m_numSuffix.clear();
	adjToBaseline   = false;
	autoLineSpacing = false;
	isVisible       = true;
	flags           = 0;
}

QString gtParagraphStyle::target() const
{
	return QString("paragraph");
}

int gtParagraphStyle::getFlags() const
{
	return flags;
}

bool gtParagraphStyle::isDefaultStyle() const
{
	return defaultStyle;
}

void gtParagraphStyle::setDefaultStyle(bool defStyle)
{
	defaultStyle = defStyle;
}

double gtParagraphStyle::getLineSpacing() const
{
	return lineSpacing;
}

void gtParagraphStyle::setLineSpacing(double newLineSpacing)
{
	lineSpacing = newLineSpacing;
	flags |= lineSpacingWasSet;
}

bool gtParagraphStyle::getAutoLineSpacing() const
{
	return autoLineSpacing;
}

void gtParagraphStyle::setAutoLineSpacing(bool newALS)
{
	autoLineSpacing = newALS;
	flags |= autoLineSpacingWasSet;
}

int gtParagraphStyle::getAlignment() const
{
	return alignment;
}

void gtParagraphStyle::setAlignment(Alignment newAlignment)
{
	alignment = newAlignment;
	flags |= alignmentWasSet;
}

void gtParagraphStyle::setAlignment(int newAlignment)
{
	if ((newAlignment > -1) && (newAlignment < AlignmentMAX))
	{
		alignment = newAlignment;
		flags |= alignmentWasSet;
	}
}

double gtParagraphStyle::getIndent() const
{
	return indent;
}

void gtParagraphStyle::setIndent(double newIndent)
{
	indent = newIndent;
	flags |= indentWasSet;
}

double gtParagraphStyle::getFirstLineIndent() const
{
	return firstLineIndent;
}

void gtParagraphStyle::setFirstLineIndent(double newFirstLineIndent)
{
	firstLineIndent = newFirstLineIndent;
	flags |= firstIndentWasSet;
}

double gtParagraphStyle::getSpaceAbove() const
{
	return spaceAbove;
}

void gtParagraphStyle::setSpaceAbove(double newSpaceAbove)
{
	spaceAbove = newSpaceAbove;
	flags |= spaceAboveWasSet;
}

double gtParagraphStyle::getSpaceBelow() const
{
	return spaceBelow;
}

void gtParagraphStyle::setSpaceBelow(double newSpaceBelow)
{
	spaceBelow = newSpaceBelow;
	flags |= spaceBelowWasSet;
}

const QList<ParagraphStyle::TabRecord>& gtParagraphStyle::getTabValues() const
{
	return tabValues;
}

void gtParagraphStyle::setTabValue(double newTabValue, TabType ttype)
{
	ParagraphStyle::TabRecord tb;
	tb.tabPosition = newTabValue;
	tb.tabType = ttype;
	tb.tabFillChar =  QChar();
	tabValues.append(tb);
	flags |= tabValueWasSet;
}

bool gtParagraphStyle::hasDropCap() const
{
	return dropCap;
}

void gtParagraphStyle::setDropCap(bool newDropCap)
{
	dropCap = newDropCap;
	flags |= dropCapWasSet;
}

void gtParagraphStyle::setDropCap(int newHeight)
{
	setDropCap(true);
	dropCapHeight = newHeight;
	flags |= dropCapHeightWasSet;
}

int gtParagraphStyle::getDropCapHeight() const
{
	return dropCapHeight;
}

void   gtParagraphStyle::setDropCapHeight(int newHeight)
{
	dropCapHeight = newHeight;
	flags |= dropCapHeightWasSet;
}

bool gtParagraphStyle::hasBullet() const
{
	return m_bullet;
}

QString  gtParagraphStyle::getBullet() const
{
	return m_bulletStr;
}

void gtParagraphStyle::setBullet(bool newBullet, const QString& str)
{
	m_bullet = newBullet;
	if (str != "")
		m_bulletStr = str;
	else
		m_bulletStr = QString(QChar(0x2022));
	flags |= bulletWasSet;
}

bool gtParagraphStyle::hasNum() const
{
	return m_numeration;
}

void gtParagraphStyle::setNum(bool newNum, int format, int level, int start, const QString& prefix, const QString& suffix)
{
	m_numeration = newNum;
	if (newNum)
	{
		m_numFormat = format;
		m_numLevel = level;
		m_numStart = start;
		m_numPrefix = prefix;
		m_numSuffix = suffix;
	}
	flags |= numWasSet;
}

int gtParagraphStyle::getNumLevel() const
{
	return m_numLevel;
}

int gtParagraphStyle::getNumFormat() const
{
	return m_numFormat;
}

int gtParagraphStyle::getNumStart() const
{
	return m_numStart;
}

QString gtParagraphStyle::getNumPrefix() const
{
	return m_numPrefix;
}

QString gtParagraphStyle::getNumSuffix() const
{
	return m_numSuffix;
}

bool gtParagraphStyle::isAdjToBaseline() const
{
	return adjToBaseline;
}

void gtParagraphStyle::setAdjToBaseline(bool newAdjToBaseline)
{
	adjToBaseline = newAdjToBaseline;
	flags |= adjToBaselineWasSet;
}

void gtParagraphStyle::getStyle(gtStyle* style)
{
	*style = gtStyle(*this);
}
