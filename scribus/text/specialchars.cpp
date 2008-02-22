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

