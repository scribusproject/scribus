/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PRINTDIALOG_H
#define PRINTDIALOG_H

#include "scribusapi.h"
#include "scribusstructs.h"

#include <QDialog>
#include "ui_printdialogbase.h"

class PrefsContext;
class ScribusDoc;
class CupsOptions;

class SCRIBUS_API PrintDialog : public QDialog, Ui::PrintDialogBase
{
	Q_OBJECT

public:
	PrintDialog( QWidget* parent, ScribusDoc* doc, QString PDatei, QString PDev, QString PCom, QByteArray& PSettings, bool gcr, QStringList spots);
	~PrintDialog();
	QString printerName();
	QString outputFileName();
	bool outputToFile();
	int numCopies();
	bool outputSeparations();
	QString separationName();
	QStringList allSeparations();
	QString PrinterOpts;
	bool color();
	bool mirrorHorizontal();
	bool mirrorVertical();
	bool doGCR();
	bool doClip();
	PrintEngine printEngine();
	bool doDev();
	bool doSpot();
	bool doOverprint();
	bool ICCinUse();
	bool doPrintAll();
	bool doPrintCurrentPage();
	QString getPageString();
	QByteArray DevMode;

public slots:
	void setMinMax(int min, int max, int cur);

signals:
	void doPreview();

protected slots:
	void doDocBleeds();
	void createPageNumberRange();
	void SetOptions();
	void SelEngine(const QString& eng);
	void SelPrinter(const QString& prn);
	void SelRange(bool e);
	void SelMode(int e);
	void SelFile();
	void SelComm();
	void okButtonClicked();
	void previewButtonClicked();

protected:
	ScribusDoc*  m_doc;
	PrintEngineMap printEngineMap;
	PrefsContext* prefs;
	void setStoredValues(bool gcr);
	CupsOptions *cdia;
	int    unit;
	double unitRatio;
	void   getOptions();
	void   storeValues();
	void   refreshPrintEngineBox();
	void   setPrintEngine(PrintEngine engine);
};

#endif // PRINTDIALOG_H
