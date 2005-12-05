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

/*
 * A simple common strings class to reduce the string count and ease
 * the translation process a little.
 *  - Contains untranslated and translated versions of the strings to 
 *    help with reduction of future reliance of translated strings in 
 *    "real" code
 *  - Currently stores GUI strings such as &OK and its translated
 *    version, but not the string without the &.
 *  - BE CAREFUL with context. Simple strings should be stored here
 *    that are obviously a verb or a noun, and if multiple contexts
 *    don't have different translations relating to verb/noun/gender
 *    in some languages. This is the reason basic words have been
 *    done initially.
 */

class SCRIBUS_API CommonStrings : public QObject
{
	Q_OBJECT
public:
	CommonStrings();
	static void languageChange();
	static QString _Apply;
	static QString tr_Apply;
	static QString _Cancel;
	static QString tr_Cancel;
	static QString None;
	static QString trNone;
	static QString _OK;
	static QString tr_OK;
	static QString _Save;
	static QString tr_Save;
	static QString Warning;
	static QString trWarning;
};

#endif
