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

#include "specialchars.h"

QChar SpecialChars::OBJECT       = QChar(25);
QChar SpecialChars::TAB          = QChar(9);
QChar SpecialChars::PARSEP       = QChar(13);
QChar SpecialChars::LINEBREAK    = QChar(28);
QChar SpecialChars::COLBREAK     = QChar(26);
QChar SpecialChars::FRAMEBREAK   = QChar(27);
QChar SpecialChars::SHYPHEN      = QChar(0xAD);
QChar SpecialChars::NBHYPHEN     = QChar(0x2011);
QChar SpecialChars::NBSPACE      = QChar(0xA0);
QChar SpecialChars::OLD_NBHYPHEN = QChar(24);
QChar SpecialChars::OLD_NBSPACE  = QChar(29);
QChar SpecialChars::ZWNBSPACE    = QChar(0x2060);
QChar SpecialChars::ZWSPACE      = QChar(0x200B);
QChar SpecialChars::PAGENUMBER   = QChar(30);
QChar SpecialChars::PAGECOUNT    = QChar(23);
QChar SpecialChars::BLANK        = QChar(32);      // SPACE is some macro on my machine - av


bool SpecialChars::isBreakingSpace(QChar c)
{
	return c == BLANK || c == ZWSPACE;
}

bool SpecialChars::isExpandingSpace(QChar c)
{
	return c == BLANK || c == NBSPACE;
}

bool SpecialChars::isBreak(QChar c, bool includeColBreak)
{
	return (c == PARSEP 
			|| c == LINEBREAK 
			|| c == FRAMEBREAK 
			|| (includeColBreak && c == COLBREAK));
}

int SpecialChars::getCJKAttr(QChar c)
{
	static uchar attr_3000[0x100] = {
		// 0x3000 - 0x3007
		CJK_SPACE,CJK_COMMA,CJK_PERIOD,CJK_NOTOP,CJK_KANJI,CJK_NOTOP,CJK_KANJI,CJK_KANJI,
		// 0x3008 - 0x300f
		CJK_FENCE_BEGIN,CJK_FENCE_END,CJK_FENCE_BEGIN,CJK_FENCE_END,CJK_FENCE_BEGIN,CJK_FENCE_END,CJK_FENCE_BEGIN,CJK_FENCE_END,
		// 0x3010 - 0x3017
		CJK_FENCE_BEGIN,CJK_FENCE_END,CJK_KANJI,CJK_KANJI,CJK_FENCE_BEGIN,CJK_FENCE_END,CJK_FENCE_BEGIN,CJK_FENCE_END,
		// 0x3018 - 0x301f
		CJK_FENCE_BEGIN,CJK_FENCE_END,CJK_FENCE_BEGIN,CJK_FENCE_END,CJK_HYPHEN,CJK_FENCE_BEGIN,CJK_FENCE_END,CJK_FENCE_END,
		// 0x3020 - 0x3027
		CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0x3028 - 0x302f
		CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0x3030 - 0x3037
		CJK_BETWEEN,CJK_BETWEEN,CJK_BETWEEN,CJK_BETWEEN,CJK_BETWEEN,CJK_BETWEEN,CJK_KANJI,CJK_KANJI,
		// 0x3038 - 0x303f
		CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0x3040 - 0x3047
		CJK_KANJI,CJK_NOTOP,CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_NOTOP,
		// 0x3048 - 0x304f
		CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x3050 - 0x3057
		CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x3058 - 0x305f
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x3060 - 0x3067
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x3068 - 0x306f
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x3070 - 0x3077
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x3078 - 0x307f
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x3080 - 0x3087
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_NOTOP,
		// 0x3088 - 0x308f
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_NOTOP,CJK_KANA,
		// 0x3090 - 0x3097
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_NOTOP,CJK_NOTOP,CJK_KANJI,
		// 0x3098 - 0x309f
		CJK_KANJI,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_NOTOP,CJK_NOTOP,CJK_KANA,
		
		// 0x30a0 - 0x30a7
		CJK_KANJI,CJK_NOTOP,CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_NOTOP,
		// 0x30a8 - 0x30af
		CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x30b0 - 0x30b7
		CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x30b8 - 0x30bf
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x30c0 - 0x30c7
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x30c8 - 0x30cf
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x30d0 - 0x30d7
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x30d8 - 0x30df
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,
		// 0x30e0 - 0x30e7
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_NOTOP,CJK_KANA,CJK_NOTOP,
		// 0x30e8 - 0x30ef
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_NOTOP,CJK_KANA,
		// 0x30f0 - 0x30f7
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_KANA,CJK_NOTOP,CJK_NOTOP,CJK_KANA,
		// 0x30f8 - 0x30ff
		CJK_KANA,CJK_KANA,CJK_KANA,CJK_MIDPOINT,CJK_NOTOP,CJK_NOTOP,CJK_NOTOP,CJK_KANA
	};
	
	static uchar attr_ff00[0x61] = {
		// 0xff00 - 0xff07
		CJK_SPACE,CJK_DELMITER,CJK_KANJI,CJK_PREFIX,CJK_PREFIX,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0xff08 - 0xff0f
		CJK_FENCE_BEGIN,CJK_FENCE_END,CJK_KANJI,CJK_KANJI,CJK_COMMA,CJK_BETWEEN,CJK_PERIOD,CJK_KANJI,
		// 0xff10 - 0xff17
		CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0xff18 - 0xff1f
		CJK_KANJI,CJK_KANJI,CJK_MIDPOINT,CJK_MIDPOINT,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_DELMITER,
		// 0xff20 - 0xff27
		CJK_KANJI,CJK_KANJI,CJK_MIDPOINT,CJK_MIDPOINT,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0xff28 - 0xff2f
		CJK_KANJI,CJK_KANJI,CJK_MIDPOINT,CJK_MIDPOINT,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0xff30 - 0xff37
		CJK_KANJI,CJK_KANJI,CJK_MIDPOINT,CJK_MIDPOINT,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0xff38 - 0xff3f
		CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_FENCE_BEGIN,CJK_KANJI,CJK_FENCE_END,CJK_KANJI,CJK_KANJI,
		// 0xff40 - 0xff47
		CJK_KANJI,CJK_KANJI,CJK_MIDPOINT,CJK_MIDPOINT,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0xff48 - 0xff4f
		CJK_KANJI,CJK_KANJI,CJK_MIDPOINT,CJK_MIDPOINT,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0xff50 - 0xff57
		CJK_KANJI,CJK_KANJI,CJK_MIDPOINT,CJK_MIDPOINT,CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_KANJI,
		// 0xff58 - 0xff5f
		CJK_KANJI,CJK_KANJI,CJK_KANJI,CJK_FENCE_BEGIN,CJK_KANJI,CJK_FENCE_END,CJK_BETWEEN,CJK_FENCE_BEGIN,
		// 0xff60
		CJK_FENCE_END
	};
	ushort code = c.unicode();
	if(code >= 0x3100 && code < 0xa000){
		return CJK_KANJI;
	}
	
	int attr = 0;
	if(code >= 0x3000 && code < 0x3100){
		attr = attr_3000[code - 0x3000];
	} else if( code >= 0xff00 && code <= 0xff60 ){
		attr = attr_ff00[code - 0xff00];
	}
	if( attr != 0 ){
		if(attr == CJK_COMMA || attr == CJK_PERIOD || attr == CJK_NOTOP || attr == CJK_FENCE_END )
			attr |= CJK_NOBREAK_BEFORE;
		else if(attr == CJK_FENCE_BEGIN )
			attr |= CJK_NOBREAK_AFTER;
		else if(attr == CJK_MIDPOINT || attr == CJK_HYPHEN || attr == CJK_DELMITER )
			attr |= CJK_NOBREAK_BEFORE;
	}
	return attr;
}
