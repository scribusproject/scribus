/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include <QStringList>

#include "formatutils.h"

bool extensionIndicatesPDF(const QString &ext)
{
	QStringList strl;
	strl << "pdf";
	return strl.contains(ext, Qt::CaseInsensitive);
}

bool extensionIndicatesEPS(const QString &ext)
{
	QStringList strl;
	strl << "eps" << "epsf" << "epsi" << "ps";
	return strl.contains(ext, Qt::CaseInsensitive);
}
