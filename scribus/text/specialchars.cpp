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
QChar SpecialChars::LINEBREAK    = QChar(0x2028);
QChar SpecialChars::OLD_LINEBREAK = QChar(28);
QChar SpecialChars::COLBREAK     = QChar(26);
QChar SpecialChars::FRAMEBREAK   = QChar(27);
QChar SpecialChars::SHYPHEN      = QChar(0xAD);
QChar SpecialChars::NBHYPHEN     = QChar(0x2011);
QChar SpecialChars::NBSPACE      = QChar(0xA0);
QChar SpecialChars::OLD_NBHYPHEN = QChar(24);
QChar SpecialChars::OLD_NBSPACE  = QChar(29);
QChar SpecialChars::ZWNBSPACE    = QChar(0x2060);
QChar SpecialChars::ZWSPACE      = QChar(0x200B);
QChar SpecialChars::ZWNJ         = QChar(0x200C);
QChar SpecialChars::ZWJ          = QChar(0x200D);
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

bool SpecialChars::isCJK(uint ch)
{
	if (	(0x2E80 < ch && ch < 0x2EFF)   ||  // CJK Radicals Supplement
		(0x3000 < ch && ch < 0x303F)   ||  // CJK Symbols and Punctuation
		(0x3040 < ch && ch <= 0x30FF)  ||  // Hiragana, Katakana
		(0x31C0 < ch && ch < 0x31EF)   ||  // CJK Strokes
		(0x3200 < ch && ch < 0x32FF)   ||  // Enclosed CJK Letters and Months
		(0x3300 < ch && ch < 0x33FF)   ||  // CJK Compatibility
		(0x3400 < ch && ch < 0x4DBF)   ||  // CJK Unified Ideographs Extension A
		(0x4E00 < ch && ch < 0x9FFF)   ||  // CJK Unified Ideographs
		(0xF900 < ch && ch < 0xFAFF)   ||  // CJK Compatibility Ideographs
		(0xFE30 < ch && ch < 0xFE4F)   ||  // CJK Compatibility Forms
		(0x20000 < ch && ch < 0x2A6DF) ||  // CJK Unified Ideographs Extension B
		(0x2A700 < ch && ch < 0x2B73F) ||  // CJK Unified Ideographs Extension C
		(0x2B740 < ch && ch < 0x2B81F) ||  // CJK Unified Ideographs Extension D
		(0x2F800 < ch && ch < 0x2FA1F) ||  // CJK Compatibility Ideographs Supplement
		(0xFF01 < ch && ch < 0xFF0F)   ||
		(0xFF1A < ch && ch < 0xFF20)   ||
		(0xFF58 < ch && ch < 0xFFDC)   ||
		(ch == 0x3000) ||
		(ch == 0x3002) ||
		(ch == 0x201C) ||
		(ch == 0x201D))
		return true;
	else
		return false;
}


bool SpecialChars::isLetterRequiringSpaceAroundCJK(uint ch) {
	return (0x0030 <= ch && ch <= 0x0039) ||  // ASCII digits
		   (0x0041 <= ch && ch <= 0x005A) ||  // Latin uppercase alphabet
		   (0x0061 <= ch && ch <= 0x007A) ||  // Latin lowercase alphabet
		   (0x00C0 <= ch && ch <= 0x00D6) ||  // Letters in Latin-1 Supplement
		   (0x00D8 <= ch && ch <= 0x00F6) ||  // Letters in Latin-1 Supplement
		   (0x00F8 <= ch && ch <= 0x00FF) ||  // Letters in Latin-1 Supplement
		   (0x0100 <= ch && ch <= 0x017F) ||  // Letters in Latin Extended-A
		   (0x0180 <= ch && ch <= 0x024F) ||  // Letters in Latin Extended-B
		   (0x0250 <= ch && ch <= 0x02AF) ||  // Letters in IPA Extensions
		   (0x1D00 <= ch && ch <= 0x1D25) ||  // Letters in Phonetic Extentions
		   (0x1D6B <= ch && ch <= 0x1D70) ||  // Letters in Phonetic Extentions
		   (0x1D80 <= ch && ch <= 0x1D9A) ||  // Letters in Phonetic Extentions Supplement
		   (0x1E02 <= ch && ch <= 0x1EF3) ||  // Letters in Latin Extended Additional
		   (0x2C60 <= ch && ch <= 0x2C7F) ||  // Letters in Latin Extended-C
		   (0xA722 <= ch && ch <= 0xA787) ||  // Letters in Latin Extended-D
		   (0xA78B <= ch && ch <= 0xA7AE) ||  // Letters in Latin Extended-D
		   (0xA7B0 <= ch && ch <= 0xA7B7) ||  // Letters in Latin Extended-D
		   (0xA7F7 <= ch && ch <= 0xA7BF) ||  // Letters in Latin Extended-D
		   (0xAB30 <= ch && ch <= 0xAB5A) ||  // Letters in Latin Extended-D
		   (0xAB5C <= ch && ch <= 0xAB64) ||  // Letters in Latin Extended-D
		   (0xFB00 <= ch && ch <= 0xFB06) ;   // Alphabetic Presentation Forms
}


bool SpecialChars::isIgnorableCodePoint(uint ch)
{
	// based on list of Default_Ignorable_Code_Point in Unicode 9
	if ( (ch == 0x00AD)					  || //SOFT HYPHEN
		 (ch == 0x034F)					  || //COMBINING GRAPHEME JOINER
		 (ch == 0x061C)					  || //ARABIC LETTER MARK
		 (ch == 0x115F) || (ch == 0x1160) || //HANGUL CHOSEONG FILLER..HANGUL JUNGSEONG FILLER
		 (ch == 0x17B4) || (ch == 0x17B5) || //KHMER VOWEL INHERENT AQ..KHMER VOWEL INHERENT AA
		 (ch >= 0x180B && ch <= 0x180D)	  || //MONGOLIAN FREE VARIATION SELECTOR ONE..MONGOLIAN FREE VARIATION SELECTOR THREE
		 (ch == 0x180E)					  || // MONGOLIAN VOWEL SEPARATOR
		 (ch >= 0x200B && ch <= 0x200F)   || //ZERO WIDTH SPACE..RIGHT-TO-LEFT MARK
		 (ch >= 0x202A && ch <= 0x202E)   || //LEFT-TO-RIGHT EMBEDDING..RIGHT-TO-LEFT OVERRIDE
		 (ch >= 0x2060 && ch <= 0x2064)   || //WORD JOINER..INVISIBLE PLUS
		 (ch >= 0x2066 && ch <= 0x206F)   || //LEFT-TO-RIGHT ISOLATE..NOMINAL DIGIT SHAPES
		 (ch == 0x3164)					  || //HANGUL FILLER
		 (ch >= 0xFE00 && ch <= 0xFE0F)   || //VARIATION SELECTOR-1..VARIATION SELECTOR-16
		 (ch == 0xFEFF)					  || //ZERO WIDTH NO-BREAK SPACE
		 (ch == 0xFFA0)					  || //HALFWIDTH HANGUL FILLER
		 (ch >= 0x1BCA0 && ch <= 0x1BCA3) || //SHORTHAND FORMAT LETTER OVERLAP..SHORTHAND FORMAT UP STEP
		 (ch >= 0x1D173 && ch <= 0x1D17A) || //MUSICAL SYMBOL BEGIN BEAM..MUSICAL SYMBOL END PHRASE
		 (ch == 0xE0001)					 //LANGUAGE TAG
			)
		return true;
	else
		return false;
}

bool SpecialChars::isArabicModifierLetter(uint ch)
{
	if (ch == 0x0640) //ARABIC TATWEEL
		return true;
	else if (ch == 0x06E5) //ARABIC SMALL WAW
		return true;
	else if (ch == 0x06E6) //ARABIC SMALL YEH
		return true;
	return false;
}
