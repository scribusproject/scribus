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
	void AddObj(QString name, QString daten, QPixmap Bild, bool isDir = false, bool isRaster = false, bool isVector = false);
	void checkAndChange(QString &text, QString nam, QString dir);
	void ReadOldContents(QString, QString newName);
	void ReadContents(QString name);
	struct Elem
	{
		bool isDir;
		bool isRaster;
		bool isVector;
		QString Data;
		QPixmap Preview;
		QListWidgetItem* widgetItem;
	};
	QMap<QString,Elem> objectMap;
	QString ScFilename;
	QString visibleName;
	bool canWrite;

signals:
	void ToggleAllPalettes();
	void Schliessen();
	void CloseMpal();
	void CloseTpal();
	void objDropped(QString text);
	void fileDropped(QString path, int testResult);

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
	QStringList getOpenScrapbooksNames();
	QString getObjectName(QString &text);
	BibView* tempBView;
	BibView* activeBView;
	QListWidgetItem *actItem;
	
public slots:
	void languageChange();
	void ObjFromFile(QString path, int testResult);
	void ObjFromMenu(QString text);
	void ObjFromMainMenu(QString text, int scrapID);
	void closeOnDel(QString libName);
	void reloadLib(QString fileName);

private slots:
	void handleDoubleClick(QListWidgetItem *ite);
	void handlePasteToPage();
	void HandleMouse(QPoint p);
	bool copyObj(int id);
	void moveObj(int id);
	void deleteObj();
	void deleteAllObj();
	void renameObj();
	void NewLib();
	void updateView();
	void goOneDirUp();
	void closeLib();
	void libChanged(int index);
	void Import();

signals:
	void updateRecentMenue();
	void pasteToActualPage(QString);

protected:
	QToolBox* Frame3;
	QVBoxLayout* BiblioLayout;
	int tempCount;
	QString OldName;
	QHBoxLayout* buttonLayout;
	QToolButton* newButton;
	QToolButton* upButton;
	QToolButton* importButton;
	QToolButton* closeButton;
	QToolButton* configButton;
	QMenu* configMenue;
	QAction* conf_HideDirs;
	QAction* conf_HideImages;
	QAction* conf_HideVectors;
	QAction* conf_OpenMode;
	PrefsContext* prefs;
};

#endif // BIBLIO_H
