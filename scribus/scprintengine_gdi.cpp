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
#include <QByteArray>
#include <QScopedPointer>
using namespace ::std;

#include "scconfig.h"

#ifdef HAVE_ICM
#include <icm.h>
#endif


#include "commonstrings.h"
#include "prefscontext.h"
#include "prefsfile.h"
#include "prefsmanager.h"
#include "pslib.h"
#include "scpage.h"
#include "scpageoutput.h"
#include "scpaths.h"
#include "scpainterex_cairo.h"
#include "scprintengine_gdi.h"
#include "scribus.h"
#include "scribusapp.h"
#include "scribuscore.h"
#include "scribusview.h"
#include "util.h"
#include "util_ghostscript.h"
#include "ui/customfdialog.h"
#include "ui/multiprogressdialog.h"

#include <cairo.h>
#include <cairo-win32.h>

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

ScPrintEngine_GDI::ScPrintEngine_GDI(void) : ScPrintEngine()
{
	m_abort = false;
	m_forceGDI = false;
}

void ScPrintEngine_GDI::setForceGDI(bool force)
{
	m_forceGDI = force;
}

void ScPrintEngine_GDI::resetData(void)
{
	m_abort = false;
	//m_forceGDI = false;
}

bool ScPrintEngine_GDI::print(ScribusDoc& doc, PrintOptions& options)
{
	bool toFile;
	bool success;
	HDC printerDC;
	QString diaSelection, docDir, prefsDocDir;
	QString printerName = options.printer;
	QByteArray devMode  = options.devMode;
	QString fileName;

	if (options.toFile)	
		return false;
	resetData();

	toFile = printerUseFilePort(options.printer);
	if (toFile)
	{
		QString docName = doc.documentFileName();
		diaSelection = docName.right(docName.length() - docName.lastIndexOf("/") - 1);
		diaSelection = diaSelection.left(diaSelection.indexOf("."));
		diaSelection += ".prn";
		PrefsContext* dirs = PrefsManager::instance().prefsFile->getContext("dirs");
		QString prefsDocDir = PrefsManager::instance().documentDir();
		if (!prefsDocDir.isEmpty())
			docDir = dirs->get("winprn", prefsDocDir);
		else
			docDir = ".";
		CustomFDialog dia(doc.scMW()->view, docDir, QObject::tr("Save As"), "Spool Files (*.prn *.ps);;All Files (*)", fdNone);
		dia.setSelection(diaSelection);
		if (dia.exec() == QDialog::Accepted)
		{
			QString selectedFile = dia.selectedFile();
			if (overwrite(doc.scMW()->view, selectedFile))
			{
				dirs->set("winprn", selectedFile.left(selectedFile.lastIndexOf("/")));
				fileName = QDir::toNativeSeparators(selectedFile);
			}
		}
		else
			return true;
	}

	// Set user options in the DEVmode structure
	setDeviceParams(&doc, options, (DEVMODEW*) devMode.data());
		
	// Create the device context
	printerDC = CreateDCW(nullptr, (LPCWSTR) printerName.utf16(), nullptr, (DEVMODEW*) devMode.data());
	if (printerDC)
	{
		success = printPages(&doc, options, printerDC, (DEVMODEW*) devMode.data(), fileName);
		DeleteDC(printerDC);
	}
	else
	{
		qWarning("doPrintPages : the device context could not be created");
		success = false;
	}

	return success;
}

bool ScPrintEngine_GDI::gdiPrintPreview(ScribusDoc* doc, ScPage* page, QImage* image, PrintOptions& options, double scale)
{
	bool success = true;
	HCOLORSPACE hColorSpace  = nullptr;
	int imagew, imageh;
	double scalex = 1, scaley = 1;
	bool rotate = false;

	if (!doc || !page || !image)
		return false;
	resetData();

	// Get page position
	int clipx = static_cast<int>(page->xOffset());
	int clipy = static_cast<int>(page->yOffset());
	int clipw = qRound(page->width());
	int cliph = qRound(page->height());

	// Setup image
	imagew = clipw * scale;
	imageh = cliph * scale;
	*image = QImage(imagew, imageh, QImage::Format_ARGB32_Premultiplied);
	if (image->width() <= 0 || image->height() <= 0)
		return false;

	// Calculate scaling factors and offsets
	scalex = options.mirrorH ? -1.0 : 1.0;
	scaley = options.mirrorV ? -1.0 : 1.0; 
	double dx = - clipx * scalex;
	double dy = - clipy * scaley;
	if (options.mirrorH) dx += clipw;
	if (options.mirrorV) dy += cliph;
	 
	// Create the GDI painters
	ScPageOutput pageOutput(doc, false);
	QRect drawRect(0, 0, imagew, imageh);

	cairo_surface_t* surface = cairo_image_surface_create_for_data(image->bits(), CAIRO_FORMAT_ARGB32, imagew, imageh, imagew*4);
	if (!surface)
		return false;
	cairo_t* context = cairo_create(surface);
	if (!context)
	{
		cairo_surface_destroy(surface);
		return false;
	}
	ScPainterEx_Cairo painter(context, drawRect, doc, !options.useColor);
	
	scalex *= scale;
	scaley *= scale;
	dx *= scale;
	dy *= scale;
	
	// Set the world transformation matrix
	QTransform matrix(scalex, 0.0, 0.0, scaley, dx, dy);
	painter.setWorldMatrix(matrix);

	image->fill(qRgba(255, 255, 255, 255));
	pageOutput.drawPage(page, &painter); 

	return success;
}

bool ScPrintEngine_GDI::printPages(ScribusDoc* doc, PrintOptions& options, HDC printerDC, DEVMODEW* devMode, QString& fileName)
{
	int  jobId;
	QScopedPointer<MultiProgressDialog> progress;
	PrintPageFunc printPageFunc = nullptr;
	bool  success = true;
	WCHAR docName[512];
	DOCINFOW docInfo;
	ScPage* docPage;

	// Test printer for PostScript support and
	// choose appropriate page printing function
	bool psPrint  = isPostscriptPrinter(printerDC);
	bool useGDI   = (!psPrint || m_forceGDI || (options.prnEngine == WindowsGDI));
	printPageFunc = (useGDI) ? &ScPrintEngine_GDI::printPage_GDI : &ScPrintEngine_GDI::printPage_PS;

	// Setup document infos structure
	wcsncpy (docName, (const WCHAR*) doc->documentFileName().utf16(), 511);
	ZeroMemory(&docInfo, sizeof(docInfo));
	docInfo.cbSize = sizeof(docInfo);
	docInfo.lpszDocName = docName;
	docInfo.lpszOutput  = (LPCWSTR) (fileName.length() > 0 ? fileName.utf16() : nullptr);
	docInfo.lpszDatatype = nullptr;
	docInfo.fwType = 0;

	cairo_surface_t* prnSurface = nullptr;
	cairo_t* context            = nullptr;
	if (printPageFunc == &ScPrintEngine_GDI::printPage_GDI)
	{
		prnSurface = cairo_win32_printing_surface_create(printerDC);
		if (!prnSurface)
			return false;
		context = cairo_create(prnSurface);
		if (!context)
		{
			cairo_surface_destroy(prnSurface);
			return false;
		}
	}
	else if ((printPageFunc == &ScPrintEngine_GDI::printPage_PS) && options.outputSeparations)
		printPageFunc = &ScPrintEngine_GDI::printPage_PS_Sep;

	jobId = StartDocW(printerDC, &docInfo);
	if (jobId <= 0)
	{
		AbortDoc(printerDC) ;
		return false;
	}

	bool usingGui = ScCore->usingGUI();
	if (usingGui)
	{
		progress.reset(new MultiProgressDialog(QObject::tr("Printing..."), CommonStrings::tr_Cancel, doc->scMW()));
		progress->setOverallTotalSteps(options.pageNumbers.size());
		progress->setOverallProgress(0);
		connect(progress.data(), SIGNAL(canceled()), this, SLOT(cancelRequested()));
		progress->show();
	}

	for (uint index = 0; index < options.pageNumbers.size(); index++)
	{
		if (usingGui)
			progress->setOverallProgress(index);
		docPage = doc->Pages->at(options.pageNumbers[index] - 1);
		success = (this->*printPageFunc)(doc, docPage, options, printerDC, context);
		ScQApp->processEvents();
		if (!success || m_abort)
			break;
		if (usingGui)
			progress->setOverallProgress(index + 1);
	}

	if (usingGui)
		progress->close();

	if (m_abort)
		AbortDoc(printerDC) ;
	EndDoc(printerDC);

	cairo_destroy(context);
	cairo_surface_destroy(prnSurface);

	return success;
}

bool ScPrintEngine_GDI::printPage_GDI(ScribusDoc* doc, ScPage* page, PrintOptions& options, HDC printerDC, cairo_t* context)
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
	QString inputProfile;
	QString printerProfile;
	HCOLORSPACE hColorSpace = nullptr;
	double scalex = 1, scaley = 1;
	bool rotate = false;

	StartPage(printerDC);

#ifdef HAVE_ICM
	if (isPostscriptPrinter(printerDC))
	{
		success = false;
		QString mProf = doc->prefsData().colorPrefs.DCMSset.DefaultSolidColorRGBProfile;
		QString pProf = doc->prefsData().colorPrefs.DCMSset.DefaultPrinterProfile;
		if (ScCore->MonitorProfiles.contains(mProf) && ScCore->PrinterProfiles.contains(pProf))
		{
			inputProfile   = QDir::toNativeSeparators(ScCore->InputProfiles[mProf]);
			printerProfile = QDir::toNativeSeparators(ScCore->PrinterProfiles[pProf]);
			// Avoid color transform if input and output profile are the same
			if (inputProfile != printerProfile)
			{
				// Setup input color space
				LOGCOLORSPACEW logColorSpace;
				logColorSpace.lcsSize = sizeof(logColorSpace);
				logColorSpace.lcsVersion = 0x400;
				logColorSpace.lcsSignature = LCS_SIGNATURE;
				logColorSpace.lcsCSType = LCS_CALIBRATED_RGB;
				logColorSpace.lcsIntent = LCS_GM_GRAPHICS;
				wcsncpy(logColorSpace.lcsFilename, (const wchar_t*) inputProfile.utf16(), MAX_PATH);
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
				hColorSpace = CreateColorSpaceW(&logColorSpace);
				if (hColorSpace)
				{
					// Setup the input and output profiles for the device context
					if (SetColorSpace(printerDC, hColorSpace) && SetICMProfileW(printerDC, (LPWSTR) printerProfile.utf16()))
					{
						int result = SetICMMode(printerDC, ICM_ON);
						success = (result != 0);
					}
				}
			}
			else
				success = true;
		}
		// Return if color managament could not be setup
		if (!success)
		{
			EndPage(printerDC);
			if (hColorSpace)
				DeleteColorSpace(hColorSpace);
			return false;
		}
	}
#endif

	// Get page position
	int clipx = static_cast<int>(page->xOffset());
	int clipy = static_cast<int>(page->yOffset());
	int clipw = qRound(page->width());
	int cliph = qRound(page->height());

	// Get horizontal and vertical resolution of printer
	logPixelsX = GetDeviceCaps(printerDC, LOGPIXELSX);
	logPixelsY = GetDeviceCaps(printerDC, LOGPIXELSY);

	// Get paper dimensions (in pixels and points)
	physicalWidth   = GetDeviceCaps(printerDC, PHYSICALWIDTH);
	physicalHeight  = GetDeviceCaps(printerDC, PHYSICALHEIGHT);
	physicalWidthP  = physicalWidth / (double) logPixelsX * 72.0;
	physicalHeightP = physicalHeight / (double) logPixelsY * 72.0;

	// Get margins dimensions
	physicalOffsetX = GetDeviceCaps(printerDC, PHYSICALOFFSETX);
	physicalOffsetY = GetDeviceCaps(printerDC, PHYSICALOFFSETY);

	// Calculate scaling factors and offsets
	scalex = options.mirrorH ? -1.0 : 1.0;
	scaley = options.mirrorV ? -1.0 : 1.0; 
	double dx = (physicalWidthP - clipw) / 2.0 - clipx * scalex;
	double dy = (physicalHeightP - cliph) / 2.0 - clipy * scaley;
	if (options.mirrorH) dx += clipw;
	if (options.mirrorV) dy += cliph;
	dx -= (physicalOffsetX / (double) logPixelsX * 72.0);
	dy -= (physicalOffsetY / (double) logPixelsY * 72.0);
	 
	// Create the GDI painter
	MarksOptions marksOptions(options);
	ScPageOutput pageOutput(doc, true, 300, true);
	pageOutput.setMarksOptions(marksOptions);
	
	QRect drawRect(0, 0, physicalWidth, physicalHeight);
	ScPainterEx_Cairo painter(context, drawRect, doc, !options.useColor);
	painter.clear();
	
	scalex *= (logPixelsX / 72.0);
	scaley *= (logPixelsY / 72.0);
	dx     *= (logPixelsX / 72.0);
	dy     *= (logPixelsY / 72.0);
	QTransform matrix(scalex, 0.0, 0.0, scaley, dx, dy);
	painter.setWorldMatrix(matrix);

	pageOutput.drawPage(page, &painter);

	cairo_show_page(context);
	EndPage(printerDC);

	if (hColorSpace)
		DeleteColorSpace(hColorSpace);

	return success;
}

bool ScPrintEngine_GDI::printPage_PS(ScribusDoc* doc, ScPage* page, PrintOptions& options, HDC printerDC, cairo_t* /*context*/)
{
	bool succeed = false;
	PrintOptions options2 = options;
	QString tempFilePath;
	int ret = 0;

	options2.pageNumbers.clear();
	options2.pageNumbers.push_back(page->pageNr() + 1);
	options2.includePDFMarks = false;

	tempFilePath = PrefsManager::instance().preferencesLocation() + "/tmp.ps";
	PSLib *dd = new PSLib(doc, options2, PSLib::OutputEPS);
	ret = dd->createPS(tempFilePath);
	delete dd;
	if (ret != 0) return false;

	if (options.prnEngine == PostScript1 || options.prnEngine == PostScript2)
	{
		QString tmp;
		QStringList opts;
		QString tempFilePath2 = PrefsManager::instance().preferencesLocation() + "/tmp2.ps";
		opts.append( QString("-dDEVICEWIDTHPOINTS=%1").arg(tmp.setNum(doc->pageWidth())));
		opts.append( QString("-dDEVICEHEIGHTPOINTS=%1").arg(tmp.setNum(doc->pageHeight())));
		if (QFile::exists(tempFilePath2))
			QFile::remove(tempFilePath2);
		ret = convertPS2PS(tempFilePath, tempFilePath2, opts, options.prnEngine);
		if (ret == 0)
		{
			QFile::remove(tempFilePath);
			tempFilePath = tempFilePath2;
		}
		else
		{
			QFile::remove(tempFilePath2);
		}
	}
	
	if (ret == 0)
	{
		double bleedH = options.bleeds.left() + options.bleeds.right();
		double bleedV = options.bleeds.top()  + options.bleeds.bottom();
		StartPage(printerDC);
		succeed = sendPSFile(tempFilePath, printerDC, page->width() + bleedH, page->height() + bleedV, (page->orientation() == 1));
		EndPage(printerDC);
	}
	
	QFile::remove(tempFilePath);
	return succeed;
}

bool ScPrintEngine_GDI::printPage_PS_Sep(ScribusDoc* doc, ScPage* page, PrintOptions& options, HDC printerDC, cairo_t* context)
{
	bool succeed = true;
	QStringList separations;
	if (options.separationName != "All")
		separations.append(options.separationName);
	else
		separations += options.allSeparations;
	for (int i = 0; i < separations.count(); ++i)
	{
		PrintOptions tempOptions = options;
		tempOptions.separationName = separations.at(i);
		succeed &= printPage_PS(doc, page, tempOptions, printerDC, context);
		if (!succeed) break;
	}
	return succeed;
}

bool ScPrintEngine_GDI::sendPSFile(QString filePath, HDC printerDC, int pageWidth, int pageHeight, bool landscape)
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
	QFile file(filePath);
	int fileSize = 0;
	int br, bw;

	if (!printerDC)
		return false;
	escape = getPSPassthroughSupport(printerDC);
	if (!escape)
		return false;

	// Get printer resolution
	logPixelsX = GetDeviceCaps(printerDC, LOGPIXELSX);
	logPixelsY = GetDeviceCaps(printerDC, LOGPIXELSY);

	// Get paper dimensions (in point units)
	physicalWidth  = GetDeviceCaps(printerDC, PHYSICALWIDTH) / (double) logPixelsX * 72.0;
	physicalHeight = GetDeviceCaps(printerDC, PHYSICALHEIGHT) / (double) logPixelsY * 72.0;

	// Calculate and set scaling factor
	scalex = logPixelsX / 72.0;
	scaley = -logPixelsY / 72.0;
	sprintf((char*) sps.data, "%0.3f %0.3f scale\n", scalex, scaley);
	sps.numBytes = strlen((char*) sps.data);
	if (ExtEscape(printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, nullptr) <= 0)
		return false;

	// Set some necessary stuff for embedding ps into ps
	QString eBegin = "/b4_Inc_state save def\n";
	eBegin += "/dict_count countdictstack def\n";
	eBegin += "/op_count count 1 sub def\n";
	eBegin += "userdict begin\n";
	eBegin += "/showpage { } def\n";
	eBegin += "0 setgray 0 setlinecap\n";
	eBegin += "1 setlinewidth 0 setlinejoin\n";
	eBegin += "10 setmiterlimit [ ] 0 setdash newpath\n";
	eBegin += "/languagelevel where\n";
	eBegin += "{pop languagelevel\n";
	eBegin += "1 ne\n";
	eBegin += "{false setstrokeadjust false setoverprint\n";
	eBegin += "} if } if\n";
	sprintf((char*) sps.data, "%s", eBegin.toLatin1().data());
	sps.numBytes = strlen((char*) sps.data);
	if (ExtEscape(printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, nullptr) <= 0)
		return false;

	// Match Postscript and GDI coordinate system
	sprintf((char*) sps.data, "0 %0.3f neg translate\n", (double) physicalHeight);
	sps.numBytes = strlen((char*) sps.data);
	if (ExtEscape(printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, nullptr) <= 0)
		return false;

	// In case of landscape printing, pslib will rotate the page
	// we must take that into account
	/*if (landscape)
	{
		sprintf((char*) sps.data, "-90 rotate %0.3f %0.3f translate\n", (double) -pageHeight, 0.0);
		sps.numBytes = strlen((char*) sps.data);
		if (ExtEscape(printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, nullptr) <= 0)
			return false;
		transx = (physicalHeight - pageHeight) / -2.0;
		transy = (physicalWidth  - pageWidth) / 2.0;
	}
	else*/
	{
		transx = (physicalWidth  - pageWidth) / 2.0;
		transy = (physicalHeight - pageHeight) / 2.0;
	}

	// Center the printed page in paper zone
	sprintf((char*) sps.data, "%0.3f %0.3f translate\n", transx, transy);
	sps.numBytes = strlen((char*) sps.data);
	if (ExtEscape(printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, nullptr) <= 0)
		return false;

	sprintf((char*) sps.data, "%s: %s\n", "%%BeginDocument", file.fileName().toLocal8Bit().data());
	sps.numBytes = strlen((char*) sps.data);
	if (ExtEscape(printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, nullptr) <= 0)
		return false;

	if (!file.open(QIODevice::ReadOnly))
		return false;
	fileSize = file.size();
	bw = 0; // bytes written
	br = file.read((char*) sps.data, sizeof(sps.data));
	while (br > 0)
	{
		sps.numBytes = br;
		if (ExtEscape(printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, nullptr) <= 0)
			break;
		bw += br;
		br = file.read((char*) sps.data, sizeof(sps.data));
	}
	file.close();

	sprintf((char*) sps.data, "%s", "\n%%EndDocument\n");
	sps.numBytes = strlen((char*) sps.data);
	if (ExtEscape(printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, nullptr) <= 0)
		return false;

	// Set some necessary stuff for embedding ps into ps
	QString eEnd = "count op_count sub {pop} repeat\n";
	eEnd += "countdictstack dict_count sub {end} repeat\n";
	eEnd += "b4_Inc_state restore\n";
	sprintf((char*) sps.data, "%s", eEnd.toLatin1().data());
	sps.numBytes = strlen((char*) sps.data);
	if (ExtEscape(printerDC, escape, sizeof(sps), (LPCSTR) &sps, 0, nullptr) <= 0)
		return false;

	return ((fileSize == bw) && (br >= 0));
}

void ScPrintEngine_GDI::setDeviceParams(ScribusDoc* doc, PrintOptions& options, DEVMODEW* devMode)
{
	HANDLE handle;
	QString printer = options.printer;

	short nCopies = options.copies;
	devMode->dmCopies  = nCopies;
	devMode->dmFields |= DM_COPIES;

	bool greyscale = !options.useColor;
	if (greyscale)
	{
		devMode->dmDitherType = DMDITHER_GRAYSCALE;
		devMode->dmFields |= DM_DITHERTYPE;
	}

	OpenPrinterW((LPWSTR) printer.utf16(), &handle, nullptr);
	DocumentPropertiesW((HWND) doc->scMW()->winId(), handle, (LPWSTR) printer.utf16(), devMode, devMode, DM_IN_BUFFER | DM_OUT_BUFFER);
	ClosePrinter(handle);
}

QString ScPrintEngine_GDI::getDefaultPrinter(void)
{
	QString defPrinter;
	WCHAR szPrinter[512] = { 0 };
	DWORD buffSize = 512;

	if (GetDefaultPrinterW(szPrinter, &buffSize))
		defPrinter = QString::fromUtf16((const ushort*) szPrinter);
	return defPrinter;
}

bool ScPrintEngine_GDI::isPostscriptPrinter(HDC dc)
{
	int	escapeCode;
	char technology[MAX_PATH] = {0};

	if (!dc)
		return false;
	
	// Test printer support for the POSTSCRIPT_PASSTHROUGH escape (available since win2k)
	escapeCode = POSTSCRIPT_PASSTHROUGH;
	if (ExtEscape(dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR) &escapeCode, 0, nullptr) > 0)
		return true;
	// Test printer support  for the POSTSCRIPT_DATA escape (available since win95)
	escapeCode = POSTSCRIPT_DATA;
	if (ExtEscape(dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR) &escapeCode, 0, nullptr) > 0)
		return true;
	// Test the printer technology
	escapeCode = GETTECHNOLOGY;
	if (ExtEscape(dc, QUERYESCSUPPORT, sizeof(int), (LPCSTR) &escapeCode, 0, nullptr) > 0)
	{
		// If GETTECHNOLOGY is supported, then ... get technology
		if (ExtEscape(dc, GETTECHNOLOGY, 0, nullptr, MAX_PATH, (LPSTR) technology) > 0)
		{
			// Check technology string for postscript word
			strupr(technology);
			if (strstr(technology, "POSTSCRIPT"))
				return true;
		}
	}
	return false;
}

int	 ScPrintEngine_GDI::getPSPassthroughSupport(HDC printerDC)
{
	int	escapeCode;
	char technology[MAX_PATH] = {0};
	if (!printerDC)
		return 0;
	// Test printer support for the POSTSCRIPT_PASSTHROUGH escape (available since win2k)
	escapeCode = POSTSCRIPT_PASSTHROUGH;
	if (ExtEscape(printerDC, QUERYESCSUPPORT, sizeof(int), (LPCSTR) &escapeCode, 0, nullptr) > 0)
		return POSTSCRIPT_PASSTHROUGH;
	// Test printer support for the POSTSCRIPT_DATA escape (available since win95)
	escapeCode = POSTSCRIPT_DATA;
	if (ExtEscape(printerDC, QUERYESCSUPPORT, sizeof(int), (LPCSTR) &escapeCode, 0, nullptr) > 0)
		return POSTSCRIPT_DATA;
	// Test printer support for the PASSTHROUGH escape
	escapeCode = PASSTHROUGH;
	if (ExtEscape(printerDC, QUERYESCSUPPORT, sizeof(int), (LPCSTR) &escapeCode, 0, nullptr) > 0)
		return PASSTHROUGH;
	return 0;
}

bool ScPrintEngine_GDI::printerUseFilePort(QString& printerName)
{
 bool done;
 bool toFile = false;
 HANDLE prnHandle;
 DWORD size = 0;

    done = OpenPrinterW((LPWSTR) printerName.utf16(), &prnHandle, nullptr);
	if (!done)
		return false;
	
	// Get buffer size for the PRINTER_INFO_2 structure
	GetPrinterW(prnHandle, 2, nullptr, 0, &size);
	if (size > 0)
	{
		PRINTER_INFO_2W* pInfos = (PRINTER_INFO_2W*) malloc(size);
		if (pInfos)
		{
			// Get printer informations
			done = GetPrinterW(prnHandle, 2, (LPBYTE) pInfos, size, &size);
			if (done)
			{
				// Get printer port
				WCHAR* pPortName = pInfos->pPortName;
				if (wcsstr(pPortName, L"FILE:"))
					toFile = true;
			}
			free(pInfos);
		}
	}

	ClosePrinter(prnHandle);
	return toFile;
}
