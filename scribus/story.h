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
	void keyPressEvent(QKeyEvent *k);
	int clines;

signals:
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
};

class StoryEditor : public QDialog
{
	Q_OBJECT

public:
	StoryEditor( QWidget* parent, ScribusDoc *docc, PageItem* ite );
	~StoryEditor() {};
	int getStyle(int where);
	void addPar(int where, QString text, int sty);

	STable* table1;
	QPtrList<QTextEdit> edList;
	QPtrList<QComboBox> stList;
	QStringList style;
	ScribusDoc* doc;
	PageItem* CurrItem;
	bool TextChanged;

public slots:
	void updateTextFrame();
	void styleChange(int st);
	void modifiedText();
	void WrapHandler();
	void clickAt( int row, int col);
	void KeyDel();
	void KeyBS();
	void KeyRet();

protected:
	QHBoxLayout* Form1Layout;

};

#endif

