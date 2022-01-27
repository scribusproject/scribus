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
#include <QStringList>
#include "ui_printdialogbase.h"

class PrefsContext;
class ScribusDoc;
class CupsOptions;

struct PrintOptions;

class SCRIBUS_API PrintDialog : public QDialog, Ui::PrintDialogBase
{
	Q_OBJECT

public:
	PrintDialog( QWidget* parent, ScribusDoc* doc, const PrintOptions& printOptions);
	~PrintDialog();

	QString printerName() const;
	QString outputFileName() const;
	bool outputToFile() const;
	int numCopies() const;
	bool outputSeparations() const;
	QString separationName() const;
	QStringList allSeparations() const;
	bool color() const;
	bool mirrorHorizontal() const;
	bool mirrorVertical() const;
	bool doGCR() const;
	bool doClip() const;
	PrintLanguage printLanguage() const;
	bool doDev() const;
	bool doSpot() const;
	bool doPrintAll() const;
	bool doPrintCurrentPage() const;
	QString getPageString() const;

public slots:
	void setMinMax(int min, int max, int cur);

signals:
	void doPreview();

protected slots:
	void doDocBleeds();
	void createPageNumberRange();
	void selectOptions();
	void selectPrinter(const QString& prn);
	void selectPrintLanguage(const QString& prnLanguage);
	void selectRange(bool e);
	void selectSepMode(int e);
	void selectFile();
	void selectCommand();
	void okButtonClicked();
	void previewButtonClicked();

protected:
	ScribusDoc*    m_doc { nullptr };
	PrefsContext*  prefs { nullptr };
	CupsOptions*   m_cupsOptions { nullptr };
	int            m_unit { 0 };
	double         m_unitRatio { 1 };
	QStringList    m_spotColors;
	
	QByteArray m_devMode; // Buffer for storing storing printer options on Windows

	QString getOptions();
	void storeValues();
	void setPrintLanguage(PrintLanguage engine);
	void setStoredValues(const QString& fileName);
};

#endif // PRINTDIALOG_H
