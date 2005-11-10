#ifndef MUSTERPAGES_H
#define MUSTERPAGES_H

#include <qdialog.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCloseEvent>
class Q3ListBox;
class Q3ListBoxItem;
class QToolButton;
class QHBoxLayout;
class QVBoxLayout;

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
	Q3ListBox* masterPageData;
	QToolButton* importButton;
	QToolButton* newButton;
	QToolButton* duplicateButton;
	QToolButton* deleteButton;
	QToolButton* closeButton;
	ScribusDoc *currentDoc;
	ScribusView *currentView;
	QString sMuster;

protected:
	QVBoxLayout* masterPagesLayout;
	QHBoxLayout* buttonLayout;

private slots:
	void duplicateMasterPage();
	void deleteMasterPage();
	void newMasterPage();
	void appendPage();
	void selectMasterPage(Q3ListBoxItem *item);

signals:
	void createNew(int);
	void removePage(int);
	void loadPage(QString, int, bool);
	void finished();
	void docAltered();

};

#endif // MUSTERPAGES_H
