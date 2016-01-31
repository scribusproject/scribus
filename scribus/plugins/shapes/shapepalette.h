/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          shapepalette.h  -  description
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

#ifndef SHAPEPAL_H
#define SHAPEPAL_H

#include <QKeyEvent>
#include <QImage>
#include <QListWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QToolBox>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>
#include <QHash>


class QEvent;
class ScribusMainWindow;
class ScribusDoc;


#include "pluginapi.h"
#include "ui/scdockpalette.h"
#include "ui/sclistwidgetdelegate.h"
#include "fpointarray.h"

struct shapeData
{
	int width;
	int height;
	QString name;
	FPointArray path;
};

class PLUGIN_API ShapeView : public QListWidget
{
	Q_OBJECT

public:
	ShapeView( QWidget* parent);
	~ShapeView() {};

	void updateShapeList();

	QHash<QString, shapeData> shapes;
	ScribusMainWindow *scMW;

public slots:
	void HandleContextMenu(QPoint);
	void changeDisplay();
	void delOne();
	void deleteAll();
signals:
	void objectDropped();

protected:
	bool viewportEvent(QEvent *event);
	void keyPressEvent(QKeyEvent *e);
	void dragEnterEvent(QDragEnterEvent *e);
	void dragMoveEvent(QDragMoveEvent *e);
	void dropEvent(QDropEvent *e);
	void startDrag(Qt::DropActions supportedActions);


private:
	ScListWidgetDelegate* delegate;
};

class PLUGIN_API ShapePalette : public ScDockPalette
{
	Q_OBJECT

public:
	ShapePalette(QWidget* parent);
	~ShapePalette() {};

	void writeToPrefs();
	void readFromPrefs();
	double decodePSDfloat(uint data);
	void setMainWindow(ScribusMainWindow *mw);
	void setDoc(ScribusDoc *);
	void unsetDoc();
	
	virtual void changeEvent(QEvent *e);

public slots:
	void closeTab();
	void Import();
	void languageChange();

protected:
	ShapeView *ShapeViewWidget;
	QToolBox* Frame3;
	QWidget* containerWidget;
	QVBoxLayout* vLayout;
	QHBoxLayout* buttonLayout;
	QToolButton* importButton;
	QToolButton* closeButton;

	ScribusDoc *m_doc;
	ScribusMainWindow *m_scMW;
};

#endif
