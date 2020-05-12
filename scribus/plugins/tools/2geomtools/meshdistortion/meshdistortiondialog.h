/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
*   Copyright (C) 2008 by Franz Schmid                                     *
*   franz.schmid@altmuehlnet.de                                            *
*                                                                          *
*   This program is free software; you can redistribute it and/or modify   *
*   it under the terms of the GNU General Public License as published by   *
*   the Free Software Foundation; either version 2 of the License, or      *
*   (at your option) any later version.                                    *
*                                                                          *
*   This program is distributed in the hope that it will be useful,        *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*   GNU General Public License for more details.                           *
*                                                                          *
*   You should have received a copy of the GNU General Public License      *
*   along with this program; if not, write to the                          *
*   Free Software Foundation, Inc.,                                        *
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.              *
****************************************************************************/

#ifndef MESHDISTORTIONDIALOG_H
#define MESHDISTORTIONDIALOG_H

#include <QDialog>
#include <QList>
#include <QGraphicsEllipseItem>
#include <QGraphicsPathItem>

#include "ui_meshdistortiondialog.h"
#include "pluginapi.h"
#include "scribusdoc.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif

#include "third_party/lib2geom/sbasis.h"
#include "third_party/lib2geom/sbasis-geometric.h"
#include "third_party/lib2geom/bezier-to-sbasis.h"
#include "third_party/lib2geom/sbasis-to-bezier.h"
#include "third_party/lib2geom/d2.h"
#include "third_party/lib2geom/piecewise.h"
#include "third_party/lib2geom/utils.h"
#include "third_party/lib2geom/path.h"
#include "third_party/lib2geom/sbasis-2d.h"
#include "third_party/lib2geom/transforms.h"
#include "third_party/lib2geom/scribushelper.h"
#include <vector>

//using namespace Geom;
class MeshDistortionDialog;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;
class QStyleOptionGraphicsItem;

class PLUGIN_API NodeItem : public QGraphicsEllipseItem
{
public:
	NodeItem(QRectF geom, uint num, MeshDistortionDialog* parent);
	~NodeItem() {};
	
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget* widget);
	uint handle;
	bool mouseMoving;
	bool mousePressed;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
	MeshDistortionDialog *dialog;
};

class PLUGIN_API MeshDistortionDialog : public QDialog, Ui::MeshDistortionDialog
{
	Q_OBJECT

public:
	MeshDistortionDialog(QWidget* parent, ScribusDoc *doc);
	~MeshDistortionDialog() {};
	
	void addItemsToScene(Selection* itemSelection, ScribusDoc *doc, QGraphicsPathItem* parentItem, PageItem* parent);
	void adjustHandles();
	void updateMesh(bool gridOnly);
	void updateAndExit();

	bool isFirst;
	double w4;
	double w2;
	double ww;
	double deltaX;
	double deltaY;
	ScribusDoc*  m_doc;
	QGraphicsScene scene;
	QGraphicsPathItem* pItemG;
	QList<QGraphicsPathItem*> origPathItem;
	QList<PageItem*> origPageItem;
	QList<NodeItem*> nodeItems;
	QList< Geom::Piecewise<Geom::D2<Geom::SBasis> > > origPath;
	std::vector<Geom::Point> handles;
	std::vector<Geom::Point> origHandles;
	Geom::D2<Geom::SBasis2d> sb2;

private slots:
	void doZoomIn();
	void doZoomOut();
	void doReset();

protected:
	void showEvent(QShowEvent *e);
};

#endif
