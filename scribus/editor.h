#ifndef EDITOR_H
#define EDITOR_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include "scribusview.h"

class Editor : public QDialog
{ 
    Q_OBJECT

public:
    Editor( QWidget* parent, QString daten, ScribusView* vie = 0);
    ~Editor() {};
    QTextEdit* EditTex;
    QPopupMenu* fmenu;
    QPopupMenu* emenu;
    QMenuBar* menuBar;
    ScribusView* view;

private slots:
	void LoadScript();
	void SaveAs();
	void GetFieldN();

protected:
    QVBoxLayout* EditorLayout;
};

#endif // EDITOR_H
