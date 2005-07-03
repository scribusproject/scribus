/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
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
#include "prefscontext.h"

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
	void OpenScript();
	void SaveAs();
	void GetFieldN();

protected:
    QVBoxLayout* EditorLayout;
	PrefsContext* dirs;
};

#endif // EDITOR_H
