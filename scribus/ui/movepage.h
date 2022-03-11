/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MOVEPAGES_H
#define MOVEPAGES_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

class QComboBox;
class QLabel;
class QPushButton;
class ScrSpinBox;

#include "scribusapi.h"

class SCRIBUS_API MovePages : public QDialog
{ 
	Q_OBJECT

public:
	MovePages( QWidget* parent, int currentPage, int maxPages, bool moving );
	~MovePages() = default;

	int getFromPage();
	int getToPage();
	int getWhere();
	int getWherePage();
	int getCopyCount();


private:
	QLabel* moveLabel;
	QLabel* toLabel;
	QLabel* numberOfCopiesLabel;
	ScrSpinBox* fromPageData;
	ScrSpinBox* toPageData;
	ScrSpinBox* numberOfCopiesData;
	ScrSpinBox* mvWherePageData;
	QComboBox* mvWhereData;
	QDialogButtonBox* buttonBox;
	QVBoxLayout* dialogLayout;
	QGridLayout* fromToLayout;

	bool move;

private slots:
	virtual void fromChanged();
	virtual void toChanged();
	void mvWherePageDataDisable(int index);
};

#endif // MOVEPAGES_H

