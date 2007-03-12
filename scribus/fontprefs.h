/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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
#include <Q3HBoxLayout>
#include <Q3VBoxLayout>
#include "scribusapi.h"
#include "scribusstructs.h"
#include "prefsmanager.h"

class ScribusDoc;

/*! \brief A configuration Widget for Font Preferences */
class SCRIBUS_API FontPrefs : public QTabWidget
{
	Q_OBJECT

public:
	FontPrefs( QWidget* parent, bool Hdoc, QString PPath, ScribusDoc* doc );
	~FontPrefs() {};
	void restoreDefaults();

	struct fontSet
	{
		bool FlagPS;
		bool FlagUse;
		bool FlagSub;
		bool FlagOTF;
		bool FlagNames;
	};
	QMap<QString, fontSet> fontFlags;
	QMap<QString,QString> RList;
	Q3PtrList<QComboBox> FlagsRepl;

private:
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

	QStringList UsedFonts;
	QString HomeP;
	QString CurrentPath;
	bool DocAvail;
	ScribusDoc* docc;
	QPixmap ttfFont;
	QPixmap otfFont;
	QPixmap psFont;
	//QPixmap okIcon;
	//QPixmap empty;
	QPixmap checkOn;
	QPixmap checkOff;

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
	void rebuildDialog();

	Q3VBoxLayout* tab1Layout;
	Q3VBoxLayout* tabLayout;
	Q3HBoxLayout* Layout2a;
	Q3HBoxLayout* tab3Layout;
	Q3VBoxLayout* LayoutR;
};

#endif // FONTPREFS_H
