/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif

#ifndef _WIN32
#error "This file compiles on win32 platform only!"
#endif

#include <memory>
#include <valarray>
#include <windows.h>
using namespace ::std;

#include <icm.h>
#include <gdiplus.h>
using namespace Gdiplus;

#include "util.h"
#include "gsutil.h"
#include "scwinprint.h"
#include "scwinprint.moc"
#include "scpainterex_gdi.h"
#include "scpageoutput.h"
#include "scribusview.h"
#include "scribusapp.h"
#include "scribus.h"
#include "page.h"

#include "prefsfile.h"
#include "prefscontext.h"
#include "prefsmanager.h"
#include "customfdialog.h"
#include "commonstrings.h"
#include "multiprogressdialog.h"
#include "scpaths.h"
#include "pslib.h"

#ifdef SC_USE_GDIPLUS
ULONG_PTR gdiplusToken;
GdiplusStartupInput gdiplusStartupInput;
void gdiplusBegin(void) { GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); }
void gdiplusEnd(void) { GdiplusShutdown(gdiplusToken); }
#endif

// Calculates fixed point from floating point.
#define __FXPTMANTISSA(d, f)  ( (DWORD)d << f )
#define __FXPTFRACTION(d, f)  ( (DWORD)ldexp((d - (DWORD)d), f) )
#define __FXPT32(d, f)      ( __FXPTMANTISSA(d, f) | __FXPTFRACTION(d, f) )
#define __FXPT2DOT30(d)   __FXPT32(d, 30)
#define __FXPT16DOT16(d)  __FXPT32(d, 16)

// Convenient structure for performing postscript passthrough
typedef struct 
{
	WORD numBytes;
	BYTE  data[32768];
} sPSPassthrough;

// Generic type for page printing function
//typedef bool (ScWinPrint::*PrintPageFunc) ( ScribusDoc* doc, Page* page, PrintOptions& options, HDC printerDC, DEVMODE* devMode );

/*! ScWinPrint Constructor
\param none
\author Jean Ghali
*/
ScWinPrint::ScWinPrint(void)
{
	resetData();
}

/*! ScWinPrint Constructor
\param none
\author Jean Ghali
*/
void ScWinPrint::resetData(void)
{
	m_abort = false;
}

void ScWinPrint::cancelRequested()
{
	m_abort = true;
}

/*! Print a Scribus document
\param doc the document whose pages are to be printer
\param options print options
\param devMode a bytearray containing a DEVMODE structure to use for device context creation
\param forceGDI force use of GDI printing method
\retval return true if no error occured 
\author Jean Ghali
*/
bool ScWinPrint::print( ScribusDoc* doc, PrintOptions& options, QByteArray& devMode, bool forceGDI )
{
	bool toFile;
	bool success;
	HDC printerDC;
	QString diaSelection, docDir, prefsDocDir;
	QCString printerName = options.printer;
	QCString fileName;

	if( !doc || options.toFile )	
		return false;
	resetData();

	toFile = printerUseFilePort( options.printer );
	if ( toFile )
	{
		diaSelection = doc->DocName.right( doc->DocName.length() - doc->DocName.findRev("/") - 1 );
		diaSelection = diaSelection.left( diaSelection.find(".") );
		diaSelection += ".prn";
		PrefsContext* dirs = PrefsManager::instance()->prefsFile->getContext("dirs");
		QString prefsDocDir = PrefsManager::instance()->documentDir();
		if (!prefsDocDir.isEmpty())
			docDir = dirs->get("winprn", prefsDocDir);
		else
			docDir = ".";
		CustomFDialog dia( ScMW->view, docDir, QObject::tr("Save as"), "Spool Files (*.prn *.ps);;All Files (*)", false, false);
		dia.setSelection( diaSelection );
		if (dia.exec() == QDialog::Accepted)
		{
			QString selectedFile = dia.selectedFile();
			if ( overwrite(ScMW->view, selectedFile) )
			{
				dirs->set("winprn", selectedFile.left(selectedFile.findRev("/")));
				selectedFile = QDir::convertSeparators( selectedFile );
				fileName = selectedFile.local8Bit();
			}
		}
		else
			return true;
	}

#ifdef SC_USE_GDIPLUS
	gdiplusBegin();
#endif

	// Set user options in the DEVmode structure
	setDeviceParams( options, (DEVMODE*) devMode.data() );
		
	// Create the device context
	printerDC = CreateDC( NULL, printerName.data(), NULL, (DEVMODE*) devMode.data() );
	if( printerDC )
	{
		success = printPages( doc, options, printerDC, (DEVMODE*) devMode.data(), fileName, forceGDI );
		DeleteDC( printerDC );
	}
	else
	{
		qWarning("doPrintPages : the device context could not be created");
		success = false;
	}

#ifdef SC_USE_GDIPLUS
	gdiplusEnd();
#endif

	return success;
}

/*! Draw print preview to an image
\param doc the document whose page is to be preview
\param page the page to be previewed
\param image the image on which preview is to be drawn
\param options print options
\retval true on success 
\author Jean Ghali
*/
bool ScWinPrint::gdiPrintPreview( ScribusDoc* doc, Page* page, QImage* image, PrintOptions& options, double scale )
{
	HDC dc = NULL;
	bool success = true;
	ScPainterEx_GDI *painter;
	ScPageOutput *pageOutput;
	HCOLORSPACE hColorSpace = NULL;
	int imagew, imageh;
	double scalex = 1, scaley = 1;
	bool rotate = false;

	if ( !doc || !page || !image)
		return false;
	resetData();

#ifdef SC_USE_GDIPLUS
	gdiplusBegin();
#endif

	// Get page position
	int clipx = static_cast<int>(page->xOffset());
	int clipy = static_cast<int>(page->yOffset());
	int clipw = qRound(page->width());
	int cliph = qRound(page->height());

	// Setup image
	imagew = clipw * scale;
	imageh = cliph * scale;
	success = image->create( imagew, imageh, 32 );
	if (!success)
		return false;

	// Create a memory device context
	dc = CreateCompatibleDC( NULL );
	if (!dc )
		return false;

	// Create a DIB Section
	long* bmpData = NULL;
	BITMAPINFO bmpInfo;
	BITMAPINFOHEADER *bmpHeader;
	bmpHeader = &(bmpInfo.bmiHeader);
	bmpHeader->biSize = sizeof(BITMAPINFOHEADER);
	bmpHeader->biWidth = imagew;
	bmpHeader->biHeight = -imageh;
	bmpHeader->biPlanes = 1;
	bmpHeader->biBitCount = 32;
	bmpHeader->biCompression = BI_RGB;
	bmpHeader->biSizeImage = 0; // Valid only if biCompression = BI_RGB
	bmpHeader->biXPelsPerMeter = 0;
	bmpHeader->biYPelsPerMeter = 0;
	bmpHeader->biClrUsed = 0;
	bmpHeader->biClrImportant = 0;
	HBITMAP hBmp = CreateDIBSection(dc, &bmpInfo, DIB_RGB_COLORS, (void** ) &bmpData, NULL, NULL);
	if ( !hBmp )
	{
		DeleteDC(dc);
		return false;
	}

	// And select it into the created dc
	HGDIOBJ obj = SelectObject( dc, hBmp );

	// Get horizontal and vertical resolution of device context
	double logPixelsX = GetDeviceCaps( dc, LOGPIXELSX );
	double logPixelsY = GetDeviceCaps( dc, LOGPIXELSY );

	// Calculate scaling factors and offsets
	scalex = options.mirrorH ? -1.0 : 1.0;
	scaley = options.mirrorV ? -1.0 : 1.0; 
	double dx = - clipx * scalex;
	double dy = - clipy * scaley;
	if ( options.mirrorH ) dx += clipw;
	if ( options.mirrorV ) dy += cliph;

#ifdef SC_USE_GDIPLUS
	// When using gdiplus ScPainterEx_GDI use point units
	scalex *= ( 72.0 / logPixelsX );
	scaley *= ( 72.0 / logPixelsY );
	dx *= ( 72.0 / logPixelsX );
	dy *= ( 72.0 / logPixelsY );
#endif

	scalex *= scale;
	scaley *= scale;
	dx *= scale;
	dy *= scale;
	 
	// Create the GDI painters
	pageOutput = new ScPageOutput(doc, false);
	pageOutput->setScale( 1.0 );
	QRect drawRect( 0, 0, imagew, imageh );
	painter = new ScPainterEx_GDI( dc, drawRect, !options.useColor );
	
	// Set the world transformation matrix
	QWMatrix matrix( scalex, 0.0, 0.0, scaley, dx, dy );
	painter->setWorldMatrix( matrix );

	painter->clear();
	pageOutput->DrawPage(page, painter); 

	delete painter;
	delete pageOutput;

	// Copy DibSection data in QImage
	int* pDib = (int*) bmpData;
	int* pImage = (int*) image->bits();
	int words = imagew * imageh;
	for( int i = 0; i < words; i++ )
	{
		*pImage++ = *pDib;
		pDib++;
	}

	// Delete GDI objects
	SelectObject( dc, obj );
	DeleteDC( dc );
	DeleteObject( hBmp );

#ifdef SC_USE_GDIPLUS
	gdiplusEnd();
#endif

	return success;
}

/*! Print selected pages to a printer
\param doc the document whose pages are to be printer
\param options print options
\printerDC an initialized printer device context
\param devMode the DEVMODE structure used for creating printerDC
\retval true on success 
\author Jean Ghali
*/
bool ScWinPrint::printPages( ScribusDoc* doc, PrintOptions& options, HDC printerDC, DEVMODE* devMode, QCString& fileName, bool forceGDI )
{
 int  jobId;
 bool psPrint;
 auto_ptr<MultiProgressDialog> progress;
 PrintPageFunc doPrintPage = NULL;
 bool success = true;
 char docName[512];
 DOCINFO docInfo;
 Page* docPage;

	// Test printer for PostScript support and
	// choose appropriate page printing function
	psPrint = isPostscriptPrinter( printerDC );
	if ( psPrint && !forceGDI )
		doPrintPage = &ScWinPrint::printPage_PS;
	else
		doPrintPage = &ScWinPrint::printPage_GDI;

	// Setup document infos structure
	strncpy( docName, doc->DocName.local8Bit(), sizeof(docName) - 1);
	ZeroMemory( &docInfo, sizeof(docInfo) );
	docInfo.cbSize = sizeof(docInfo);
	docInfo.lpszDocName = docName;
	docInfo.lpszOutput = ( fileName.size() > 0 ? fileName.data() : NULL );
	docInfo.lpszDatatype = NULL;
	docInfo.fwType = 0;

	jobId = StartDoc( printerDC, &docInfo );
	if ( jobId <= 0 )
	{
		AbortDoc( printerDC ) ;
		return false;
	}

	bool usingGui = ScQApp->usingGUI();
	if ( usingGui )
	{
		progress.reset( new MultiProgressDialog(tr("Printing..."), CommonStrings::tr_Cancel, ScMW, "printprogress") );
		progress->setOverallTotalSteps( options.pageNumbers.size() );
		progress->setOverallProgress(0);
		connect(progress->buttonCancel, SIGNAL(clicked()), this, SLOT(cancelRequested()));
		progress->show();
	}

	for ( uint index = 0; index < options.pageNumbers.size(); index++ )
	{
		if( usingGui )
			progress->setOverallProgress(index);
		docPage = doc->Pages->at( options.pageNumbers[index] - 1 );
		success = (this->*doPrintPage)( doc, docPage, options, printerDC, devMode );
		ScQApp->processEvents();
		if (!success || m_abort )
			break;
		if ( usingGui )
			progress->setOverallProgress(index + 1);
	}

	if ( usingGui )
		progress->close();

	if( m_abort )
		AbortDoc( printerDC ) ;
	EndDoc( printerDC );

	return success;
}

/*! Print a page to a gdi printer
\param doc the document whose page is to be printer
\param page the page to print
\param options print options
\printerDC an initialized printer device context
\devMode the DEVMODE structure used for creating printerDC
\param devMode pointer to a DEVMODE structure
\retval true on success 
\author Jean Ghali
*/
bool ScWinPrint::printPage_GDI( ScribusDoc* doc, Page* page, PrintOptions& options, HDC printerDC, DEVMODE* devMode )
{
	int logPixelsX;
	int logPixelsY;
	int physicalWidth;
	int physicalHeight;
	int physicalWidthP;
	int physicalHeightP;
	int physicalOffsetX;
	int physicalOffsetY;
	bool success = true;
	ScPainterEx_GDI *painter;
	ScPageOutput *pageOutput;
	QCString monitorProfile;
	QCString printerProfile;
	HCOLORSPACE hColorSpace = NULL;
	double scalex = 1, scaley = 1;
	bool rotate = false;

	StartPage( printerDC );

	if ( options.useICC && isPostscriptPrinter(printerDC) )
	{
		success = false;
		QString mProf = doc->CMSSettings.DefaultMonitorProfile;
		QString pProf = doc->CMSSettings.DefaultPrinterProfile;
		if ( ScMW->MonitorProfiles.contains(mProf) && ScMW->PrinterProfiles.contains(pProf) )
		{
			monitorProfile = QDir::convertSeparators(ScMW->MonitorProfiles[mProf]).local8Bit();
			printerProfile = QDir::convertSeparators(ScMW->PrinterProfiles[pProf]).local8Bit();
			// Avoid color transform if input and output profile are the same
			if ( monitorProfile != printerProfile )
			{
				// Setup input color space
				LOGCOLORSPACE logColorSpace;
				logColorSpace.lcsSize = sizeof(logColorSpace);
				logColorSpace.lcsVersion = 0x400;
				logColorSpace.lcsSignature = LCS_SIGNATURE;
				logColorSpace.lcsCSType = LCS_CALIBRATED_RGB;
				logColorSpace.lcsIntent = LCS_GM_GRAPHICS;
				strncpy(logColorSpace.lcsFilename, monitorProfile.data(), MAX_PATH);
				// MSDN recommend to setup reasonable values even if profile is specified
				// so let's use sRGB colorspace values
				logColorSpace.lcsEndpoints.ciexyzRed.ciexyzX = __FXPT2DOT30(0.64);
				logColorSpace.lcsEndpoints.ciexyzRed.ciexyzY = __FXPT2DOT30(0.33);
				logColorSpace.lcsEndpoints.ciexyzRed.ciexyzZ = __FXPT2DOT30(0.03);
				logColorSpace.lcsEndpoints.ciexyzGreen.ciexyzX = __FXPT2DOT30(0.3);
				logColorSpace.lcsEndpoints.ciexyzGreen.ciexyzY = __FXPT2DOT30(0.6);
				logColorSpace.lcsEndpoints.ciexyzGreen.ciexyzZ = __FXPT2DOT30(0.1);
				logColorSpace.lcsEndpoints.ciexyzBlue.ciexyzX = __FXPT2DOT30(0.15);
				logColorSpace.lcsEndpoints.ciexyzBlue.ciexyzY = __FXPT2DOT30(0.06);
				logColorSpace.lcsEndpoints.ciexyzBlue.ciexyzZ = __FXPT2DOT30(0.79);
				logColorSpace.lcsGammaRed = __FXPT16DOT16(0.45);
				logColorSpace.lcsGammaGreen = __FXPT16DOT16(0.45);
				logColorSpace.lcsGammaBlue = __FXPT16DOT16(0.45);
				// Create the color space handle
				hColorSpace = CreateColorSpace( &logColorSpace );
				if ( hColorSpace )
				{
					// Setup the input and output profiles for the device context
					if ( SetColorSpace(printerDC, hColorSpace) && SetICMProfile(printerDC, printerProfile.data()) )
					{
						int result = SetICMMode( printerDC, ICM_ON );
						success = ( result != 0 );
					}
				}
			}
			else
				success = true;
		}
		// Return if color managament could not be setup
		if ( !success )
		{
			EndPage( printerDC );
			if ( hColorSpace )
				DeleteColorSpace( hColorSpace );
			return false;
		}
	}

	// Get page position
	int clipx = static_cast<int>(page->xOffset());
	int clipy = static_cast<int>(page->yOffset());
	int clipw = qRound(page->width());
	int cliph = qRound(page->height());

	// Get horizontal and vertical resolution of printer
	logPixelsX = GetDeviceCaps( printerDC, LOGPIXELSX );
	logPixelsY = GetDeviceCaps( printerDC, LOGPIXELSY );

	// Get paper dimensions ( in pixels and points)
	physicalWidth = GetDeviceCaps( printerDC, PHYSICALWIDTH );
	physicalHeight = GetDeviceCaps( printerDC, PHYSICALHEIGHT );
	physicalWidthP = physicalWidth / (double) logPixelsX * 72.0;
	physicalHeightP = physicalHeight / (double) logPixelsY * 72.0;

	// Get margins dimensions
	physicalOffsetX = GetDeviceCaps( printerDC, PHYSICALOFFSETX );
	physicalOffsetY = GetDeviceCaps( printerDC, PHYSICALOFFSETY );

	// Calculate scaling factors and offsets
	scalex = options.mirrorH ? -1.0 : 1.0;
	scaley = options.mirrorV ? -1.0 : 1.0; 
	double dx = ( physicalWidthP - clipw ) / 2.0 - clipx * scalex;
	double dy = ( physicalHeightP - cliph ) / 2.0 - clipy * scaley;
	if ( options.mirrorH ) dx += clipw;
	if ( options.mirrorV ) dy += cliph;
	dx -= ( physicalOffsetX / (double) logPixelsX * 72.0 );
	dy -= ( physicalOffsetY / (double) logPixelsY * 72.0 );

#ifndef SC_USE_GDIPLUS
	// When using GDI, it's better to draw directly using device coordinates
	// otherwise we may have crappy rendering of bezier curves
	scalex *= ( logPixelsX / 72.0 );
	scaley *= ( logPixelsY / 72.0 );
	dx *= ( logPixelsX / 72.0 );
	dy *= ( logPixelsY / 72.0 );
#endif
	 
	// Create the GDI painter
	pageOutput = new ScPageOutput(doc, true, ScPageOutput::loadImagesAsRGBProof, 300, options.useICC);
	pageOutput->setScale( 1.0 );
	
	QRect drawRect( 0, 0, physicalWidth, physicalHeight);
	painter = new ScPainterEx_GDI( printerDC, drawRect, !options.useColor );
	painter->clear();
	
	QWMatrix matrix( scalex, 0.0, 0.0, scaley, dx, dy );
	painter->setWorldMatrix( matrix );

	pageOutput->DrawPage(page, painter); 

	delete painter;
	delete pageOutput;

	EndPage( printerDC );

	if (hColorSpace)
		DeleteColorSpace( hColorSpace );

	return success;
}

/*! Print a page to a PostScript printer using passthroughs
\param doc the document whose page is to be printer
\param page the page to print
\param options print options
\printerDC an initialized printer device context
\devMode the DEVMODE structure used for creating printerDC
\param devMode pointer to a DEVMODE structure
\retval true on success 
\author Jean Ghali
*/
bool ScWinPrint::printPage_PS ( ScribusDoc* doc, Page* page, PrintOptions& options, HDC printerDC, DEVMODE* devMode )
{
	bool succeed = false;
	ColorList usedColors;
	std::vector<int> pageNumber;
	QMap<QString,int> usedFonts;
	QString tempFilePath;
	int ret = 0;

	doc->getUsedFonts(&usedFonts);
	doc->getUsedColors(usedColors);
	pageNumber.push_back( page->pageNr() + 1 );

	tempFilePath = PrefsManager::instance()->preferencesLocation() + "/tmp.ps";
	PSLib *dd = new PSLib( true, PrefsManager::instance()->appPrefs.AvailFonts, usedFonts, usedColors, false, options.useSpotColors );
	dd->PS_set_file( tempFilePath );
	dd->CreatePS( doc, pageNumber, options.outputSeparations, options.separationName, options.allSeparations, options.useColor, 
				  options.mirrorH, options.mirrorV, options.useICC, options.doGCR, options.setDevParam, options.doClip);
	delete dd;

	if ( options.PSLevel == 1 || options.PSLevel == 2 )
	{
		QString tmp;
		QStringList opts;
		QString tempFilePath2 = PrefsManager::instance()->preferencesLocation() + "/tmp2.ps";
		opts.append( QString("-dDEVICEWIDTHPOINTS=%1").arg(tmp.setNum(doc->pageWidth)) );
		opts.append( QString("-dDEVICEHEIGHTPOINTS=%1").arg(tmp.setNum(doc->pageHeight)) );
		if ( QFile::exists( tempFilePath2 ) )
			QFile::remove( tempFilePath2 );
		ret = convertPS2PS(tempFilePath, tempFilePath2, opts, options.PSLevel);
		if ( ret == 0 )
		{
			unlink( tempFilePath );
			tempFilePath = tempFilePath2;
		}
		else
		{
			QFile::remove( tempFilePath2 );
		}
	}
	
	if( ret == 0 )
	{
		StartPage( printerDC );
		succeed = sendPSFile( tempFilePath, printerDC, page->width(), page->height() );
		EndPage( printerDC );
	}
	
	QFile::remove( tempFilePath );
	return succeed;
}

/*! Send a file to printer using PostScript Passthrough
\param filePath the Postscript file path
\param printerDC the printer deice context
\retval true on success 
\author Jean Ghali
*/
bool ScWinPrint::sendPSFile( QString filePath, HDC printerDC, int pageWidth, int pageHeight  )
{
	int  escape;
	int  logPixelsX;
	int  logPixelsY;
	int  physicalWidth;
	int  physicalHeight;
	bool done = true;
	sPSPassthrough sps;
	double transx, transy;
	double scalex, scaley;
	QFile file( filePath );
	int fileSize = 0;
	int br, bw;

	if (!printerDC)
		return false;
	escape = getPSPassthroughSupport( printerDC );
	if (!escape)
		return false;

	// Get printer resolution
	logPixelsX = GetDeviceCaps(printerDC, LOGPIXELSX);
	logPixelsY = GetDeviceCaps(printerDC, LOGPIXELSY);

	// Get paper dimensions ( in point units )
	physicalWidth = GetDeviceCaps( printerDC, PHYSICALWIDTH ) / (double) logPixelsX * 72.0;
	physicalHeight = GetDeviceCaps( printerDC, PHYSICALHEIGHT ) / (double) logPixelsY * 72.0;

	// Calculate and set scaling factor
	scalex = logPixelsX / 72.0;
	scaley = -logPixelsY / 72.0;
	sprintf( (char*) sps.data, "%0.3f %0.3f scale\n", scalex, scaley );
	sps.numBytes = strlen( (char*) sps.data );
	if( ExtEscape( printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, NULL) <= 0 )
		return false;

	// Center the printed page in paper zone
	//transx = 0;
	//transy = -pageHeight;
	transx = ( physicalWidth - pageWidth ) / 2.0;
	transy = ( pageHeight - physicalHeight ) / 2.0 - pageHeight;
	sprintf( (char*) sps.data, "%0.3f %0.3f translate\n", transx, transy );
	sps.numBytes = strlen( (char*) sps.data );
	if( ExtEscape( printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, NULL) <= 0 )
		return false;

	if ( !file.open( IO_ReadOnly ) )
		return false;
	fileSize = file.size();

	bw = 0; // bytes written
	br = file.readBlock( (char*) sps.data, sizeof( sps.data ) );
	while( br > 0 )
	{
		sps.numBytes = br;
		if( ExtEscape( printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, NULL) > 0 )
			bw += br;
		else
			break;
		br = file.readBlock( (char*) sps.data, sizeof( sps.data ) );
	}

	file.close();

	return ( (fileSize == bw) && ( br >= 0 ) );
}

/*! Set device params into DEVMODE structure according to print options
\param options print options
\param devMode pointer to a DEVMODE structure
\retval none 
\author Jean Ghali
*/
void ScWinPrint::setDeviceParams( PrintOptions& options, DEVMODE* devMode )
{
	HANDLE handle;
	QCString printer = options.printer.local8Bit();
	DWORD devFlags = devMode->dmFields;

	short nCopies = options.copies;
	devMode->dmCopies = nCopies;
	devFlags  = devFlags | DM_COPIES;

	bool greyscale = !options.useColor;
	if( greyscale )
	{
		devMode->dmDitherType = DMDITHER_GRAYSCALE;
		devFlags = devFlags | DM_DITHERTYPE;
	}

	devMode->dmFields = devFlags;

	OpenPrinter( printer.data(), &handle, NULL );
	DocumentProperties( ScMW->winId(), handle, printer.data(), devMode, devMode, DM_IN_BUFFER | DM_OUT_BUFFER);
	ClosePrinter( handle );
}

/*! Get the default printer
\retval the default printer name on success or an empty string 
\author Jean Ghali
*/
QString ScWinPrint::getDefaultPrinter( void )
{
	QString defPrinter;
	OSVERSIONINFO osvi;
	DWORD returned, buffSize;
	char szPrinter[512] = { 0 };
	char* p;
	
	buffSize = sizeof( szPrinter );
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);

	if ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion >= 5 ) // Win2k and later
	{
		if ( GetDefaultPrinter(szPrinter, &buffSize) )
			defPrinter = szPrinter;
	}
	else if( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT && osvi.dwMajorVersion < 5 ) // NT4 or earlier
	{
		if ( GetProfileString("windows","device","", szPrinter, buffSize) < (buffSize - 1) )
		{
			p = szPrinter;
			while (*p != 0 && *p != ',')
				++p;
			*p = 0;
			defPrinter = szPrinter;
		}
	}
	else
	{
		DWORD numPrinters;
		PRINTER_INFO_2* printerInfos = NULL;
		EnumPrinters ( PRINTER_ENUM_DEFAULT, NULL, 2, NULL, 0, &buffSize, &numPrinters );
		printerInfos = (PRINTER_INFO_2*) malloc(buffSize);
		if ( EnumPrinters ( PRINTER_ENUM_LOCAL, NULL, 2, (LPBYTE) printerInfos, buffSize, &buffSize, &returned ) )
		{
			if ( returned > 0 )
			{
				defPrinter = printerInfos->pPrinterName;
			}
		}
		if( printerInfos) free(printerInfos);
	}
	return defPrinter;
}

/*! Check if PostScript is supported by a printer device context
\param dc the printer device context
\retval return true if PostScript is supported 
\author Jean Ghali
*/
bool ScWinPrint::isPostscriptPrinter( HDC dc )
{
	int	escapeCode;
	char technology[MAX_PATH] = {0};

	if (!dc)
		return false;
	
	// Test printer support for the POSTSCRIPT_PASSTHROUGH escape (available since win2k)
	escapeCode = POSTSCRIPT_PASSTHROUGH;
	if ( ExtEscape( dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR)&escapeCode, 0, NULL ) > 0 )
		return true;
	// Test printer support  for the POSTSCRIPT_DATA escape (available since win95)
	escapeCode = POSTSCRIPT_DATA;
	if ( ExtEscape( dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR)&escapeCode, 0, NULL ) > 0 )
		return true;
	// Test the printer technology
	escapeCode = GETTECHNOLOGY;
	if ( ExtEscape( dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR)&escapeCode, 0, NULL ) > 0 )
	{
		// If GETTECHNOLOGY is supported, then ... get technology
		if ( ExtEscape( dc, GETTECHNOLOGY, 0, NULL, MAX_PATH, (LPSTR) technology ) > 0 )
		{
			// Check technology string for postscript word
			strupr( technology );
			if ( strstr( technology, "POSTSCRIPT" ) )
				return true;
		}
	}
	return false;
}

/*! Get support for PostScript Passthrough
\param dc the printer device context
\retval the postscript passthrough escape code if success, 0 if the function fails
\author Jean Ghali
*/
int	 ScWinPrint::getPSPassthroughSupport( HDC printerDC )
{
	int	escapeCode;
	char technology[MAX_PATH] = {0};
	if (!printerDC)
		return 0;
	// Test printer support for the POSTSCRIPT_PASSTHROUGH escape (available since win2k)
	escapeCode = POSTSCRIPT_PASSTHROUGH;
	if ( ExtEscape( printerDC, QUERYESCSUPPORT, sizeof(int), (LPCSTR)&escapeCode, 0, NULL ) > 0 )
		return POSTSCRIPT_PASSTHROUGH;
	// Test printer support for the POSTSCRIPT_DATA escape (available since win95)
	escapeCode = POSTSCRIPT_DATA;
	if ( ExtEscape( printerDC, QUERYESCSUPPORT, sizeof(int), (LPCSTR)&escapeCode, 0, NULL ) > 0 )
		return POSTSCRIPT_DATA;
	// Test printer support for the PASSTHROUGH escape
	escapeCode = PASSTHROUGH;
	if ( ExtEscape( printerDC, QUERYESCSUPPORT, sizeof(int), (LPCSTR)&escapeCode, 0, NULL ) > 0 )
		return PASSTHROUGH;
	return 0;
}

/*! Check if a specified printer use the FILE: port
\param printerName the printer name
\retval return true if the specified printer print to the FILE: port 
\author Jean Ghali
*/
bool ScWinPrint::printerUseFilePort( QString& printerName )
{
 bool done;
 bool toFile = false;
 QCString printer = printerName.local8Bit();
 HANDLE prnHandle;
 DWORD size = 0;

	done = OpenPrinter( printer.data(), &prnHandle, NULL );
	if ( !done )
		return false;
	
	// Get buffer size for the PRINTER_INFO_2 structure
	GetPrinter( prnHandle, 2, NULL, 0, &size );
	if ( size > 0 )
	{
		PRINTER_INFO_2* pInfos = (PRINTER_INFO_2*) malloc(size);
		if( pInfos )
		{
			// Get printer informations
			done = GetPrinter( prnHandle, 2, (LPBYTE) pInfos, size, &size );
			if( done )
			{
				// Get printer port
				char* pPortName = pInfos->pPortName;
				if( strstr(pPortName, "FILE:") )
					toFile = true;
			}
			free(pInfos);
		}
	}

	ClosePrinter( prnHandle );
	return toFile;
}
