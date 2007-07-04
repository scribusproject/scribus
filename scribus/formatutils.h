/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

//Added by Craig Bradney in July 2007
//To be used for basic format detection and checking
//One central place for storing all our extensions we support, etc

#ifndef FORMAT_UTILS_H
#define FORMAT_UTILS_H

#include <QString>

bool extensionIndicatesPDF(const QString &ext);
bool extensionIndicatesEPS(const QString &ext);

#endif
