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
#include <QVBoxLayout>

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
    QVBoxLayout* EditorLayout;
	PrefsContext* dirs;
};

#endif // EDITOR_H
