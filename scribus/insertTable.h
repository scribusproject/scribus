#ifndef INSERTTABLE_H
#define INSERTTABLE_H

#include <qvariant.h>
#include <qdialog.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

class InsertTable : public QDialog
{
	Q_OBJECT

public:
	InsertTable( QWidget* parent, int maxRow, int maxCol );
	~InsertTable() {};

	QSpinBox* Cols;
	QLabel* Text1;
	QLabel* Text2;
	QSpinBox* Rows;
	QPushButton* OKButton;
	QPushButton* CancelB;

protected:
	QVBoxLayout* InsertTableLayout;
	QGridLayout* layout2;
	QHBoxLayout* layout1;

};

#endif // INSERTTABLE_H
