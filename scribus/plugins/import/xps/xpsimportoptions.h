/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef XPSIMPORTOPTIONS_H
#define XPSIMPORTOPTIONS_H

#include <QDialog>

namespace Ui {
class XpsImportOptions;
}
class XpsPlug;

class XpsImportOptions : public QDialog
{
	Q_OBJECT
	
public:
	explicit XpsImportOptions(QWidget* parent = nullptr);
	~XpsImportOptions();

	void setUpOptions(const QString& fileName, int actPage, int numPages, bool interact);
	QString getPagesString();

public slots:
	void createPageNumberRange();

private:
	Ui::XpsImportOptions *ui { nullptr };
	int m_maxPage { 0 };
};

#endif // PDFIMPORTOPTIONS_H
