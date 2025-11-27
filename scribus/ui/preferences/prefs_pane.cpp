/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "prefs_pane.h"

const QString& Prefs_Pane::caption() const
{
	return m_caption;
}

const QString& Prefs_Pane::icon() const
{
	return m_icon;
}
