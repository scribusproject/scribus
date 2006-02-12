/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef BIBLIO_H
#define BIBLIO_H

#include <qiconview.h>
#include <qframe.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <qdragobject.h>

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "scribusstructs.h"

class SCRIBUS_API BibView : public QIconView
{
	Q_OBJECT

public:
	BibView( QWidget* parent);
	~BibView() {};
	void keyPressEvent(QKeyEvent *k);
	void AddObj(QString name, QString daten, QPixmap Bild);
	void SaveContents(QString name, QString oldName);
	void ReadOldContents(QString, QString newName);
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
	virtual QDragObject *dragObject();
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
	void readOldContents(QString, QString);
	void installEventFilter(const QObject *);
	
public slots:
	void languageChange();

private slots:
	void HandleMouse(QIconViewItem *ite);
	void DeleteObj(QString name, QIconViewItem *ite);
	void ItemRenamed(QIconViewItem *ite);
	void DropOn(QDropEvent *e);
	void SaveAs();
	void Load();
	//void CloseWin();
	void NewLib();
	void SetPreview(int id);

protected:
//	QMap<QString, QGuardedPtr<ScrAction> > scrapbookActions;
	QPopupMenu* pmenu;
	QPopupMenu* fmenu;
	QPopupMenu* vmenu;
	QMenuBar* menuBar;
	QFrame* Frame3;
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
