/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef SELECTFIELDS_H
#define SELECTFIELDS_H

#include <QDialog>
class QHBoxLayout;
class QVBoxLayout;
class QListWidget;
class QListWidgetItem;
class QPushButton;
class QLabel;

#include "scribusapi.h"
#include "scribusview.h"

class SCRIBUS_API SelectFields : public QDialog
{
	Q_OBJECT

public:
	SelectFields( QWidget* parent, QString Felder, QString Own, ScribusDoc *Doc, int Art);
	~SelectFields() {};

	QLabel* Text1;
	QListWidget* AvailFields;
	QPushButton* ToSel;
	QPushButton* FromSel;
	QLabel* Text2;
	QListWidget* SelFields;
	QPushButton* OK;
	QPushButton* Cancel;
	QString S_Fields;
	int FTyp;

public slots:
	void SetRetVal();
	void RemoveSel();
	void PutToSel();
	void SelAField(QListWidgetItem *c);
	void SelEField(QListWidgetItem *c);

protected:
	QVBoxLayout* SelectFieldsLayout;
	QHBoxLayout* Layout5;
	QVBoxLayout* Layout1;
	QVBoxLayout* Layout2;
	QVBoxLayout* Layout3;
	QHBoxLayout* Layout4;
};

#endif // SELECTFIELDS_H
