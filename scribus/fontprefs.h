#ifndef FONTPREFS_H
#define FONTPREFS_H

#include <qlayout.h>
#include <qpushbutton.h>
#include <qtable.h>
#include <qcheckbox.h>
#include <qheader.h>
#include <qptrlist.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qcombobox.h>
#include <qlistbox.h>
#include <qlistview.h>
#include "scribusapi.h"
#include "scribusstructs.h"
#include "prefsmanager.h"

class ScribusDoc;

class SCRIBUS_API FontPrefs : public QTabWidget
{
	Q_OBJECT

public:
	FontPrefs( QWidget* parent, SCFonts &flist, bool Hdoc, QString PPath, ScribusDoc* doc );
	~FontPrefs() {};
	void restoreDefaults();

	QWidget* tab1;
	QWidget* tab;
	QWidget* tab3;
	QListBox* PathList;
	QListView* fontList;
	QTable* Table3;
	QHeader *Header;
	QHeader *Header2;
	QPushButton* DelB;
	QPushButton* ChangeB;
	QPushButton* AddB;
	QPushButton* RemoveB;
	struct fontSet
	{
		bool FlagPS;
		bool FlagUse;
		bool FlagSub;
		bool FlagOTF;
		bool FlagNames;
	};
	QMap<QString, fontSet> fontFlags;
	QPtrList<QComboBox> FlagsRepl;
	QMap<QString,QString> RList;
	QStringList UsedFonts;
	QString HomeP;
	QString CurrentPath;
	bool DocAvail;
	ScribusDoc* docc;
	QPixmap ttfFont;
	QPixmap otfFont;
	QPixmap psFont;
	QPixmap okIcon;
	QPixmap empty;

public slots:
	void slotClick(QListViewItem* ite, const QPoint &, int col);
	void ReplaceSel(int r, int c);
	void UpdateFliste();
	void DelEntry();
	void SelectPath(QListBoxItem *c);
	void AddPath();
	void ChangePath();
	void DelPath();

protected:
	void readPaths();
	void writePaths();
	void RebuildDialog();

	QVBoxLayout* tab1Layout;
	QVBoxLayout* tabLayout;
	QHBoxLayout* Layout2a;
	QHBoxLayout* tab3Layout;
	QVBoxLayout* LayoutR;
};

#endif // FONTPREFS_H
