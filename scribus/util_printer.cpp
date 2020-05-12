/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "util_printer.h"
#include "scconfig.h"
#include <QPrinterInfo>
#include <QPrinter>
#include <QPageLayout>

#if defined(_WIN32)
 #include <windows.h>
 #include <winspool.h>
#endif

#include <QStringList>
#include <QDataStream>
#include <QByteArray>
#include "util.h"
#include "commonstrings.h"

#include "scribuscore.h"

QString PrinterUtil::getDefaultPrinterName()
{
	return QPrinterInfo::defaultPrinterName();
}

QStringList PrinterUtil::getPrinterNames()
{
	return QPrinterInfo::availablePrinterNames();
}

#if defined(_WIN32)
bool PrinterUtil::getDefaultSettings(QString printerName, QByteArray& devModeA)
{
	bool done;
	uint size;
	LONG result = IDOK+1;
	Qt::HANDLE handle = nullptr;
	// Get the printer handle
	done = OpenPrinterW((LPWSTR) printerName.utf16(), &handle, nullptr);
	if (!done)
		return false;
	// Get size of DEVMODE structure (public + private data)
	size = DocumentPropertiesW((HWND) ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), nullptr, nullptr, 0);
	// Allocate the memory needed by the DEVMODE structure
	devModeA.resize(size);
	// Retrieve printer default settings
	result = DocumentPropertiesW((HWND) ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), (DEVMODEW*) devModeA.data(), nullptr, DM_OUT_BUFFER);
	// Free the printer handle
	ClosePrinter(handle);
	return (result == IDOK);
}
#endif

#if defined(_WIN32)
bool PrinterUtil::initDeviceSettings(QString printerName, QByteArray& devModeA)
{
	bool done;
	uint size;
	LONG result = IDOK+1;
	Qt::HANDLE handle = nullptr;
	// Get the printer handle
	done = OpenPrinterW((LPWSTR) printerName.utf16(), &handle, nullptr);
	if (!done)
		return false;
	// Get size of DEVMODE structure (public + private data)
	size = DocumentPropertiesW((HWND) ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), nullptr, nullptr, 0);
	// Compare size with DevMode structure size
	if (devModeA.size() == size)
	{
		// Merge printer settings
		result = DocumentPropertiesW((HWND) ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), (DEVMODEW*) devModeA.data(), (DEVMODEW*) devModeA.data(), DM_IN_BUFFER | DM_OUT_BUFFER);
	}
	else
	{
		// Retrieve default settings
		devModeA.resize(size);
		result = DocumentPropertiesW((HWND) ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), (DEVMODEW*) devModeA.data(), nullptr, DM_OUT_BUFFER);
	}
	done = (result == IDOK);
	// Free the printer handle
	ClosePrinter(handle);
	return done;
}
#endif

bool PrinterUtil::getPrinterMarginValues(const QString& printerName, const QSizeF& pageSize, QMarginsF& margins)
{
	QPrinterInfo pInfo = QPrinterInfo::printerInfo(printerName);
	if (pInfo.isNull())
		return false;

	QPrinter printer(pInfo, QPrinter::HighResolution);
	margins = printer.pageLayout().margins();

	// Unfortunately margin values are not updated when calling QPrinter or QPageLayout's setOrientation()
	// so we have to adapt margin values according to orientation ourselves
	if (pageSize.width() > pageSize.height())
	{
		double l = margins.left();
		double r = margins.right();
		double b = margins.bottom();
		double t = margins.top();
		margins = QMarginsF(b, l, t, r);
	}
	return true;
}

PrintEngine PrinterUtil::getDefaultPrintEngine(const QString&  /*printerName*/, bool toFile)
{
	if (!toFile)
	{
#if defined(_WIN32)
		return WindowsGDI;
#else
		return PostScript3;
#endif
	}
	return PostScript3;
}

PrintEngineMap PrinterUtil::getPrintEngineSupport(const QString& printerName, bool toFile)
{
	PrintEngineMap prnMap;
	if (toFile || PrinterUtil::isPostscriptPrinter(printerName))
	{
		if (ScCore->haveGS())
		{
			prnMap.insert(CommonStrings::trPostScript1, PostScript1);
			prnMap.insert(CommonStrings::trPostScript2, PostScript2);
		}
		prnMap.insert(CommonStrings::trPostScript3, PostScript3);
	}
#if defined(_WIN32)
	if (!toFile)
		prnMap.insert(CommonStrings::trWindowsGDI, WindowsGDI);
#endif
	return prnMap;
}

bool PrinterUtil::checkPrintEngineSupport(const QString& printerName, PrintEngine engine, bool toFile)
{
	bool psSupported = toFile || PrinterUtil::isPostscriptPrinter(printerName);
	if (psSupported && (engine >= PostScript1 && engine <= PostScript3))
		return true;
	if (!psSupported && (engine >= PostScript1 && engine <= PostScript3))
		return false;
	if (engine == WindowsGDI)
	{
#if defined(_WIN32)
		return true; //WindowsGDI
#else
		return false;
#endif
	}
	return false;
}

//Parameter needed on win32..
bool PrinterUtil::isPostscriptPrinter(const QString& printerName)
{
#ifdef _WIN32
	HDC dc;
	int	escapeCode;
	char technology[MAX_PATH] = {0};
	
	// Create the default device context
	dc = CreateDCW(nullptr, (LPCWSTR) printerName.utf16(), nullptr, nullptr);
	if (!dc)
	{
		qWarning("isPostscriptPrinter() failed to create device context for %s", printerName.toLatin1().data());
		return false;
	}
	// test if printer support the POSTSCRIPT_PASSTHROUGH escape code
	escapeCode = POSTSCRIPT_PASSTHROUGH;
	if (ExtEscape(dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR) &escapeCode, 0, nullptr) > 0)
	{
		DeleteDC(dc);
		return true;
	}
	// test if printer support the POSTSCRIPT_DATA escape code
	escapeCode = POSTSCRIPT_DATA;
	if (ExtEscape(dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR) &escapeCode, 0, nullptr) > 0)
	{
		DeleteDC(dc);
		return true;
	}
	// try to get postscript support by testing the printer technology
	escapeCode = GETTECHNOLOGY;
	if (ExtEscape(dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR) &escapeCode, 0, nullptr) > 0)
	{
		// if GETTECHNOLOGY is supported, then ... get technology
		if (ExtEscape(dc, GETTECHNOLOGY, 0, nullptr, MAX_PATH, (LPSTR) technology) > 0)
		{
			// check technology string for postscript word
			strupr(technology);
			if (strstr(technology, "POSTSCRIPT"))
			{
				DeleteDC(dc);
				return true;
			}
		}
	}
	DeleteDC(dc);
	return false;
#else
	return true;
#endif
}
