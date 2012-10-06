/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          symbolpalette.h  -  description
                             -------------------
    begin                : Tue Mar 27 2012
    copyright            : (C) 2012 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
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

#ifndef INLINEPAL_H
#define INLINEPAL_H

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
#include "scdockpalette.h"
#include "sclistwidgetdelegate.h"

class SCRIBUS_API InlineView : public QListWidget
{
	Q_OBJECT

public:
	InlineView( QWidget* parent);
	~InlineView() {};

protected:
	void dragEnterEvent(QDragEnterEvent *e);
	void dragMoveEvent(QDragMoveEvent *e);
	void dropEvent(QDropEvent *e);
	void startDrag(Qt::DropActions supportedActions);

private:
	ScListWidgetDelegate* delegate;
};

class SCRIBUS_API InlinePalette : public ScDockPalette
{
	Q_OBJECT

public:
	InlinePalette(QWidget* parent);
	~InlinePalette() {};
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *);
	void unsetDoc();
	void editingStart();
	void editingFinished();
	void updateItemList();
	
	virtual void changeEvent(QEvent *e);

public slots:
	void handleContextMenue(QPoint p);
	void handlePasteToItem();
	void handleEditItem();
	void handleDoubleClick(QListWidgetItem *item);
	void handleDeleteItem();
	void handleUpdateRequest(int);
	void languageChange();

signals:
	void startEdit(int);
	void endEdit();

protected:
	InlineView *InlineViewWidget;
	QVBoxLayout* PaletteLayout;
	ScribusDoc *currDoc;
	ScribusMainWindow *m_scMW;
	int actItem;
};

#endif
