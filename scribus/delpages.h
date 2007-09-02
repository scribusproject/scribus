/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef DELPAGES_H
#define DELPAGES_H

#include "scribusapi.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>

class QLabel;
class QPushButton;
class QSpinBox;

class SCRIBUS_API DelPages : public QDialog
{
	Q_OBJECT

public:
	DelPages( QWidget* parent, int currentPage, int maxPage );
	~DelPages() {};

	int getFromPage() const;
	int getToPage() const;

private:
	QVBoxLayout* dialogLayout;
	QHBoxLayout* fromToLayout;
	QHBoxLayout* okCancelLayout;

	QPushButton* cancelButton;
	QPushButton* okButton;
	QLabel* fromLabel;
	QLabel* toLabel;
	QSpinBox* toPageData;
	QSpinBox* fromPageData;

private slots:
	virtual void fromChanged(int pageNumber);
	virtual void toChanged(int pageNumber);
};

#endif // DELPAGES_H
