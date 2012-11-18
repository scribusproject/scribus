/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef PDFIMPORTOPTIONS_H
#define PDFIMPORTOPTIONS_H

#include <QDialog>

namespace Ui {
class PdfImportOptions;
}
class PdfPlug;

class PdfImportOptions : public QDialog
{
	Q_OBJECT
	
public:
	explicit PdfImportOptions(QWidget *parent = 0);
	~PdfImportOptions();
	void setUpOptions(int actPage, int numPages, bool interact, PdfPlug* plug);
	QString getPagesString();

public slots:
	void updatePreview(int pg);
	void createPageNumberRange();

private:
	Ui::PdfImportOptions *ui;
	PdfPlug* m_plugin;
	int m_maxPage;
};

#endif // PDFIMPORTOPTIONS_H
