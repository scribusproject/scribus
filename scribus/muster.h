/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef MUSTERPAGES_H
#define MUSTERPAGES_H

#include <qdialog.h>
//Added by qt3to4:
#include <Q3VBoxLayout>
#include <QCloseEvent>
#include <Q3HBoxLayout>
class Q3ListBox;
class Q3ListBoxItem;
class QToolButton;
class Q3HBoxLayout;
class Q3VBoxLayout;

#include "scribusapi.h"
class ScribusDoc;
class ScribusView;

class SCRIBUS_API MasterPagesPalette : public QDialog
{
	Q_OBJECT

public:
	MasterPagesPalette( QWidget* parent, ScribusDoc *pCurrentDoc, ScribusView *pCurrentView, QString masterPageName);
	~MasterPagesPalette() {};
	void closeEvent(QCloseEvent *closeEvent);
	void updateMasterPageList(QString MasterPageName);

public slots:
	void selectMasterPage(QString name);

protected slots:
	void reject();

private:
	Q3ListBox* masterPageListBox;
	QToolButton* importButton;
	QToolButton* newButton;
	QToolButton* duplicateButton;
	QToolButton* deleteButton;
	QToolButton* closeButton;
	ScribusDoc *currentDoc;
	ScribusView *currentView;
	QString sMuster;

protected:
	Q3VBoxLayout* masterPagesLayout;
	Q3HBoxLayout* buttonLayout;

private slots:
	void duplicateMasterPage();
	void deleteMasterPage();
	void newMasterPage();
	void renameMasterPage(Q3ListBoxItem * item);
	void appendPage();
	void selectMasterPage(Q3ListBoxItem *item);

signals:
	void removePage(int);
	void finished();
};

#endif // MUSTERPAGES_H
