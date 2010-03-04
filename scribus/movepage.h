/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MOVEPAGES_H
#define MOVEPAGES_H

#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QDialog>

class QComboBox;
class QLabel;
class QPushButton;
class QSpinBox;

#include "scribusapi.h"

class SCRIBUS_API MovePages : public QDialog
{ 
	Q_OBJECT

public:
	MovePages( QWidget* parent, int currentPage, int maxPages, bool moving );
	~MovePages() {};

	const int getFromPage();
	const int getToPage();
	const int getWhere();
	const int getWherePage();
	const int getCopyCount();


private:
	QLabel* moveLabel;
	QLabel* toLabel;
	QLabel* numberOfCopiesLabel;
	QSpinBox* fromPageData;
	QSpinBox* toPageData;
	QSpinBox* numberOfCopiesData;
	QSpinBox* mvWherePageData;
	QComboBox* mvWhereData;
	QPushButton* okButton;
	QPushButton* cancelButton;
	QVBoxLayout* dialogLayout;
	QGridLayout* fromToLayout;
	QHBoxLayout* okCancelLayout;

	bool move;

private slots:
	virtual void fromChanged();
	virtual void toChanged();
	void mvWherePageDataDisable(int index);
};

#endif // MOVEPAGES_H

