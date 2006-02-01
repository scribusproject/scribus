/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#include "printerutil.h"
#include "scconfig.h"

#if defined( HAVE_CUPS )
 #include <cups/cups.h>
#elif defined(_WIN32)
 #include <windows.h>
 #include <winspool.h>
#endif

#include <qstringlist.h>
#include "util.h"
#include "scribus.h"

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
	PRINTER_INFO_2* printerInfos = NULL;
    EnumPrinters ( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS , NULL, 2, NULL, 0, &size, &numPrinters );
	printerInfos = (PRINTER_INFO_2*) malloc(size);
	if ( EnumPrinters ( PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, 2, (LPBYTE) printerInfos, size, &size, &numPrinters ) )
	{
		for ( uint i = 0; i < numPrinters; i++)
		{
			printerName = printerInfos[i].pPrinterName;
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
		QTextStream ts(&Pcap, IO_ReadOnly);
		while(!ts.atEnd())
		{
			tmp = ts.readLine();
			if (tmp.isEmpty())
				continue;
			if ((tmp[0] != '#') && (tmp[0] != ' ') && (tmp[0] != '\n') && (tmp[0] != '\t'))
			{
				tmp = tmp.stripWhiteSpace();
				tmp = tmp.left(tmp.length() - (tmp.right(2) == ":\\" ? 2 : 1));
				wt = QStringList::split("|", tmp);
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
	QCString printer;
	LONG result = IDOK+1;
	HANDLE handle = NULL;
	printer = printerName.local8Bit();
	// Get the printer handle
	done = OpenPrinter( printer.data(), &handle, NULL );
	if(!done)
		return false;
	// Get size of DEVMODE structure (public + private data)
	size = DocumentProperties( ScMW->winId(), handle, printer.data(), NULL, NULL, 0);
	// Allocate the memory needed by the DEVMODE structure
	devModeA.resize( size );
	// Retrieve printer default settings
	result = DocumentProperties( ScMW->winId(), handle, printer.data(), (DEVMODE*) devModeA.data(), NULL, DM_OUT_BUFFER);
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
	QCString printer;
	LONG result = IDOK+1;
	HANDLE handle = NULL;
	printer = printerName.local8Bit();
	// Get the printer handle
	done = OpenPrinter( printer.data(), &handle, NULL );
	if(!done)
		return false;
	// Get size of DEVMODE structure (public + private data)
	size = DocumentProperties( ScMW->winId(), handle, printer.data(), NULL, NULL, 0);
	// Compare size with DevMode structure size
	if( devModeA.size() == size )
	{
		// Merge printer settings
		result = DocumentProperties( ScMW->winId(), handle, printer.data(), (DEVMODE*) devModeA.data(), (DEVMODE*) devModeA.data(), DM_IN_BUFFER | DM_OUT_BUFFER);
	}
	else
	{
		// Retrieve default settings
		devModeA.resize( size );
		result = DocumentProperties( ScMW->winId(), handle, printer.data(), (DEVMODE*) devModeA.data(), NULL, DM_OUT_BUFFER);
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
	filename=cupsGetPPD(printerName);
	if (filename!=NULL)
	{
		ppd_file_t *ppd; // PPD data
		ppd = ppdOpenFile(filename);
		if (ppd!=NULL)
		{
			ppd_size_t *size; // page size data, null if printer doesnt support selected size
			size = ppdPageSize(ppd, pageSize);
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
	QCString printer;
	typedef char char64[64];
	printer = printerName.local8Bit();
	nPaper = DeviceCapabilities( printerName.data(), NULL, DC_PAPERS, NULL, NULL );
	nPaperNames = DeviceCapabilities( printerName.data(), NULL, DC_PAPERNAMES, NULL, NULL );
	if ( (nPaper > 0) && (nPaperNames > 0) && (nPaper == nPaperNames) )
	{
		int paperIndex = -1;
		DWORD *papers = new DWORD[nPaper];
		char64 *paperNames = new char64[nPaperNames];
		DWORD s1 = DeviceCapabilities( printerName.data(), NULL, DC_PAPERS, (LPSTR) papers, NULL );
		DWORD s2 = DeviceCapabilities( printerName.data(), NULL, DC_PAPERNAMES, (LPSTR) paperNames, NULL );
		for ( int i = 0; i < nPaperNames; i++ )
		{
			if ( pageSize == QString(paperNames[i]) )
			{
				paperIndex = i;
				break;
			}
		}
		if ( paperIndex >= 0 )
		{
			HANDLE handle = NULL;
			if( OpenPrinter( printer.data(), &handle, NULL ) )
			{
				// Retrieve DEVMODE structure for selected device
				uint size = DocumentProperties( ScMW->winId(), handle, printer.data(), NULL, NULL, 0);
				QByteArray devModeA(size);
				DEVMODE* devMode = (DEVMODE*) devModeA.data();
				DocumentProperties( ScMW->winId(), handle, printer.data(), devMode, NULL, DM_OUT_BUFFER);
				ClosePrinter( handle );
				// Set paper size
				devMode->dmPaperSize = papers[paperIndex];
				// Create device context
				HDC printerDC = CreateDC( NULL, printer.data(), NULL, devMode );
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

//Parameter needed on win32..
bool PrinterUtil::isPostscriptPrinter( QString printerName)
{
#ifdef _WIN32
	HDC dc;
	int	escapeCode;
	char technology[MAX_PATH] = {0};
	QCString printer = printerName.local8Bit();
	
	// Create the default device context
	dc = CreateDC( NULL, printer.data(), NULL, NULL );
	if ( !dc )
	{
		qWarning( QString("isPostscriptPrinter() failed to create device context for %1").arg(printerName) );
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
