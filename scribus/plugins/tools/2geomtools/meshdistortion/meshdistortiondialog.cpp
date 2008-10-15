/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2008 by Franz Schmid                                    *
*   franz.schmid@altmuehlnet.de                                           *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include "meshdistortiondialog.h"

#include <QPainterPath>
#include <QGraphicsItem>

#include "commonstrings.h"
#include "fpointarray.h"
#include "pageitem.h"
#include "sccolorengine.h"
#include "scpattern.h"
#include "selection.h"
#include "util_icon.h"
#include "util_math.h"

NodeItem::NodeItem(QRectF geom, uint num, MeshDistortionDialog *parent) : QGraphicsEllipseItem(geom)
{
	dialog = parent;
	handle = num;
	setBrush(Qt::NoBrush);
	setPen(QPen(Qt::red, 2.0));
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	setZValue(9999999);
	setAcceptsHoverEvents(true);
	mouseMoving = false;
	mousePressed = false;
}

void NodeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(widget);
	if (option->state & QStyle::State_Selected)
	{
		painter->setBrush(Qt::red);
		painter->setPen(QPen(Qt::red, qMax(0.1, 1.0 / option->levelOfDetail)));
	}
	else
	{
		painter->setBrush(Qt::NoBrush);
		painter->setPen(QPen(Qt::red, qMax(0.2, 2.0 / option->levelOfDetail)));
	}
	painter->drawEllipse(rect());
}

void NodeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	mousePressed = true;
	mouseMoving = false;
	QGraphicsItem::mousePressEvent(event);
}

void NodeItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	mouseMoving = true;
	if ((mouseMoving) && (mousePressed))
		dialog->updateMesh(true);
	QGraphicsItem::mouseMoveEvent(event);
}

void NodeItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	if ((mouseMoving) && (mousePressed))
		dialog->updateMesh(false);
	mouseMoving = false;
	mousePressed = false;
	QGraphicsItem::mouseReleaseEvent(event);
}

void NodeItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	QPainterPath p;
	p.addEllipse(rect());
	if (isSelected())
		qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
	else
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void NodeItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QPainterPath p;
	p.addEllipse(rect());
	if (isSelected())
		qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
	else
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void NodeItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

MeshDistortionDialog::MeshDistortionDialog(QWidget* parent, ScribusDoc *doc) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(QIcon(loadIcon("AppIcon.png")));
	buttonZoomOut->setIcon(QIcon(loadIcon("16/zoom-out.png")));
	buttonZoomIn->setIcon(QIcon(loadIcon("16/zoom-in.png")));
	m_doc = doc;
	PageItem *currItem;
	double gx, gy, gh, gw;
	doc->m_Selection->setGroupRect();
	doc->m_Selection->getGroupRect(&gx, &gy, &gw, &gh);
	uint selectedItemCount = doc->m_Selection->count();
	w4 = qMax(gw, gh) / 2.0;
	w2 = qMax(gw, gh);
	ww = qMax(gw, gh) * 2.0;
	QStack<PageItem*> groupStack;
	QStack<QGraphicsPathItem*> groupStack2;
	QStack<PageItem*> groupStack3;
	groupStack2.push(0);
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		currItem = doc->m_Selection->itemAt(i);
		FPointArray path = currItem->PoLine;
		deltaX = ((w2 - gw) / 2.0) + w4;
		deltaY = ((w2 - gh) / 2.0) + w4;
		QPainterPath pp;
		if (currItem->itemType() == PageItem::PolyLine)
			pp = path.toQPainterPath(false);
		else
			pp = path.toQPainterPath(true);
		QGraphicsPathItem* pItem = new QGraphicsPathItem(pp, groupStack2.top());
		if (groupStack2.top() == 0)
		{
			scene.addItem(pItem);
			pItem->setPos(currItem->xPos() - gx + deltaX, currItem->yPos() - gy + deltaY);
			pItem->rotate(currItem->rotation());
		}
		else
		{
			PageItem* parent = groupStack3.top();
			QMatrix mm;
			mm.rotate(-parent->rotation());
			mm.translate(-parent->xPos(), -parent->yPos());
			pItem->setPos(mm.map(QPointF(currItem->xPos(), currItem->yPos())));
		}
		QPainterPath pathO = pp;
		QMatrix mmO;
		mmO.translate(-w4, -w4);
		pathO = pItem->mapToScene(pathO);
		pathO = mmO.map(pathO);
		Geom::Piecewise<D2<Geom::SBasis> >  path_a_pw;
		if (currItem->itemType() == PageItem::PolyLine)
			path_a_pw = QPainterPath2Piecewise(pathO, false);
		else
			path_a_pw = QPainterPath2Piecewise(pathO, true);
		origPath.append(path_a_pw);
		pItem->setZValue(i);
		origPathItem.append(pItem);
		if (((currItem->fillColor() == CommonStrings::None) && (currItem->GrType == 0)) || (currItem->controlsGroup()))
			pItem->setBrush(Qt::NoBrush);
		else
		{
			if (currItem->GrType != 0)
			{
				if (currItem->GrType != 8)
				{
					QGradient pat;
					double x1 = currItem->GrStartX;
					double y1 = currItem->GrStartY;
					double x2 = currItem->GrEndX;
					double y2 = currItem->GrEndY;
					switch (currItem->GrType)
					{
						case 1:
						case 2:
						case 3:
						case 4:
						case 6:
							pat = QLinearGradient(x1, y1,  x2, y2);
							break;
						case 5:
						case 7:
							pat = QRadialGradient(x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)), x1, y1);
							break;
					}
					QList<VColorStop*> colorStops = currItem->fill_gradient.colorStops();
					QColor qStopColor;
					for( int offset = 0 ; offset < colorStops.count() ; offset++ )
					{
						qStopColor = colorStops[ offset ]->color;
						int h, s, v, sneu, vneu;
						int shad = colorStops[offset]->shade;
						qStopColor.getHsv(&h, &s, &v);
						sneu = s * shad / 100;
						vneu = 255 - ((255 - v) * shad / 100);
						qStopColor.setHsv(h, sneu, vneu);
						qStopColor.setAlphaF(colorStops[offset]->opacity);
						pat.setColorAt(colorStops[ offset ]->rampPoint, qStopColor);
					}
					pItem->setBrush(pat);
				}
				else if ((currItem->GrType == 8) && (!currItem->pattern().isEmpty()) && (doc->docPatterns.contains(currItem->pattern())))
				{
					double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation;
					currItem->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation);
					QMatrix qmatrix;
					qmatrix.translate(patternOffsetX, patternOffsetY);
					qmatrix.rotate(patternRotation);
					qmatrix.scale(patternScaleX / 100.0, patternScaleY / 100.0);
					QImage pat = *doc->docPatterns[currItem->pattern()].getPattern();
					QBrush brush = QBrush(pat);
					brush.setMatrix(qmatrix);
					pItem->setBrush(brush);
				}
			}
			else
			{
				QColor paint = ScColorEngine::getShadeColorProof(doc->PageColors[currItem->fillColor()], doc, currItem->fillShade());
				paint.setAlphaF(1.0 - currItem->fillTransparency());
				pItem->setBrush(paint);
			}
		}
		if ((currItem->lineColor() == CommonStrings::None) || (currItem->controlsGroup()))
			pItem->setPen(Qt::NoPen);
		else
		{
			QColor paint = ScColorEngine::getShadeColorProof(doc->PageColors[currItem->lineColor()], doc, currItem->lineShade());
			paint.setAlphaF(1.0 - currItem->lineTransparency());
			pItem->setPen(QPen(paint, currItem->lineWidth(), currItem->lineStyle(), currItem->lineEnd(), currItem->lineJoin()));
		}
		if (currItem->controlsGroup())
		{
			groupStack.push(currItem->groupsLastItem);
			groupStack2.push(pItem);
			groupStack3.push(currItem);
			pItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
		}
		if (groupStack.count() != 0)
		{
			while (currItem == groupStack.top())
			{
				groupStack3.pop();
				groupStack2.pop();
				groupStack.pop();
				if (groupStack.count() == 0)
					break;
			}
		}
	}
	for(unsigned dim = 0; dim < 2; dim++)
	{
		sb2[dim].us = 2;
		sb2[dim].vs = 2;
		const int depth = sb2[dim].us*sb2[dim].vs;
		sb2[dim].resize(depth, Linear2d(0));
	}
	handles.resize(sb2[0].vs*sb2[0].us*4);
	origHandles.resize(sb2[0].vs*sb2[0].us*4);
	unsigned ii = 0;
	for(unsigned vi = 0; vi < sb2[0].vs; vi++)
	{
		for(unsigned ui = 0; ui < sb2[0].us; ui++)
		{
			for(unsigned iv = 0; iv < 2; iv++)
			{
				for(unsigned iu = 0; iu < 2; iu++)
				{
					handles[ii++] = Geom::Point((2*(iu+ui)/(2.*ui+1)+1)*w4, (2*(iv+vi)/(2.*vi+1)+1)*w4);
				}
			}
		}
	}
	Geom::Point dir(1,-2);
	for(unsigned dim = 0; dim < 2; dim++)
	{
		Geom::Point dir(0,0);
		dir[dim] = 1;
		for(unsigned vi = 0; vi < sb2[dim].vs; vi++)
		{
			for(unsigned ui = 0; ui < sb2[dim].us; ui++)
			{
				for(unsigned iv = 0; iv < 2; iv++)
				{
					for(unsigned iu = 0; iu < 2; iu++)
					{
						unsigned corner = iu + 2*iv;
						unsigned i = ui + vi*sb2[dim].us;
						Geom::Point base((2*(iu+ui)/(2.*ui+1)+1)*w4, (2*(iv+vi)/(2.*vi+1)+1)*w4);
						if(vi == 0 && ui == 0)
							base = Geom::Point(w4, w4);
						double dl = dot((handles[corner+4*i] - base), dir)/dot(dir,dir);
						sb2[dim][i][corner] = dl/(ww/2)*pow(4.0f,((int)(ui+vi)));
					}
				}
			}
		}
	}
	QPainterPath pathG;
	D2sb2d2QPainterPath(&pathG, sb2, dir*0.1, ww);
	pItemG = new QGraphicsPathItem(pathG);
	pItemG->setPen(QPen(Qt::black));
	pItemG->setBrush(Qt::NoBrush);
	pItemG->setZValue(8888888);
	scene.addItem(pItemG);
	for(unsigned i = 0; i < handles.size(); i++)
	{
		origHandles[i] = handles[i];
		double x = handles[i][Geom::X];
		double y = handles[i][Geom::Y];
		NodeItem* pItemN = new NodeItem(QRectF(x-4.0, y-4.0, 8.0, 8.0), i, this);
		scene.addItem(pItemN);
		nodeItems.append(pItemN);
	}
	previewLabel->setRenderHint(QPainter::Antialiasing);
	previewLabel->setScene(&scene);
	isFirst = true;
	connect(buttonZoomIn, SIGNAL(clicked()), this, SLOT(doZoomIn()));
	connect(buttonZoomOut, SIGNAL(clicked()), this, SLOT(doZoomOut()));
	connect(resetButton, SIGNAL(clicked()), this, SLOT(doReset()));
}

void MeshDistortionDialog::showEvent(QShowEvent *e)
{
	QDialog::showEvent(e);
	if (isFirst)
	{
		QRectF scR = scene.itemsBoundingRect().adjusted(-50, -50, 50, 50);
		previewLabel->fitInView(scR, Qt::KeepAspectRatio);
		scene.setSceneRect(scR);
		adjustHandles();
	}
	isFirst = false;
}

void MeshDistortionDialog::doZoomIn()
{
	previewLabel->scale(2.0, 2.0);
	adjustHandles();
}

void MeshDistortionDialog::doZoomOut()
{
	previewLabel->scale(0.5, 0.5);
	adjustHandles();
}

void MeshDistortionDialog::doReset()
{
	bool found = false;
	for(int n = 0; n < nodeItems.count(); n++)
	{
		if (nodeItems.at(n)->isSelected())
		{
			found = true;
			handles[nodeItems.at(n)->handle] = origHandles[nodeItems.at(n)->handle];
		}
	}
	if (!found)
	{
		for(unsigned i = 0; i < handles.size(); i++)
		{
			handles[i] = origHandles[i];
		}
	}
	adjustHandles();
	updateMesh(false);
}

void MeshDistortionDialog::adjustHandles()
{
	double sc = previewLabel->matrix().m11();
	for(int n = 0; n < nodeItems.count(); n++)
	{
		double x = handles[n][Geom::X];
		double y = handles[n][Geom::Y];
		QPointF mm = nodeItems.at(n)->mapFromScene(QPointF(x - 4.0 / sc, y - 4.0 / sc));
		nodeItems.at(n)->setRect(QRectF(mm.x(), mm.y(), 8.0 / sc, 8.0 / sc));
	}
}

void MeshDistortionDialog::updateMesh(bool gridOnly)
{
	for(int n = 0; n < nodeItems.count(); n++)
	{
		QPointF mm = nodeItems.at(n)->mapToScene(nodeItems.at(n)->rect().center());
		Geom::Point mouse(mm.x(), mm.y());
		handles[n] = mouse;
	}
	Geom::Point dir(1,-2);
	for(unsigned dim = 0; dim < 2; dim++)
	{
		Geom::Point dir(0,0);
		dir[dim] = 1;
		for(unsigned vi = 0; vi < sb2[dim].vs; vi++)
		{
			for(unsigned ui = 0; ui < sb2[dim].us; ui++)
			{
				for(unsigned iv = 0; iv < 2; iv++)
				{
					for(unsigned iu = 0; iu < 2; iu++)
					{
						unsigned corner = iu + 2*iv;
						unsigned i = ui + vi*sb2[dim].us;
						Geom::Point base((2*(iu+ui)/(2.*ui+1)+1)*w4, (2*(iv+vi)/(2.*vi+1)+1)*w4);
						if(vi == 0 && ui == 0)
							base = Geom::Point(w4, w4);
						double dl = dot((handles[corner+4*i] - base), dir)/dot(dir,dir);
						sb2[dim][i][corner] = dl/(ww/2)*pow(4.0f,((int)(ui+vi)));
					}
				}
			}
		}
	}
	if ((!gridOnly) || (origPathItem.count() < 20))
	{
		if (origPathItem.count() > 19)
			qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
		for (int a = 0; a < origPathItem.count(); a++)
		{
			QGraphicsPathItem* pItem = origPathItem[a];
			Geom::Piecewise<D2<Geom::SBasis> >  path_a_pw = origPath[a];
			Piecewise<D2<SBasis> > output;
			for(unsigned i = 0; i < path_a_pw.size(); i++)
			{
				D2<SBasis> B = path_a_pw[i];
				B *= (2./ww);
				D2<SBasis> tB = compose_each(sb2, B);
				B = B*(ww/2) + Geom::Point(w4, w4);
				tB = tB*(ww/2) + Geom::Point(w4, w4);
				output.concat(Piecewise<D2<SBasis> >(tB));
			}
			QPainterPath pathP;
			Piecewise2QPainterPath(&pathP, output);
			pathP = pItem->mapFromScene(pathP);
			pItem->setPath(pathP);
		}
		if (origPathItem.count() > 19)
			qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
	}
	QPainterPath pathG;
	D2sb2d2QPainterPath(&pathG, sb2, dir*0.1, ww);
	pItemG->setPath(pathG);
}

void MeshDistortionDialog::updateAndExit()
{
	qApp->changeOverrideCursor(QCursor(Qt::WaitCursor));
	for (int a = 0; a < origPathItem.count(); a++)
	{
		Geom::Piecewise<D2<Geom::SBasis> >  path_a_pw;
		QGraphicsPathItem* pItem = origPathItem[a];
		QPainterPath path = pItem->path();
		FPointArray outputPath;
		outputPath.fromQPainterPath(path);
		PageItem *currItem = m_doc->m_Selection->itemAt(a);
		currItem->PoLine = outputPath;
		currItem->Frame = false;
		currItem->ClipEdited = true;
		currItem->FrameType = 3;
		m_doc->AdjustItemSize(currItem);
		currItem->OldB2 = currItem->width();
		currItem->OldH2 = currItem->height();
		currItem->updateClip();
		currItem->ContourLine = currItem->PoLine.copy();
	}
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}
