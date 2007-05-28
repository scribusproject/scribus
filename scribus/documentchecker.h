/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
	begin                : 2005
	copyright            : (C) 2005 by Franz Schmid
	email                : Franz.Schmid@altmuehlnet.de
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

#ifndef DOCUMENTCHECKER_H
#define DOCUMENTCHECKER_H

#include "scribusapi.h"
class ScribusDoc;

/*! \brief It create a error/warning list for CheckDocument GUI class.
All errors and/or warnings are stored in errorCodes (inheritted QMap
see scribusstructs.h) and parsed into tree view in CheckDocument widgets.
*/
class SCRIBUS_API DocumentChecker
{
	public:
		//! Check the passed document for errors, return true on error found
		static bool checkDocument(ScribusDoc *currDoc);
};

#endif
