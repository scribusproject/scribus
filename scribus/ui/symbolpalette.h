/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          symbolpalette.h  -  description
                             -------------------
    copyright            : Scribus Team
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SYMBOLPAL_H
#define SYMBOLPAL_H

#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QImage>
#include <QListWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QToolTip>
#include <QVBoxLayout>


class QEvent;
class ScribusMainWindow;
class ScribusDoc;


#include "scribusapi.h"
#include "scrpalettebase.h"

class SCRIBUS_API SymbolView : public QListWidget
{
	Q_OBJECT

public:
	SymbolView( QWidget* parent);
	~SymbolView() {};

protected:
	void dragEnterEvent(QDragEnterEvent *e);
	void dragMoveEvent(QDragMoveEvent *e);
	void dropEvent(QDropEvent *e);
	void startDrag(Qt::DropActions supportedActions);
};

class SCRIBUS_API SymbolPalette : public ScrPaletteBase
{
	Q_OBJECT

public:
	SymbolPalette(QWidget* parent);
	~SymbolPalette() {};
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *);
	void unsetDoc();
	void editingStart(QStringList names);
	void editingFinished();
	void updateSymbolList();
	
	virtual void changeEvent(QEvent *e);

public slots:
	void handleDoubleClick(QListWidgetItem *item);
	void handleUpdateRequest(int);
	void languageChange();

signals:
	void startEdit(QString);
	void endEdit();

protected:
	SymbolView *SymbolViewWidget;
	QVBoxLayout* PaletteLayout;
	ScribusDoc *currDoc;
	ScribusMainWindow *m_scMW;
	QStringList editItemNames;
};

#endif