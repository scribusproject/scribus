/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef LINEFORMATE_H
#define LINEFORMATE_H

#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <Q3HBoxLayout>

#include "scribusapi.h"
#include "multiline.h"
#include "scribusstructs.h"
#include "sclistboxpixmap.h"

class Q3ListBox;
class QPushButton;
class Q3VBoxLayout;
class Q3HBoxLayout;
class ScribusDoc;

class SCRIBUS_API LineFormate : public QDialog
{
	Q_OBJECT

public:
	LineFormate( QWidget* parent, ScribusDoc *doc);
	~LineFormate() {};

	Q3ListBox* ListBox1;
	QPushButton* LoadLS;
	QPushButton* NewB;
	QPushButton* EditB;
	QPushButton* DublicateB;
	QPushButton* DeleteB;
	QPushButton* SaveB;
	QPushButton* ExitB;
	QPushButton* CancelB;
	QString sFnumber;
	QMap<QString,multiLine> TempStyles;
	QMap<QString,QString> Replacement;
	ScribusDoc *Docu;
	void UpdateFList();

public slots:
	void saveIt();
	void selFormat(Q3ListBoxItem *c);
	void selEditFormat(Q3ListBoxItem *c);
	void editFormat();
	void neuesFormat();
	void dupFormat();
	void deleteFormat();
	void loadLStyles();

signals:
	void saveStyle(LineFormate *);
	
protected:
	Q3HBoxLayout* StilFormateLayout;
	Q3VBoxLayout* Layout15;
};

class SCRIBUS_API LineFormateItem : public ScListBoxPixmap<37, 37>
{
public:
	LineFormateItem(ScribusDoc* Doc, const multiLine& MultiLine, const QString& Text);
	virtual int rtti() const { return 148523874; }
protected:
	multiLine mLine;
	ScribusDoc* doc;
	virtual void redraw(void);
};

#endif
