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
	void dragEnterEvent(QDragEnterEvent *e) override;
	void dragMoveEvent(QDragMoveEvent *e) override;
	void dropEvent(QDropEvent *e) override;
	void startDrag(Qt::DropActions supportedActions) override;

signals:
	void objectDropped(QString);

private:
	ScListWidgetDelegate* delegate { nullptr };
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
	void editingStart(int itemID);
	void editingFinished();
	void updateItemList();

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
	void objectDropped(QString);

protected:
	InlineView *InlineViewWidget { nullptr };
	QVBoxLayout* PaletteLayout { nullptr };
	ScribusDoc *m_doc { nullptr };
	ScribusMainWindow *m_scMW { nullptr };
	int actItem { -1 };
	int currentEditedItem { -1 };

	void changeEvent(QEvent *e) override;
};

#endif
