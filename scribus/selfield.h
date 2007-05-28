/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SELECTFIELDS_H
#define SELECTFIELDS_H

#include <qdialog.h>
#include <qlabel.h>
#include <q3listbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>

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
	Q3VBoxLayout* SelectFieldsLayout;
	Q3HBoxLayout* Layout5;
	Q3VBoxLayout* Layout1;
	Q3VBoxLayout* Layout2;
	Q3VBoxLayout* Layout3;
	Q3HBoxLayout* Layout4;
};

#endif // SELECTFIELDS_H
