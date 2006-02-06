/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : Aug 2005
	copyright            : (C) 2005 by Craig Bradney
	email                : cbradney@zip.com.au
***************************************************************************/

/***************************************************************************
*                                                                         *
*   ScMW program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/

#ifndef COMMONSTRINGS_H
#define COMMONSTRINGS_H

#include <qobject.h>
#include <qstring.h>
#include "scribusapi.h"

/*! \brief A simple common strings class to reduce the string count and ease
 the translation process a little.
 - Contains untranslated and translated versions of the strings to
   help with reduction of future reliance of translated strings in
   "real" code
 - Currently stores GUI strings such as &OK and its translated
   version, but not the string without the &.
 - BE CAREFUL with context. Simple strings should be stored here
   that are obviously a verb or a noun, and if multiple contexts
   don't have different translations relating to verb/noun/gender
   in some languages. This is the reason basic words have been
   done initially.

 trNone = tr("None") was removed because it's problematic in
   East European langiages.
*/
class SCRIBUS_API CommonStrings : public QObject
{
	Q_OBJECT
public:
	CommonStrings();
	
	/**
	 * Return the translated Page Set string if given the "untranslated" one
	 * @param  "untranslated" string
	 * @return translated string
	 */
	static const QString& translatePageSetString(const QString &);
	/**
	 * Return the translated Page Set Location string if given the "untranslated" one
	 * @param  "untranslated" string
	 * @return translated string
	 */
	static const QString& translatePageSetLocString(const QString &);
	/**
	 * Return the untranslated Page Set string if given the translated one
	 * @param  translated string
	 * @return "untranslated" string
	 */
	static const QString& untranslatePageSetString(const QString &);
	/**
	 * Return the untranslated Page Set Location string if given the translated one
	 * @param  translated string
	 * @return "untranslated" string
	 */
	static const QString& untranslatePageSetLocString(const QString &);
	static void languageChange();
	static QString _Apply;
	static QString tr_Apply;
	static QString _Cancel;
	static QString tr_Cancel;
	static QString None;
	static QString _OK;
	static QString tr_OK;
	static QString _Save;
	static QString tr_Save;
	static QString Warning;
	static QString trWarning;
	
	//Page Size
	static QString customPageSize;
	static QString trCustomPageSize;
	
	//Page Sets
	static QString pageSet1;
	static QString pageSet2;
	static QString pageSet3;
	static QString pageSet4;
	static QString trPageSet1;
	static QString trPageSet2;
	static QString trPageSet3;
	static QString trPageSet4;
	static QString pageLocLeft;
	static QString pageLocMiddle;
	static QString pageLocMiddleLeft;
	static QString pageLocMiddleRight;
	static QString pageLocRight;
	static QString trPageLocLeft;
	static QString trPageLocMiddle;
	static QString trPageLocMiddleLeft;
	static QString trPageLocMiddleRight;
	static QString trPageLocRight;
};

#endif
