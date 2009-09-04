/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef FONTPREFSTAB_H
#define FONTPREFSTAB_H

#include <QTabWidget>
#include <QList>
#include <QMap>
class QHBoxLayout;
class QVBoxLayout;
class QPushButton;
class QListWidget;
class QListWidgetItem;
class QTreeWidget;
class QTreeWidgetItem;
class QTableWidget;
class FontListView;

#include "scribusapi.h"
#include "prefsmanager.h"

class ScribusDoc;
class ScComboBox;

/*! \brief A configuration Widget for Font Preferences */
class SCRIBUS_API FontPrefsTab : public QTabWidget
{
	Q_OBJECT

public:
	FontPrefsTab( QWidget* parent, bool Hdoc, QString PPath, ScribusDoc* doc );
	~FontPrefsTab() {};
	void restoreDefaults();

	QMap<QString,QString> RList;
	QList<ScComboBox*> FlagsRepl;

private:
	QWidget* tab1;
	QWidget* tab;
	QWidget* tab3;
	QListWidget* PathList;

	//! All fonts view
	FontListView * fontList;
	QTableWidget* Table3;
	QPushButton* DelB;
	QPushButton* ChangeB;
	QPushButton* AddB;
	QPushButton* RemoveB;

	//! List of font names of allowed fonts for substitutions
	QStringList UsedFonts;
	QString HomeP;
	QString CurrentPath;
	bool DocAvail;
	ScribusDoc* docc;

public slots:
	void ReplaceSel(int r, int c);
	void UpdateFliste();
	void DelEntry();
	void SelectPath(QListWidgetItem *c);
	void AddPath();
	void ChangePath();
	void DelPath();

protected:
	void readPaths();
	void writePaths();
	void rebuildDialog();

	QVBoxLayout* tab1Layout;
	QVBoxLayout* tabLayout;
	QHBoxLayout* Layout2a;
	QHBoxLayout* tab3Layout;
	QVBoxLayout* LayoutR;
};

#endif // FONTPREFS_H
