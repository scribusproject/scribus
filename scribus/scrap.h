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

#include <qwidget.h>
#include <qiconview.h>
#include <qframe.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qpixmap.h>
#include <qdragobject.h>
#include "scribusstructs.h"

class BibView : public QIconView
{
	Q_OBJECT

public:
	BibView( QWidget* parent, ApplicationPrefs *prefs);
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
	QMap<QString,Elem> Objekte;
	ApplicationPrefs *Prefs;

signals:
	void ToggleAllPalettes();
	void CloseMpal();
	void CloseTpal();

protected:
	virtual QDragObject *dragObject();
};

class Biblio : public QWidget
{
	Q_OBJECT

public:
	Biblio( QWidget* parent, ApplicationPrefs *prefs);
	~Biblio() {};
	void AdjustMenu();
	void ObjFromMenu(QString text);

	QPopupMenu* pmenu;
	QPopupMenu* fmenu;
	QPopupMenu* vmenu;
	QMenuBar* menuBar;
	QFrame* Frame3;
	BibView* BibWin;
	ApplicationPrefs *Prefs;
	QString ScFilename;
	QString OldName;
	int fSave;
	int vS;
	int vM;
	int vB;
	bool Changed;

public slots:
	void Save();

private slots:
	void HandleMouse(QIconViewItem *ite);
	void DeleteObj(QString name, QIconViewItem *ite);
	void ItemRenamed(QIconViewItem *ite);
	void DropOn(QDropEvent *e);
	void SaveAs();
	void Load();
	void NewLib();
	void SetPreview(int id);

protected:
	QVBoxLayout* BiblioLayout;
	QVBoxLayout* Frame3Layout;
};

#endif // BIBLIO_H
