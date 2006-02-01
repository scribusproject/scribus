/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTERUTIL_H
#define PRINTERUTIL_H

#include "scribusapi.h"

#include <qstring.h>

class QStringList;

class SCRIBUS_API PrinterUtil
{
	public:
		PrinterUtil() {};
		~PrinterUtil() {};
		QStringList static SCRIBUS_API getPrinterNames();
		bool static SCRIBUS_API getDefaultSettings( QString printerName );
		bool static SCRIBUS_API initDeviceSettings( QString printerName );
		/**
		 * @brief Get the 4 minimum page margins for a certain paper size on the given printer
		 *
		 * @param printerName the printer name
		 * @param pageSize the page size to get the margins for
		 * @param ptsTopMargin the page's top margin in points
		 * @param ptsTopMargin the page's bottom margin in points
		 * @param ptsTopMargin the page's left margin in points
		 * @param ptsTopMargin the page's right margin in points
		 */
		bool static SCRIBUS_API getPrinterMarginValues(const QString& printerName, const QString& pageSize, double& ptsTopMargin, double& m_ptsBottomMargin, double& m_ptsLeftMargin, double& m_ptsRightMargin);
		/**
		 * @brief Check if a specified printer supports postscript input
		 *
		 * On Windows, the function test postscript support for a specified printer
		 * and return true if ps is supported
		 * On non Windows systems, the function always return true
		 *
		 * @param printerName the printer name
		 * @return true is printer support postscript, false otherwise.
		 *
		 */
		bool static SCRIBUS_API isPostscriptPrinter( QString printerName );
};

#endif // DRUCK_H
