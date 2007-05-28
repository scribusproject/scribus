/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef BIBLIO_H
#define BIBLIO_H

#include <q3iconview.h>
#include <q3frame.h>
#include <qtabwidget.h>
// #include <qpopupmenu.h>
#include <qmenubar.h>

#include <qtooltip.h>
#include <qpixmap.h>
#include <q3dragobject.h>
//Added by qt3to4:
#include <QDropEvent>
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include <QKeyEvent>

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "scribusstructs.h"

class Q3HBoxLayout;
class QToolButton;
class Q3VBoxLayout;
class QToolBox;

class SCRIBUS_API BibView : public Q3IconView
{
	Q_OBJECT

public:
	BibView( QWidget* parent);
	~BibView() {};
	void keyPressEvent(QKeyEvent *k);
	void AddObj(QString name, QString daten, QPixmap Bild);
	void checkAndChange(QString &text, QString nam, QString dir);
	void SaveContents(QString name, QString oldName);
	void ReadOldContents(QString, QString newName);
	void ReadContents(QString name);
	struct Elem
	{
		QString Data;
		QPixmap Preview;
	};
	QMap<QString,Elem> objectMap;
	QString ScFilename;
	bool canWrite;

signals:
	void ToggleAllPalettes();
	void Schliessen();
	void CloseMpal();
	void CloseTpal();

protected:
	virtual Q3DragObject *dragObject();
};

class SCRIBUS_API Biblio : public ScrPaletteBase
{
	Q_OBJECT

public:
	Biblio( QWidget* parent);
	~Biblio() {};
	void ObjFromMenu(QString text);
	void ObjFromCopyAction(QString text);
	void adjustReferences(QString nam);
	void CleanUpTemp();
	void setScrapbookFileName(QString);
	const QString getScrapbookFileName();
	const int objectCount();
	void readContents(QString);
	void readTempContents(QString);
	void readOldContents(QString, QString);
	void installEventFilter(QObject *);
	void setOpenScrapbooks(QStringList &fileNames);
	QStringList getOpenScrapbooks();
	BibView* tempBView;
	
public slots:
	void languageChange();

private slots:
	void HandleMouse(int button, Q3IconViewItem *ite);
	bool copyObj(int id);
	void moveObj(int id);
	void DeleteObj(QString name, Q3IconViewItem *ite);
	void ItemRenamed(Q3IconViewItem *ite);
	void DropOn(QDropEvent *e);
	void NewLib();
	void Load();
	void SaveAs();
	void closeLib();
//	void libChanged(QWidget *lib);
	void libChanged(int index);
	void Import();

signals:
	void updateRecentMenue();

protected:
//	QMap<QString, QGuardedPtr<ScrAction> > scrapbookActions;
// 	QPopupMenu* pmenu;
// 	QPopupMenu* fmenu;
// 	QMenuBar* menuBar;
//	QTabWidget* Frame3;
	QToolBox* Frame3;
	Q3VBoxLayout* BiblioLayout;
	BibView* activeBView;
	int tempCount;
	QString OldName;
// 	int mFile;
// 	int mView;
// 	int fNew;
// 	int fLoad;
// 	int fSave;
// 	int fSaveAs;
// 	int fClose;
// 	int fImport;
	Q3HBoxLayout* buttonLayout;
	QToolButton* newButton;
	QToolButton* loadButton;
	QToolButton* saveAsButton;
	QToolButton* importButton;
	QToolButton* closeButton;
};

#endif // BIBLIO_H
