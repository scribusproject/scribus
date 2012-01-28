/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
pageitem.cpp  -  description
-------------------
    begin                : Sat Apr 7 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
	***************************************************************************/

/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#ifndef SPECIALCHARS_H
#define SPECIALCHARS_H


#include <QString>
#include "scribusapi.h"

class SCRIBUS_API SpecialChars {
public:
	static QChar OBJECT;
	static QChar TAB;
	static QChar PARSEP;
	static QChar LINEBREAK;
	static QChar COLBREAK;
	static QChar FRAMEBREAK;
	static QChar SHYPHEN;
	static QChar OLD_NBHYPHEN;
	static QChar OLD_NBSPACE;
	static QChar NBHYPHEN;
	static QChar NBSPACE;
	static QChar ZWNBSPACE;
	static QChar ZWSPACE;
	static QChar PAGENUMBER;
	static QChar PAGECOUNT;
	static QChar BLANK;
	
	static bool isBreak(QChar c, bool includeColBreak = true);
	static bool isBreakingSpace(QChar c);
	static bool isExpandingSpace(QChar c);

	enum {
		CJK_FENCE_BEGIN = 0x0001,
		CJK_FENCE_END = 0x0002,
		CJK_NOTOP = 0x0003,
		CJK_HYPHEN = 0x0004,
		CJK_DELMITER = 0x0005,
		CJK_MIDPOINT = 0x0006,
		CJK_PERIOD = 0x0007,
		CJK_COMMA = 0x0008,
		CJK_BETWEEN = 0x0009,
		CJK_PREFIX = 0x000a,
		CJK_POSTFIX = 0x000b,
		CJK_SPACE = 0x000c,
		CJK_KANA = 0x000d,
		CJK_KANJI = 0x000e,

		CJK_CHAR_MASK = 0x00ff,

		CJK_NOBREAK_BEFORE = 0x0100,
		CJK_NOBREAK_AFTER = 0x0200,
	};
	static int getCJKAttr(QChar c);
};

#endif
