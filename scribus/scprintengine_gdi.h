/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef __SCPRINTENGINE_GDI_H__
#define __SCPRINTENGINE_GDI_H__

#include <QByteArray>
#include <QImage>

#include "scconfig.h"
#include "scprintengine.h"
#include "scribusdoc.h"
#include "scribusstructs.h"

#include <cairo.h>
#include <windows.h>

class SCRIBUS_API ScPrintEngine_GDI : public ScPrintEngine
{
protected:

	bool m_forceGDI;

	void resetData(void);

	typedef bool (ScPrintEngine_GDI::*PrintPageFunc) ( ScribusDoc* doc, Page* page, PrintOptions& options, HDC printerDC, cairo_t* context );

	/*! \brief Print selected pages to a printer or a file
	\param doc the document whose pages are to be printer
	\param options print options
	\param printerDC an initialized printer device context
	\param devMode the DEVMODE structure used for creating printerDC
	\param fileName file name to print into
	\param forceGDI optional boolean. false as default.
	\retval bool true on success 
	\author Jean Ghali
	*/
	bool printPages( ScribusDoc* doc, PrintOptions& options, HDC printerDC, DEVMODEW* devMode, QString& fileName );
	/*! \brief Print a page to a gdi printer
	Print a page using GDI drawing code ( works on all printers : PS, PCL, GDI... )
	\param doc the document whose page is to be printer
	\param page the page to print
	\param options print options
	\param printerDC an initialized printer device context
	\param context cairo context (not used by this function)
	\retval bool true on success 
	\author Jean Ghali
	*/
	bool printPage_GDI ( ScribusDoc* doc, Page* page, PrintOptions& options, HDC printerDC, cairo_t* context );
	/*! \brief Print a page to a PostScript printer using passthroughs
	Print a page using PS drawing code and PS passthroughs ( works on PS printers only )
	\param doc the document whose page is to be printer
	\param page the page to print
	\param options print options
	\param printerDC an initialized printer device context
	\param context cairo context (not used by this function)
	\retval bool true on success 
	\author Jean Ghali
	*/
	bool printPage_PS  ( ScribusDoc* doc, Page* page, PrintOptions& options, HDC printerDC, cairo_t* context );
	/*! \brief Send a file to printer using PostScript Passthrough
	Send a postscript file to a printer using ps passthrough ( works on PS printers only )
	\param filePath the Postscript file path
	\param printerDC the printer deice context
	\param pageWidth width
	\param pageHeight height
	\retval bool true on success 
	\author Jean Ghali
	*/
	bool sendPSFile ( QString filePath, HDC printerDC, int pageWidth, int pageHeight );
	/*! \brief Set device params into DEVMODE structure according to print options
	Set printing params according to options and DEVMODE structure
	\param options print options
	\param devMode pointer to a DEVMODE structure
	\author Jean Ghali
	*/
	void setDeviceParams ( ScribusDoc* doc, PrintOptions& options, DEVMODEW* devMode );
	/*! \brief Get support for PostScript Passthrough
	Get ps passthrough support and escape code
	\param printerDC the printer device context
	\retval int the postscript passthrough escape code if success, 0 if the function fails
	\author Jean Ghali
	*/
	int getPSPassthroughSupport( HDC printerDC );
	/*! \brief Check if a specified printer use the FILE: port
	\param printerName the printer name
	\retval bool return true if the specified printer print to the FILE: port 
	\author Jean Ghali
	*/
	bool printerUseFilePort ( QString& printerName );
	/*! \brief Check if PostScript is supported by a printer device context
	\param dc the printer device context
	\retval bool return true if PostScript is supported 
	\author Jean Ghali
	*/
	bool isPostscriptPrinter( HDC dc );

public:

	ScPrintEngine_GDI(void);

	/*! \brief Force use of gdi even on ps printers
	\param force if gdi should be forced
	\author Jean Ghali
	*/
	void setForceGDI(bool force);

	/*! \brief Print a document using gdi or ps methods
	\param doc the document whose pages are to be printer
	\param options print options
	\retval bool return true if no error occured 
	\author Jean Ghali
	*/
	virtual bool print(ScribusDoc& doc, PrintOptions& options);

	/*! \brief Draw print preview to an image using gdi method
	\param doc the document whose page is to be preview
	\param page the page to be previewed
	\param image the image on which preview is to be drawn
	\param options print options
	\param scale optional double value for scaling. 1:1 = 1.0
	\retval bool true on success 
	\author Jean Ghali
	*/
	bool gdiPrintPreview( ScribusDoc* doc, Page* page, QImage* image, PrintOptions& options, double scale = 1.0 );

	/*! \brief Get the default printer name
	\retval QString the default printer name on success or an empty string 
	\author Jean Ghali
	*/
	static QString getDefaultPrinter( void );
};

#endif