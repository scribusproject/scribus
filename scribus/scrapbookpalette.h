/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef BIBLIO_H
#define BIBLIO_H

#include <QDropEvent>
#include <QDragMoveEvent>
#include <QDragEnterEvent>
#include <QListWidget>

class QEvent;

#include "scribusapi.h"
#include "scrpalettebase.h"
#include "scribusstructs.h"

class QHBoxLayout;
class QToolButton;
class QVBoxLayout;
class QToolBox;
class QPixmap;
class QListWidgetItem;

class SCRIBUS_API BibView : public QListWidget
{
	Q_OBJECT

public:
	BibView( QWidget* parent);
	~BibView() {};
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
	void objDropped(QString text);

protected:
	void dragEnterEvent(QDragEnterEvent *e);
	void dragMoveEvent(QDragMoveEvent *e);
	void dropEvent(QDropEvent *e);
	void startDrag(Qt::DropActions supportedActions);
};

class SCRIBUS_API Biblio : public ScrPaletteBase
{
	Q_OBJECT

public:
	Biblio( QWidget* parent);
	~Biblio() {};
	
	virtual void changeEvent(QEvent *e);
	
	void ObjFromCopyAction(QString text, QString name);
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
	QString getObjectName(QString &text);
	BibView* tempBView;
	QListWidgetItem *actItem;
	
public slots:
	void languageChange();
	void ObjFromMenu(QString text);

private slots:
	void HandleMouse(QPoint p);
	bool copyObj(int id);
	void moveObj(int id);
	void deleteObj();
	void deleteAllObj();
	void renameObj();
	void NewLib();
	void Load();
	void SaveAs();
	void closeLib();
	void libChanged(int index);
	void Import();

signals:
	void updateRecentMenue();

protected:
	QToolBox* Frame3;
	QVBoxLayout* BiblioLayout;
	BibView* activeBView;
	int tempCount;
	QString OldName;
	QHBoxLayout* buttonLayout;
	QToolButton* newButton;
	QToolButton* loadButton;
	QToolButton* saveAsButton;
	QToolButton* importButton;
	QToolButton* closeButton;
};

#endif // BIBLIO_H
