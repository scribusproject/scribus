/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
#ifndef EDITOR_H
#define EDITOR_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <q3textedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <q3popupmenu.h>
#include <qmenubar.h>
//Added by qt3to4:
#include <Q3VBoxLayout>

#include "scribusapi.h"

class ScribusView;
class PrefsContext;

class SCRIBUS_API Editor : public QDialog
{ 
    Q_OBJECT

public:
    Editor( QWidget* parent, QString daten, ScribusView* vie = 0);
    ~Editor() {};
    Q3TextEdit* EditTex;
    Q3PopupMenu* fmenu;
    Q3PopupMenu* emenu;
    QMenuBar* menuBar;
    ScribusView* view;

private slots:
	void OpenScript();
	void SaveAs();
	void GetFieldN();

protected:
    Q3VBoxLayout* EditorLayout;
	PrefsContext* dirs;
};

#endif // EDITOR_H
