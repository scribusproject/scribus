/****************************************************************************
** Form interface generated from reading ui file 'Bib.ui'
**
** Created: Sun Oct 14 19:47:43 2001
**      by:  The User Interface Compiler (uic)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/
#ifndef BIBLIO_H
#define BIBLIO_H

#include <q3iconview.h>
#include <q3frame.h>
#include <q3popupmenu.h>
#include <qmenubar.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <q3dragobject.h>
//Added by qt3to4:
#include <QVBoxLayout>
#include <QKeyEvent>
#include <QDropEvent>

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "scribusstructs.h"

class SCRIBUS_API BibView : public Q3IconView
{
	Q_OBJECT

public:
	BibView( QWidget* parent);
	~BibView() {};
	void keyPressEvent(QKeyEvent *k);
	void AddObj(QString name, QString daten, QPixmap Bild);
	void SaveContents(QString name);
	void ReadContents(QString name);
	void RebuildView();
	struct Elem
	{
		QString Data;
		QPixmap Preview;
	};
	QMap<QString,Elem> objectMap;

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
	void AdjustMenu();
	void ObjFromMenu(QString text);
	void setScrapbookFileName(QString);
	const QString getScrapbookFileName();
	const bool changed();
	const int objectCount();
	void rebuildView();
	void readContents(QString);
	void installEventFilter(const QObject *);
	
public slots:
	void Save();
	void languageChange();

private slots:
	void HandleMouse(Q3IconViewItem *ite);
	void DeleteObj(QString name, Q3IconViewItem *ite);
	void ItemRenamed(Q3IconViewItem *ite);
	void DropOn(QDropEvent *e);
	void SaveAs();
	void Load();
	//void CloseWin();
	void NewLib();
	void SetPreview(int id);

protected:
//	QMap<QString, QGuardedPtr<ScrAction> > scrapbookActions;
	Q3PopupMenu* pmenu;
	Q3PopupMenu* fmenu;
	Q3PopupMenu* vmenu;
	QMenuBar* menuBar;
	Q3Frame* Frame3;
	QVBoxLayout* BiblioLayout;
	QVBoxLayout* Frame3Layout;
	QString ScFilename;
	bool Changed;
	BibView* BibWin;
	QString OldName;
	int mFile;
	int mView;
	int fNew;
	int fLoad;
	int fSave;
	int fSaveAs;
	int fClose;
	int vSmall;
	int vMedium;
	int vLarge;
};

#endif // BIBLIO_H
