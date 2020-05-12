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
#include "scdockpalette.h"
#include "scribusstructs.h"

class QHBoxLayout;
class QToolButton;
class QVBoxLayout;
class QToolBox;
class QPixmap;
class QListWidgetItem;
class QDomElement;

class SCRIBUS_API BibView : public QListWidget
{
	Q_OBJECT

public:
	BibView( QWidget* parent);
	~BibView() {};

	void addObject(const QString& name, const QString& daten, const QPixmap& Bild, bool isDir = false, bool isRaster = false, bool isVector = false);
	void checkForImg(const QDomElement& elem, bool &hasImage);
	void checkAndChange(const QString& text, const QString& nam, const QString& dir);
	void checkAndChangeGroups(const QDomElement& elem, const QString& dir, const QFileInfo& fid);
	void readOldContents(const QString&, const QString& newName);
	void readContents(const QString& name);

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
	void objDropped(QString text);
	void fileDropped(QString path, int testResult);
	void paletteClosed();

protected:
	void dragEnterEvent(QDragEnterEvent *e);
	void dragMoveEvent(QDragMoveEvent *e);
	void dropEvent(QDropEvent *e);
	void startDrag(Qt::DropActions supportedActions);
};

class SCRIBUS_API Biblio : public ScDockPalette
{
	Q_OBJECT

public:
	Biblio( QWidget* parent);
	~Biblio() {};
	
	void changeEvent(QEvent *e) override;
	
	void objFromCopyAction(const QString& text, const QString& name);
	void adjustReferences(const QString& nam);
	void adjustReferencesGroups(const QDomElement& elem, const QFileInfo& fid);
	void cleanUpTemp();
	void setScrapbookFileName(const QString&);
	const QString &getScrapbookFileName();
	int objectCount();
	bool tempHasContents();
	void readContents(const QString&);
	void readTempContents(const QString&);
	void readOldContents(const QString&, const QString&);
	void installEventFilter(QObject *);
	void setOpenScrapbooks(QStringList &fileNames);
	QStringList getOpenScrapbooks();
	QStringList getOpenScrapbooksNames();
	QString getObjectName(QString &text);

	BibView* tempBView;
	BibView* activeBView;
	QListWidgetItem *actItem;
	
public slots:
	void iconSetChange();
	void languageChange();
	void objFromFile(const QString& path, int testResult);
	void objFromMenu(QString text);
	void objFromMainMenu(QString text, int scrapID);
	void closeOnDel(const QString& libName);
	void reloadLib(const QString& fileName);

private slots:
	void handleDoubleClick(QListWidgetItem *ite);
	void handlePasteToPage();
	void handleMouse(QPoint p);
	bool copyObj(int id);
	void moveObj(int id);
	void deleteObj();
	void deleteAllObj();
	void renameObj();
	void newLib();
	void updateView();
	void goOneDirUp();
	void closeLib();
	void libChanged(int index);
	void Import();

signals:
	void updateRecentMenue();
	void pasteToActualPage(QString);
	void scrapbookListChanged();

protected:
	int m_tempCount;
	PrefsContext* m_prefs;

	QToolBox* Frame3;
	QVBoxLayout* BiblioLayout;
	QHBoxLayout* buttonLayout;
	QWidget* containerWidget;
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
};

#endif // BIBLIO_H
