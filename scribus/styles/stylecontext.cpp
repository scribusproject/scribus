/*
 For general Scribus (>=1.3.2) copyright and licensing information please refer
 to the COPYING file provided with the program. Following this notice may exist
 a copyright and/or license notice that predates the release of Scribus 1.3.2
 for which a new license (GPL+exception) is in place.
 */
/***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


#include "stylecontext.h"

void StyleContext::invalidate()
{
	++m_version; 
	if (m_cnt > 0)          // update() can be slow even if there's nothing to signal
		update(); 
}

bool StyleContext::connect(QObject* receiver, const char *member ) const
{
	bool result = const_cast<StyleContext*>(this)->connectObserver(receiver, member);
	if (result)
		++m_cnt;
	return result;
}

bool StyleContext::disconnect(QObject* receiver, const char *member ) const
{
	bool result = const_cast<StyleContext*>(this)->disconnectObserver(receiver, member);
	if (result)
		--m_cnt;
	return result;
}

