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

#include "scribusapi.h"

class SCRIBUS_API InsertTable : public QDialog
{
	Q_OBJECT

public:
	InsertTable( QWidget* parent, int maxRow, int maxCol );
	~InsertTable() {};

	QSpinBox* Cols;
	QSpinBox* Rows;

protected:
	QVBoxLayout* InsertTableLayout;
	QGridLayout* layout2;
	QHBoxLayout* layout1;
	QLabel* Text1;
	QLabel* Text2;
	QPushButton* okButton;
	QPushButton* cancelButton;
};

#endif // INSERTTABLE_H
