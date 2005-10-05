#ifndef MOVEPAGES_H
#define MOVEPAGES_H

class QDialog;
class QCombobox;
class QLabel;
class QPushbutton;
class QSpinBox;
class QLayout;

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
	virtual void fromChanged(int);
	virtual void toChanged(int);
	void mvWherePageDataDisable(int index);
};

#endif // MOVEPAGES_H

