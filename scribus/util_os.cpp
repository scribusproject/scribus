/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          util.cpp  -  description
                             -------------------
	begin                : Mon Aug 4 2014
	copyright            : (C) 2014 The Scribus Team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QtGlobal>

bool os_is_osx()
{
#ifdef Q_OS_OSX
	return true;
#else
	return false;
#endif
}

bool os_is_win()
{
#ifdef Q_OS_WIN
	return true;
#else
	return false;
#endif
}

bool os_is_unix()
{
#ifdef Q_OS_UNIX
	return true;
#else
	return false;
#endif
}
