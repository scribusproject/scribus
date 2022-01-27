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
	QString getPagesString() const;
	int getCropBox() const;
	bool croppingEnabled() const;
	bool getImportAsVectors() const;

protected:
	void paintEvent(QPaintEvent *e) override;
	void resizeEvent(QResizeEvent *e) override;

public slots:
	void updateFromCrop();
	void updateFromSpinBox(int pg);
	void updatePreview(int pg);
	void createPageNumberRange();

private:
	Ui::PdfImportOptions *ui { nullptr };
	PdfPlug* m_plugin { nullptr };
	int m_maxPage { 0 };
	bool m_resized { false };

private slots:
	void onOkButtonClicked();
};

#endif // PDFIMPORTOPTIONS_H
