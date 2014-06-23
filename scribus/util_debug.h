/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef UTIL_DEBUG_H
#define UTIL_DEBUG_H

#include <QString>

#include "scribusapi.h"

void SCRIBUS_API sDebug(QString message);

/*! \brief performance measurements.
It prints given message with it current timestamp.
Useful for duration holes finding.
\author Petr Vanek */
void tDebug(QString message);

void SCRIBUS_API printBacktrace(int nFrames);
#endif
