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
#include <QKeyEvent>
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

class SCRIBUS_API SymbolView : public QListWidget
{
	Q_OBJECT

public:
	SymbolView( QWidget* parent);
	~SymbolView() {};

public slots:
	void handleContextMenu(QPoint p);
	void changeDisplay();

protected:
	bool viewportEvent(QEvent *event) override;
	void dragEnterEvent(QDragEnterEvent *e) override;
	void dragMoveEvent(QDragMoveEvent *e) override;
	void dropEvent(QDropEvent *e) override;
	void startDrag(Qt::DropActions supportedActions) override;

signals:
	void objectDropped();

private:
	ScListWidgetDelegate* m_delegate { nullptr };
};

class SCRIBUS_API SymbolPalette : public ScDockPalette
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

public slots:
	void handleContextMenu(QPoint p);
	void handleDoubleClick(QListWidgetItem *item);
	void handleEditItem();
	void handleDeleteItem();
	void handleUpdateRequest(int);
	void languageChange();
	void keyPressEvent(QKeyEvent *e) override;

signals:
	void startEdit(QString);
	void endEdit();
	void objectDropped();

protected:
	SymbolView* SymbolViewWidget { nullptr };
	QVBoxLayout* PaletteLayout { nullptr };
	ScribusDoc *m_doc { nullptr };
	ScribusMainWindow *m_scMW { nullptr };
	QStringList editItemNames;
	QListWidgetItem *m_item { nullptr };

	void changeEvent(QEvent *e) override;
};

#endif
