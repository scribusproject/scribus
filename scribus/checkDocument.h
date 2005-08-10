#ifndef CHECKDOCUMENT_H
#define CHECKDOCUMENT_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

#include "scribusapi.h"
#include "scrpalettebase.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QListView;
class QListViewItem;
class QComboBox;
class QLabel;
class QPushButton;
class ScribusDoc;

class SCRIBUS_API CheckDocument : public ScrPaletteBase
{
	Q_OBJECT

public:
	CheckDocument( QWidget* parent, bool modal );
	~CheckDocument() {};
	void clearErrorList();
	void buildErrorList(ScribusDoc *doc);
	/*
	void closeEvent(QCloseEvent *ce);
	void keyPressEvent(QKeyEvent *ke);
	*/
	QComboBox* curCheckProfile;
	QLabel* textLabel1;
	QListView* reportDisplay;
	QPushButton* ignoreErrors;
	QMap<QListViewItem*, int> itemMap;
	QMap<QListViewItem*, int> pageMap;
	QMap<QListViewItem*, QString> masterPageMap;
	QMap<QListViewItem*, int> masterPageItemMap;
	bool noButton;
	int checkMode;

public slots:
	void slotSelect(QListViewItem* ite);
	void newScan(const QString&);

signals:
	void rescan();
	//void closePal(bool);
	void selectElement(int, int);
	void selectPage(int);
	void selectMasterPage(QString);
	void selectMasterPageElement(QString, int);
	void selectNormal();
	void ignoreAllErrors();

protected:
	QVBoxLayout* checkDocumentLayout;
	QHBoxLayout* layout1;
	QHBoxLayout* layout2;

protected slots:
	virtual void languageChange();

private:
	ScribusDoc* document;
	QPixmap graveError;
	QPixmap onlyWarning;
	QPixmap noErrors;

};

#endif // CHECKDOCUMENT_H
