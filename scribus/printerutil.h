/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTERUTIL_H
#define PRINTERUTIL_H

#include "scribusapi.h"

class QStringList;

class SCRIBUS_API PrinterUtil
{
	public:
		PrinterUtil() {};
		~PrinterUtil() {};
		QStringList getPrinterNames();
};

#endif // DRUCK_H
