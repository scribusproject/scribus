/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "util_printer.h"
#include "scconfig.h"

#if defined( HAVE_CUPS )
 #include <cups/cups.h>
 #include <cups/ppd.h>
#elif defined(_WIN32)
 #include <windows.h>
 #include <winspool.h>
#endif

#include <QStringList>
#include <QDataStream>
#include <QByteArray>
#include "util.h"
#include "commonstrings.h"
#include "scribus.h"
#include "scribuscore.h"

QStringList PrinterUtil::getPrinterNames()
{
	QString printerName;
	QStringList printerNames;
#if defined (HAVE_CUPS)
	cups_dest_t *dests;
	int num_dests = cupsGetDests(&dests);
	for (int pr = 0; pr < num_dests; ++pr)
	{
		printerName = QString(dests[pr].name);
		printerNames.append(printerName);
	}
	cupsFreeDests(num_dests, dests);
#elif defined(_WIN32)
	DWORD size;
	DWORD numPrinters;
	PRINTER_INFO_2W* printerInfos = NULL;
	EnumPrintersW ( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS , NULL, 2, NULL, 0, &size, &numPrinters );
	printerInfos = (PRINTER_INFO_2W*) malloc(size);
	if ( EnumPrintersW ( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, 2, (LPBYTE) printerInfos, size, &size, &numPrinters ) )
	{
		for ( uint i = 0; i < numPrinters; i++)
		{
			printerName = QString::fromUtf16( (const ushort*) printerInfos[i].pPrinterName );
			printerNames.append(printerName);
		}
		printerNames.sort();	
	}
	if ( printerInfos) free(printerInfos);
#else
	QString tmp;
	QString Pcap;
	QStringList wt;
	if (loadText("/etc/printcap", &Pcap))
	{
		QDataStream ts(&Pcap, QIODevice::ReadOnly);
		while(!ts.atEnd())
		{
			tmp = readLinefromDataStream(ts);
			if (tmp.isEmpty())
				continue;
			if ((tmp[0] != '#') && (tmp[0] != ' ') && (tmp[0] != '\n') && (tmp[0] != '\t'))
			{
				tmp = tmp.trimmed();
				tmp = tmp.left(tmp.length() - (tmp.right(2) == ":\\" ? 2 : 1));
				wt = tmp.split("|", QString::SkipEmptyParts);
				printerName = wt[0];
				printerNames.append(printerName);
			}
		}
	}
#endif
	return printerNames;
}

#if defined(_WIN32)
bool PrinterUtil::getDefaultSettings( QString printerName, QByteArray& devModeA )
{
	bool done;
	uint size;
	LONG result = IDOK+1;
	Qt::HANDLE handle = NULL;
	// Get the printer handle
	done = OpenPrinterW( (LPWSTR) printerName.utf16(), &handle, NULL );
	if(!done)
		return false;
	// Get size of DEVMODE structure (public + private data)
	size = DocumentPropertiesW( ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), NULL, NULL, 0);
	// Allocate the memory needed by the DEVMODE structure
	devModeA.resize( size );
	// Retrieve printer default settings
	result = DocumentPropertiesW( ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), (DEVMODEW*) devModeA.data(), NULL, DM_OUT_BUFFER);
	// Free the printer handle
	ClosePrinter( handle );
	return ( result == IDOK );
}
#endif

#if defined(_WIN32)
bool PrinterUtil::initDeviceSettings( QString printerName, QByteArray& devModeA )
{
	bool done;
	uint size;
	LONG result = IDOK+1;
	Qt::HANDLE handle = NULL;
	// Get the printer handle
	done = OpenPrinterW( (LPWSTR) printerName.utf16(), &handle, NULL );
	if(!done)
		return false;
	// Get size of DEVMODE structure (public + private data)
	size = DocumentPropertiesW( ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), NULL, NULL, 0);
	// Compare size with DevMode structure size
	if( devModeA.size() == size )
	{
		// Merge printer settings
		result = DocumentPropertiesW( ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), (DEVMODEW*) devModeA.data(), (DEVMODEW*) devModeA.data(), DM_IN_BUFFER | DM_OUT_BUFFER);
	}
	else
	{
		// Retrieve default settings
		devModeA.resize( size );
		result = DocumentPropertiesW( ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), (DEVMODEW*) devModeA.data(), NULL, DM_OUT_BUFFER);
	}
	done = ( result == IDOK);
	// Free the printer handle
	ClosePrinter( handle );
	return done;
}
#endif

bool PrinterUtil::getPrinterMarginValues(const QString& printerName, const QString& pageSize, double& ptsTopMargin, double& ptsBottomMargin, double& ptsLeftMargin, double& ptsRightMargin)
{
	bool retVal=false;
#if defined(HAVE_CUPS)
	const char *filename; // tmp PPD filename
	filename=cupsGetPPD(printerName.toLocal8Bit().constData());
	if (filename!=NULL)
	{
		ppd_file_t *ppd; // PPD data
		ppd = ppdOpenFile(filename);
		if (ppd!=NULL)
		{
			ppd_size_t *size; // page size data, null if printer doesnt support selected size
			size = ppdPageSize(ppd, pageSize.toLocal8Bit().constData());
			if (size!=NULL)
			{
				//Store in pts for returning via getNewPrinterMargins in pts
				retVal=true;
				ptsTopMargin=size->length-size->top;
				ptsBottomMargin=size->bottom;
				ptsLeftMargin=size->left;
				ptsRightMargin=size->width-size->right;
			}
			ppdClose(ppd);
		}
	}
#elif defined(_WIN32)
	DWORD nPaper;
	DWORD nPaperNames;
	typedef WCHAR wchar64[64];
	nPaper = DeviceCapabilitiesW( (LPCWSTR) printerName.utf16(), NULL, DC_PAPERS, NULL, NULL );
	nPaperNames = DeviceCapabilitiesW( (LPCWSTR) printerName.utf16(), NULL, DC_PAPERNAMES, NULL, NULL );
	if ( (nPaper > 0) && (nPaperNames > 0) && (nPaper == nPaperNames) )
	{
		int paperIndex = -1;
		DWORD   *papers = new DWORD[nPaper];
		wchar64 *paperNames = new wchar64[nPaperNames];
		DWORD s1 = DeviceCapabilitiesW( (LPCWSTR) printerName.utf16(), NULL, DC_PAPERS, (LPWSTR) papers, NULL );
		DWORD s2 = DeviceCapabilitiesW( (LPCWSTR) printerName.utf16(), NULL, DC_PAPERNAMES, (LPWSTR) paperNames, NULL );
		for ( uint i = 0; i < nPaperNames; i++ )
		{
			if ( pageSize == QString::fromUtf16((const ushort*) paperNames[i]) )
			{
				paperIndex = i;
				break;
			}
		}
		if ( paperIndex >= 0 )
		{
			Qt::HANDLE handle = NULL;
			if( OpenPrinterW( (LPWSTR) printerName.utf16(), &handle, NULL ) )
			{
				// Retrieve DEVMODE structure for selected device
				uint size = DocumentPropertiesW( ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), NULL, NULL, 0);
				QByteArray devModeW(size, 0);
				DEVMODEW* devMode = (DEVMODEW*) devModeW.data();
				DocumentPropertiesW( ScCore->primaryMainWindow()->winId(), handle, (LPWSTR) printerName.utf16(), devMode, NULL, DM_OUT_BUFFER);
				ClosePrinter( handle );
				// Set paper size
				devMode->dmPaperSize = papers[paperIndex];
				// Create device context
				HDC printerDC = CreateDCW( NULL, (LPWSTR) printerName.utf16(), NULL, devMode );
				if( printerDC )
				{
					retVal = true;
					int logPixelsX = GetDeviceCaps( printerDC, LOGPIXELSX );
					int logPixelsY = GetDeviceCaps( printerDC, LOGPIXELSY );
					int physicalOffsetX = GetDeviceCaps( printerDC, PHYSICALOFFSETX );
					int physicalOffsetY = GetDeviceCaps( printerDC, PHYSICALOFFSETY );
					ptsLeftMargin = ptsRightMargin = ( physicalOffsetX / (double) logPixelsX * 72 );
					ptsTopMargin = ptsBottomMargin = ( physicalOffsetY / (double) logPixelsY * 72 );
					DeleteDC(printerDC);
				}
			}
		}
		delete[] papers;
		delete[] paperNames;
	}
#endif
	return retVal;
}

PrintEngine PrinterUtil::getDefaultPrintEngine(const QString& printerName, bool toFile)
{
	if(!toFile)
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
	else if (!psSupported && (engine >= PostScript1 && engine <= PostScript3))
		return false;
	else if (engine == WindowsGDI)
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
bool PrinterUtil::isPostscriptPrinter( QString printerName)
{
#ifdef _WIN32
	HDC dc;
	int	escapeCode;
	char technology[MAX_PATH] = {0};
	
	// Create the default device context
	dc = CreateDCW( NULL, (LPCWSTR) printerName.utf16(), NULL, NULL );
	if ( !dc )
	{
		qWarning("isPostscriptPrinter() failed to create device context for %s", printerName.toAscii().data());
		return false;
	}
	// test if printer support the POSTSCRIPT_PASSTHROUGH escape code
	escapeCode = POSTSCRIPT_PASSTHROUGH;
	if ( ExtEscape( dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR)&escapeCode, 0, NULL ) > 0 )
	{
		DeleteDC( dc );
		return true;
	}
	// test if printer support the POSTSCRIPT_DATA escape code
	escapeCode = POSTSCRIPT_DATA;
	if ( ExtEscape( dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR)&escapeCode, 0, NULL ) > 0 )
	{
		DeleteDC( dc );
		return true;
	}
	// try to get postscript support by testing the printer technology
	escapeCode = GETTECHNOLOGY;
	if ( ExtEscape( dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR)&escapeCode, 0, NULL ) > 0 )
	{
		// if GETTECHNOLOGY is supported, then ... get technology
		if ( ExtEscape( dc, GETTECHNOLOGY, 0, NULL, MAX_PATH, (LPSTR) technology ) > 0 )
		{
			// check technology string for postscript word
			strupr( technology );
			if ( strstr( technology, "POSTSCRIPT" ) )
			{
				DeleteDC( dc );
				return true;
			}
		}
	}
	DeleteDC( dc );
	return false;
#else
	return true;
#endif
}
