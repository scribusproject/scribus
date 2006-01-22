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
#include <qlistbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>

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
	QListBox* AvailFields;
	QPushButton* ToSel;
	QPushButton* FromSel;
	QLabel* Text2;
	QListBox* SelFields;
	QPushButton* OK;
	QPushButton* Cancel;
	QString S_Fields;
	int FTyp;

public slots:
	void SetRetVal();
	void RemoveSel();
	void PutToSel();
	void SelAField(QListBoxItem *c);
	void SelEField(QListBoxItem *c);

protected:
	QVBoxLayout* SelectFieldsLayout;
	QHBoxLayout* Layout5;
	QVBoxLayout* Layout1;
	QVBoxLayout* Layout2;
	QVBoxLayout* Layout3;
	QHBoxLayout* Layout4;
};

#endif // SELECTFIELDS_H
