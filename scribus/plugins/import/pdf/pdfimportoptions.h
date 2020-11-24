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
	explicit PdfImportOptions(QWidget* parent = nullptr);
	~PdfImportOptions();
	void setUpOptions(const QString& fileName, int actPage, int numPages, bool interact, bool cropPossible, PdfPlug* plug);
	QString getPagesString();
	int getCropBox();
	bool croppingEnabled();
	bool getImportAsVectors();
	void paintEvent(QPaintEvent *e);

protected:
	void resizeEvent(QResizeEvent *e);

public slots:
	void updateFromCrop();
	void updateFromSpinBox(int pg);
	void updatePreview(int pg);
	void createPageNumberRange();

private:
	Ui::PdfImportOptions *ui;
	PdfPlug* m_plugin;
	int m_maxPage;
	bool m_resized;
};

#endif // PDFIMPORTOPTIONS_H
