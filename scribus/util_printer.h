/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTERUTIL_H
#define PRINTERUTIL_H


#include <QString>
#include <QMap>

#include "scribusapi.h"
#include "scribusstructs.h"

class QStringList;

class SCRIBUS_API PrinterUtil
{
	public:
		PrinterUtil() {};
		~PrinterUtil() {};
		QStringList static getPrinterNames();

#if defined(_WIN32)
		/**
		 * @brief Get the defaults settings for a specified printer (Windows only)
		 *
		 * This function retrieve the default settings for a specified
		 * printer and return true on success
		 * This function is available only on Windows systems
		 *
		 * @param printerName the printer name
		 * @param devModeA an array which will store the DEVMODE structure with printer settings
		 * @return true if default settings were successfully retrieved.
		 */
		static bool getDefaultSettings( QString printerName, QByteArray& devModeA );
		/**
		 * @brief Initialize print options dialog box settings (Windows only) 
		 *
		 * This function initialize the print options dialog box for a specified
		 * printer and return true on success
		 * This function is available only on Windows systems
		 *
		 * @param printerName the printer name
		 * @param devModeA an array storing the DEVMODE structure for the specified printer
		 * @return true if default settings were successfully retrieved.
		 */
		static bool initDeviceSettings( QString printerName, QByteArray& devModeA );
#endif
		/**
		 * @brief Get the 4 minimum page margins for a certain paper size on the given printer
		 *
		 * @param printerName the printer name 
		 * @param pageSize the page size to get the margins for
		 * @param ptsTopMargin the page's top margin in points
		 * @param m_ptsBottomMargin the page's bottom margin in points
		 * @param m_ptsLeftMargin the page's left margin in points
		 * @param m_ptsRightMargin the page's right margin in points
		\retval bool true on success
		 */
		static bool getPrinterMarginValues( const QString& printerName, const QString& pageSize, double& ptsTopMargin, double& m_ptsBottomMargin, double& m_ptsLeftMargin, double& m_ptsRightMargin);
		/**
		 * @brief Get default print engine for a specific printer
		 * @param printerName the printer name
		 * @param toFile if file printing is planned
		 */
		static PrintEngine getDefaultPrintEngine(const QString& printerName, bool toFile);
		/**
		 * @brief Get print engines supported by a specific printer
		 * @param printerName the printer name
		 * @param toFile if file printing is planned
		 */
		static PrintEngineMap getPrintEngineSupport(const QString& printerName, bool toFile);
		/**
		 * @brief Check if a print engine is supported by a specific printer
		 * @param printerName the printer name
		 * @param engine the print engine for which support is to be checked
		 * @param toFile if file printing is planned
		 */
		static bool checkPrintEngineSupport(const QString& printerName, PrintEngine engine, bool toFile);
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
		static bool isPostscriptPrinter( QString printerName );
};

#endif // DRUCK_H
