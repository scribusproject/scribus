#ifndef FONTPREFS_H
#define FONTPREFS_H

#include <qlayout.h>
#include <qpushbutton.h>
#include <q3table.h>
#include <qcheckbox.h>
#include <q3header.h>
#include <q3ptrlist.h>
#include <qtabwidget.h>
#include <qwidget.h>
#include <qcombobox.h>
#include <q3listbox.h>
#include <q3listview.h>
//Added by qt3to4:
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
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
	Q3ListBox* PathList;
	Q3ListView* fontList;
	Q3Table* Table3;
	Q3Header *Header;
	Q3Header *Header2;
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
	Q3PtrList<QComboBox> FlagsRepl;
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
	void slotClick(Q3ListViewItem* ite, const QPoint &, int col);
	void ReplaceSel(int r, int c);
	void UpdateFliste();
	void DelEntry();
	void SelectPath(Q3ListBoxItem *c);
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
