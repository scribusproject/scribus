#ifndef SELECTFIELDS_H
#define SELECTFIELDS_H

#include <qdialog.h>
#include <qlabel.h>
#include <q3listbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "scribusapi.h"
#include "pageitem.h"
#include "scribusview.h"

class SCRIBUS_API SelectFields : public QDialog
{
	Q_OBJECT

public:
	SelectFields( QWidget* parent, QString Felder, QString Own, ScribusDoc *Doc, int Art);
	~SelectFields() {};

	QLabel* Text1;
	Q3ListBox* AvailFields;
	QPushButton* ToSel;
	QPushButton* FromSel;
	QLabel* Text2;
	Q3ListBox* SelFields;
	QPushButton* OK;
	QPushButton* Cancel;
	QString S_Fields;
	int FTyp;

public slots:
	void SetRetVal();
	void RemoveSel();
	void PutToSel();
	void SelAField(Q3ListBoxItem *c);
	void SelEField(Q3ListBoxItem *c);

protected:
	QVBoxLayout* SelectFieldsLayout;
	QHBoxLayout* Layout5;
	QVBoxLayout* Layout1;
	QVBoxLayout* Layout2;
	QVBoxLayout* Layout3;
	QHBoxLayout* Layout4;
};

#endif // SELECTFIELDS_H
