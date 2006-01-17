#ifndef __SCWINPRINT_H__
#define __SCWINPRINT_H__

#include "qimage.h"
#include "scconfig.h"
#include "scribusdoc.h"
#include "scribusstructs.h"
#include <windows.h>

class ScWinPrint : public QObject
{
	Q_OBJECT

protected:

	bool m_abort;

	void resetData(void);

	typedef bool (ScWinPrint::*PrintPageFunc) ( ScribusDoc* doc, Page* page, PrintOptions& options, HDC printerDC, DEVMODE* devMode );

	/* Print a document to a printer or a file */
	bool printPages( ScribusDoc* doc, PrintOptions& options, HDC printerDC, DEVMODE* devMode, QCString& fileName, bool forceGDI = false );
	/* Print a page using GDI drawing code ( works on all printers : PS, PCL, GDI... )*/
	bool printPage_GDI ( ScribusDoc* doc, Page* page, PrintOptions& options, HDC printerDC, DEVMODE* devMode );
	/* Print a page using PS drawing code and PS passthroughs ( works on PS printers only ) */
	bool printPage_PS  ( ScribusDoc* doc, Page* page, PrintOptions& options, HDC printerDC, DEVMODE* devMode );
	/* Send a postscript file to a printer using ps passthrough ( works on PS printers only ) */
	bool sendPSFile ( QString filePath, HDC printerDC, int pageWidth, int pageHeight );
	/* Set printing params according to options and DEVMODE structure */
	void setDeviceParams ( PrintOptions& options, DEVMODE* devMode );
	/* Get ps passthrough support and escape code*/
	int	 getPSPassthroughSupport( HDC printerDC );
	/* Check if a specified printer print to file port */
	bool printerUseFilePort ( QString& printerName );
	/* Check if a printer support postscript intput */
	bool isPostscriptPrinter( HDC dc );

protected slots:
	void cancelRequested(void);

public:

	ScWinPrint(void);

	/* Print a document using gdi or ps methods */
	bool print( ScribusDoc* doc, PrintOptions& options, QByteArray& devMode, bool forceGDI = false );

	/* Draw print preview to an image using gdi method */
	bool gdiPrintPreview( ScribusDoc* doc, Page* page, QImage* image, PrintOptions& options, double scale = 1.0 );

	/* Get the default printer name */
	static QString getDefaultPrinter( void );
};

#endif