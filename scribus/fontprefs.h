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
#include "scribusstructs.h"
class ScribusDoc;

class FontPrefs : public QTabWidget
{
    Q_OBJECT

public:
    FontPrefs( QWidget* parent, SCFonts &flist, bool Hdoc, ApplicationPrefs *prefs, QString PPath, ScribusDoc* doc );
    ~FontPrefs() {};
	void ReadPath();
	void RebuildDialog();
    QWidget* tab1;
    QWidget* tab;
    QWidget* tab3;
    QListBox* PathList;
    QTable* Table1;
    QTable* Table3;
    QHeader *Header;
    QHeader *Header2;
    QPushButton* DelB;
    QPushButton* ChangeB;
    QPushButton* AddB;
    QPushButton* RemoveB;
    QPtrList<QCheckBox> FlagsPS;
    QPtrList<QCheckBox> FlagsUse;
    QPtrList<QCheckBox> FlagsSub;
    QPtrList<QComboBox> FlagsRepl;
  	ApplicationPrefs *Prefs;
	QMap<QString,QString> RList;
	QStringList UsedFonts;
	QStringList ExtraFonts;
	QString HomeP;
	QString CurrentPath;
	bool DocAvail;
	ScribusDoc* docc;

public slots:
	void ReplaceSel(int r, int c);
	void UpdateFliste();
	void DelEntry();
	void SelectPath(QListBoxItem *c);
	void AddPath();
	void ChangePath();
	void DelPath();

signals:
	void ReReadPrefs();

protected:
    QVBoxLayout* tab1Layout;
    QVBoxLayout* tabLayout;
    QHBoxLayout* Layout2a;
    QHBoxLayout* tab3Layout;
    QVBoxLayout* LayoutR;
};

#endif // FONTPREFS_H
