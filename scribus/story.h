/***************************************************************************
                          story.h    -  description
                             -------------------
    begin                : Tue Nov 11 2003
    copyright            : (C) 2003 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STORY_H
#define STORY_H

#include <qvariant.h>
#include <qdialog.h>
#include <qptrlist.h>
#include <qtable.h>
#include <qtextedit.h>
#include <qpopupmenu.h>
#include <qmenubar.h>
#include <qstatusbar.h>
#include <qlabel.h>
#include <qbuttongroup.h>
#include "pageitem.h"
class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QComboBox;

class SEditor : public QTextEdit
{
	Q_OBJECT

public:
	SEditor (QWidget* parent);
	~SEditor() {};
	void focusInEvent(QFocusEvent *f);
	void keyPressEvent(QKeyEvent *k);
	int clines;

signals:
	void UnRe(bool, bool);
	void wrapped();
	void bsPressed();
	void delPressed();
};

class STable : public QTable
{
	Q_OBJECT

public:
	STable (QWidget* parent);
	~STable() {};
	void keyPressEvent(QKeyEvent *k);
	void adjHeight(int r);
	int HomeK;
	int EndK;

signals:
	void StatBarUpdt();
};

class StoryEditor : public QDialog
{
	Q_OBJECT

public:
	StoryEditor( QWidget* parent, ScribusDoc *docc, PageItem* ite );
	~StoryEditor() {};
	void closeEvent(QCloseEvent *);
	int getStyle(int where);
	void addPar(int where, QString text, int sty);

	QPopupMenu* fmenu;
	QPopupMenu* emenu;
	QMenuBar* menuBar;
	QStatusBar* StateBar;
	QButtonGroup* ButtonGroup1;
	QButtonGroup* ButtonGroup2;
	QLabel* WordCT1;
	QLabel* WordCT3;
	QLabel* ParCT;
	QLabel* ParC;
	QLabel* WordCT;
	QLabel* WordC;
	QLabel* CharCT;
	QLabel* CharC;
	QLabel* WordCT2;
	QLabel* WordC2;
	QLabel* CharCT2;
	QLabel* CharC2;
	STable* table1;
	QPtrList<SEditor> edList;
	QPtrList<QComboBox> stList;
	QStringList style;
	ScribusDoc* doc;
	PageItem* CurrItem;
	bool TextChanged;
	int Mundo;
	int Mredo;
	int Mcopy;
	int Mcut;
	int Mdel;
	int Mpaste;
	int Mupdt;

public slots:
	void updateStatus();
	void Do_leave();
	void Do_new();
	void Do_undo();
	void Do_redo();
	void Do_copy();
	void Do_paste();
	void Do_cut();
	void Do_del();
	void UnReMenu(bool u, bool r);
	void CopyAvail(bool u);
	void updateTextFrame();
	void slotEditStyles();
	void styleChange(int st);
	void modifiedText();
	void WrapHandler();
	void clickAt( int row, int col);
	void KeyDel();
	void KeyBS();
	void KeyRet();

signals:
	void DocChanged();
	void EditSt();

protected:
	QVBoxLayout* Form1Layout;
	QGridLayout* ButtonGroup1Layout;
	QGridLayout* ButtonGroup2Layout;

};

#endif

