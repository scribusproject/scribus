/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/**************************************************************************
*   Copyright (C) 2007 by Franz Schmid                                    *
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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#include "lensdialog.h"

#include <cmath>

#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QRadialGradient>
#include <QStyleOptionGraphicsItem>

#include "commonstrings.h"
#include "iconmanager.h"
#include "pageitem_group.h"
#include "sccolorengine.h"
#include "scpattern.h"
#include "selection.h"

LensItem::LensItem(QRectF geom, LensDialog *parent) : QGraphicsRectItem(geom)
{
	dialog = parent;
	strength = -100.0;
	scaling = 1.0,
	handle = -1;
	setPen(QPen(Qt::black));
	QRadialGradient radialGrad(QPointF(0.5, 0.5), 1.0);
	radialGrad.setColorAt(0.0, QColor(255, 0, 0, 127));
	radialGrad.setColorAt(0.1, QColor(255, 0, 0, 127));
	radialGrad.setColorAt(1.0, QColor(255, 255, 255, 0));
	radialGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
	setBrush(radialGrad);
	setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
	acceptHoverEvents();

}

void LensItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
	Q_UNUSED(widget);
	painter->setPen(QPen(Qt::black, 1.0 / item->levelOfDetail));
	QRadialGradient radialGrad(QPointF(0.5, 0.5), 1.0);
	radialGrad.setColorAt(0.0, QColor(255, 0, 0, 127));
	radialGrad.setColorAt(0.1, QColor(255, 0, 0, 127));
	radialGrad.setColorAt(1.0, QColor(255, 255, 255, 0));
	radialGrad.setCoordinateMode(QGradient::ObjectBoundingMode);
	painter->setBrush(radialGrad);
	painter->drawEllipse(rect().toRect());
	if (item->state & QStyle::State_Selected)
	{
		scaling = item->levelOfDetail;
		double siz = 6.0 / item->levelOfDetail;
		QRectF br = boundingRect();
		painter->setBrush(Qt::NoBrush);
		painter->setPen(QPen(Qt::red, 1.0 / item->levelOfDetail, Qt::DotLine));
		painter->drawRect(br);
		painter->setBrush(Qt::red);
		painter->setPen(Qt::NoPen);
		painter->drawRect(QRectF(br.x() + br.width(), br.y() + br.height(), -siz, -siz));
		painter->drawRect(QRectF(br.x() + br.width(), br.y(), -siz, siz));
		painter->drawRect(QRectF(br.x(), br.y() + br.height(), siz, -siz));
		painter->drawRect(QRectF(br.x(), br.y(), siz, siz));
	}
}

void LensItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	mousePoint = event->screenPos();
	QRectF br = boundingRect();
	double siz = 6.0 / scaling;
	if (QRectF(br.x(), br.y(), siz, siz).contains(event->pos()))
		handle = 0;
	else if (QRectF(br.x() + br.width(), br.y(), -siz, siz).contains(event->pos()))
		handle = 1;
	else if (QRectF(br.x() + br.width(), br.y() + br.height(), -siz, -siz).contains(event->pos()))
		handle = 2;
	else if (QRectF(br.x(), br.y() + br.height(), siz, -siz).contains(event->pos()))
		handle = 3;
	else
		handle = -1;
	QGraphicsItem::mousePressEvent(event);
}

void LensItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	QRectF r = rect();
	double dx = (mousePoint.x() - event->screenPos().x()) / scaling;
	if (handle == 0)
	{
		QPointF tl = r.topLeft();
		tl = tl - QPointF(dx, dx);
		r.setTopLeft(tl);
		setRect(r.normalized());
	}
	else if (handle == 1)
	{
		QPointF tl = r.topRight();
		tl = tl - QPointF(dx, -dx);
		r.setTopRight(tl);
		setRect(r.normalized());
	}
	else if (handle == 2)
	{
		QPointF tl = r.bottomRight();
		tl = tl - QPointF(dx, dx);
		r.setBottomRight(tl);
		setRect(r.normalized());
	}
	else if (handle == 3)
	{
		QPointF tl = r.bottomLeft();
		tl = tl - QPointF(dx, -dx);
		r.setBottomLeft(tl);
		setRect(r.normalized());
	}
	else
		QGraphicsItem::mouseMoveEvent(event);
	mousePoint = event->screenPos();
	dialog->lensSelected(this);
}

void LensItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	handle = -1;
	updateEffect();
	QGraphicsItem::mouseReleaseEvent(event);
}

void LensItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	QPainterPath p;
	p.addEllipse(rect());
	if ((p.contains(event->pos())) && (isSelected()))
		qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
	else
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void LensItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
	QPainterPath p;
	p.addEllipse(rect());
	if (isSelected())
	{
		if (p.contains(event->pos()))
			qApp->changeOverrideCursor(QCursor(Qt::SizeAllCursor));
		else
		{
			QRectF br = boundingRect();
			double siz = 6.0 / scaling;
			if (QRectF(br.x(), br.y(), siz, siz).contains(event->pos()))
				qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
			else if (QRectF(br.x() + br.width(), br.y(), -siz, siz).contains(event->pos()))
				qApp->changeOverrideCursor(QCursor(Qt::SizeBDiagCursor));
			else if (QRectF(br.x() + br.width(), br.y() + br.height(), -siz, -siz).contains(event->pos()))
				qApp->changeOverrideCursor(QCursor(Qt::SizeFDiagCursor));
			else if (QRectF(br.x(), br.y() + br.height(), siz, -siz).contains(event->pos()))
				qApp->changeOverrideCursor(QCursor(Qt::SizeBDiagCursor));
			else
				qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
		}
	}
	else
		qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void LensItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
	qApp->changeOverrideCursor(QCursor(Qt::ArrowCursor));
}

void LensItem::setStrength(double s)
{
	strength = s;
}

QVariant LensItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change)
	{
		case ItemSelectedChange:
			dialog->lensSelected(this);
			break;
		case ItemPositionHasChanged:
			dialog->setLensPositionValues(mapToScene(rect().center()));
			updateEffect();
			break;
		default:
			break;
	}
	return QGraphicsItem::itemChange(change, value);
}

void LensItem::updateEffect()
{
	LensItem *item;
	for (int a = 0; a < dialog->origPathItem.count(); a++)
	{
		QGraphicsPathItem* pItem = dialog->origPathItem[a];
		QPainterPath path = dialog->origPath[a];
		path = pItem->mapToScene(path);
		for (int b = 0; b < dialog->lensList.count(); b++)
		{
			item = dialog->lensList[b];
			path = lensDeform(path, item->mapToScene(item->rect().center()), item->rect().width() / 2.0, item->strength / 100.0);
		}
		path = pItem->mapFromScene(path);
		pItem->setPath(path);
	}
}

QPainterPath LensItem::lensDeform(const QPainterPath &source, const QPointF &offset, double m_radius, double s)
{
	QPainterPath path;
	path.addPath(source);
	for (int i = 0; i < path.elementCount(); ++i)
	{
		const QPainterPath::Element &e = path.elementAt(i);
		double dx = e.x - offset.x();
		double dy = e.y - offset.y();
		double len = m_radius - sqrt(dx * dx + dy * dy);
		if (len > 0)
			path.setElementPositionAt(i, e.x - s * dx * len / m_radius, e.y - s * dy * len / m_radius);
	}
	return path;
}

LensDialog::LensDialog(QWidget* parent, ScribusDoc *doc) : QDialog(parent)
{
	setupUi(this);
	buttonRemove->setEnabled(false);
	setModal(true);
	buttonZoomOut->setIcon(QIcon(IconManager::instance().loadIcon("16/zoom-out.png")));
	buttonZoomI->setIcon(QIcon(IconManager::instance().loadIcon("16/zoom-in.png")));
	addItemsToScene(doc->m_Selection, doc, nullptr, nullptr);
	previewWidget->setRenderHint(QPainter::Antialiasing);
	previewWidget->setScene(&scene);
	isFirst = true;
	addLens();
	connect(spinXPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensX(double)));
	connect(spinYPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensY(double)));
	connect(spinRadius, SIGNAL(valueChanged(double)), this, SLOT(setNewLensRadius(double)));
	connect(spinStrength, SIGNAL(valueChanged(double)), this, SLOT(setNewLensStrength(double)));
	connect(buttonAdd, SIGNAL(clicked()), this, SLOT(addLens()));
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeLens()));
	connect(buttonMagnify, SIGNAL(toggled(bool)), this, SLOT(changeLens()));
	connect(buttonZoomI, SIGNAL(clicked()), this, SLOT(doZoomIn()));
	connect(buttonZoomOut, SIGNAL(clicked()), this, SLOT(doZoomOut()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(&scene, SIGNAL(selectionChanged()), this, SLOT(selectionHasChanged()));
}

void LensDialog::addItemsToScene(Selection* itemSelection, ScribusDoc *doc, QGraphicsPathItem* parentItem, PageItem* parent)
{
	PageItem *currItem;
	double gx, gy, gh, gw;
	itemSelection->setGroupRect();
	itemSelection->getGroupRect(&gx, &gy, &gw, &gh);
	uint selectedItemCount = itemSelection->count();
	for (uint i = 0; i < selectedItemCount; ++i)
	{
		currItem = itemSelection->itemAt(i);
		if (currItem->isGroup())
			currItem->asGroupFrame()->adjustXYPosition();
		FPointArray path = currItem->PoLine;
		QPainterPath pp;
		if (currItem->itemType() == PageItem::PolyLine)
			pp = path.toQPainterPath(false);
		else
			pp = path.toQPainterPath(true);
		origPath.append(pp);
		QGraphicsPathItem* pItem = new QGraphicsPathItem(pp, parentItem);
		if (parentItem == nullptr)
		{
			scene.addItem(pItem);
			pItem->setPos(currItem->xPos() - gx, currItem->yPos() - gy);
			pItem->setRotation(currItem->rotation());
		}
		else
		{
			QTransform mm;
			mm.rotate(-parent->rotation());
			mm.translate(-parent->xPos(), -parent->yPos());
			pItem->setPos(mm.map(QPointF(currItem->xPos(), currItem->yPos())));
		}
		pItem->setZValue(i);
		origPathItem.append(pItem);
		origPageItem.append(currItem);
		if (((currItem->fillColor() == CommonStrings::None) && (currItem->GrType == 0)) || (currItem->isGroup()))
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
					for (int offset = 0 ; offset < colorStops.count() ; offset++)
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
					double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY;
					currItem->patternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY);
					QTransform qmatrix;
					qmatrix.translate(patternOffsetX, patternOffsetY);
					qmatrix.rotate(patternRotation);
					qmatrix.shear(patternSkewX, patternSkewY);
					qmatrix.scale(patternScaleX / 100.0, patternScaleY / 100.0);
					bool mirrorX, mirrorY;
					currItem->patternFlip(mirrorX, mirrorY);
					if (mirrorX)
						qmatrix.scale(-1, 1);
					if (mirrorY)
						qmatrix.scale(1, -1);
					QImage pat = *doc->docPatterns[currItem->pattern()].getPattern();
					QBrush brush = QBrush(pat);
					brush.setTransform(qmatrix);
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
		if (currItem->isGroup())
			pItem->setPen(Qt::NoPen);
		else if (currItem->NamedLStyle.isEmpty())
		{
			if ((!currItem->strokePattern().isEmpty()) && (doc->docPatterns.contains(currItem->strokePattern())))
			{
				double patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace;
				currItem->strokePatternTransform(patternScaleX, patternScaleY, patternOffsetX, patternOffsetY, patternRotation, patternSkewX, patternSkewY, patternSpace);
				QTransform qmatrix;
				qmatrix.translate(-currItem->lineWidth() / 2.0, -currItem->lineWidth() / 2.0);
				qmatrix.translate(patternOffsetX, patternOffsetY);
				qmatrix.rotate(patternRotation);
				qmatrix.shear(patternSkewX, patternSkewY);
				qmatrix.scale(patternScaleX / 100.0, patternScaleY / 100.0);
				bool mirrorX, mirrorY;
				currItem->strokePatternFlip(mirrorX, mirrorY);
				if (mirrorX)
					qmatrix.scale(-1, 1);
				if (mirrorY)
					qmatrix.scale(1, -1);
				QImage pat = *doc->docPatterns[currItem->strokePattern()].getPattern();
				QBrush brush = QBrush(pat);
				brush.setTransform(qmatrix);
				pItem->setPen(QPen(brush, currItem->lineWidth(), currItem->lineStyle(), currItem->lineEnd(), currItem->lineJoin()));
			}
			else if (currItem->GrTypeStroke > 0)
			{
				QGradient pat;
				double x1 = currItem->GrStrokeStartX;
				double y1 = currItem->GrStrokeStartY;
				double x2 = currItem->GrStrokeEndX;
				double y2 = currItem->GrStrokeEndY;
				if (currItem->GrTypeStroke == 6)
					pat = QLinearGradient(x1, y1,  x2, y2);
				else
					pat = QRadialGradient(x1, y1, sqrt(pow(x2 - x1, 2) + pow(y2 - y1,2)), x1, y1);
				QList<VColorStop*> colorStops = currItem->stroke_gradient.colorStops();
				QColor qStopColor;
				for (int offset = 0 ; offset < colorStops.count() ; offset++)
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
				pItem->setPen(QPen(pat, currItem->lineWidth(), currItem->lineStyle(), currItem->lineEnd(), currItem->lineJoin()));
			}
			else if (currItem->lineColor() != CommonStrings::None)
			{
				QColor paint = ScColorEngine::getShadeColorProof(doc->PageColors[currItem->lineColor()], doc, currItem->lineShade());
				paint.setAlphaF(1.0 - currItem->lineTransparency());
				pItem->setPen(QPen(paint, currItem->lineWidth(), currItem->lineStyle(), currItem->lineEnd(), currItem->lineJoin()));
			}
		}
		else
		{
			if (currItem->lineColor() != CommonStrings::None)
			{
				QColor paint = ScColorEngine::getShadeColorProof(doc->PageColors[currItem->lineColor()], doc, currItem->lineShade());
				paint.setAlphaF(1.0 - currItem->lineTransparency());
				pItem->setPen(QPen(paint, currItem->lineWidth(), currItem->lineStyle(), currItem->lineEnd(), currItem->lineJoin()));
			}
		}
		if (currItem->isGroup())
		{
			pItem->setFlags(QGraphicsItem::ItemClipsChildrenToShape);
			Selection tmpSelection(this, false);
			for (int a = 0; a < currItem->groupItemList.count(); a++)
			{
				tmpSelection.addItem(currItem->groupItemList.at(a));
			}
			addItemsToScene(&tmpSelection, doc, pItem, currItem);
		}
	}

}

void LensDialog::showEvent(QShowEvent *e)
{
	QDialog::showEvent(e);
	if (isFirst)
		previewWidget->fitInView(scene.itemsBoundingRect(), Qt::KeepAspectRatio);
	isFirst = false;
}

void LensDialog::doZoomIn()
{
	previewWidget->scale(2.0, 2.0);
}

void LensDialog::doZoomOut()
{
	previewWidget->scale(0.5, 0.5);
}

void LensDialog::addLens()
{
	disconnect(spinXPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensX(double)));
	disconnect(spinYPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensY(double)));
	disconnect(spinRadius, SIGNAL(valueChanged(double)), this, SLOT(setNewLensRadius(double)));
	disconnect(spinStrength, SIGNAL(valueChanged(double)), this, SLOT(setNewLensStrength(double)));
	disconnect(buttonMagnify, SIGNAL(toggled(bool)), this, SLOT(changeLens()));
	if (lensList.count() > 0)
	{
		lensList[currentLens]->setSelected(false);
		lensList[currentLens]->update();
	}
	QRectF bBox = scene.itemsBoundingRect();
	double r = qMin(bBox.width(), bBox.height());
	double x = (bBox.width() - r) / 2.0;
	double y = (bBox.height() - r) / 2.0;
	LensItem *item = new LensItem(QRectF(x, y, r, r), this);
	scene.addItem(item);
	lensList.append(item);
	currentLens = lensList.count() - 1;
	item->setZValue(currentLens+999999);
	spinXPos->setValue(x + r / 2.0);
	spinYPos->setValue(y + r / 2.0);
	spinRadius->setValue(r / 2.0);
	spinStrength->setValue(100.0);
	buttonMagnify->setChecked(true);
	lensList[currentLens]->updateEffect();
	if (lensList.count() > 1)
		buttonRemove->setEnabled(true);
	lensList[currentLens]->setSelected(true);
	connect(spinXPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensX(double)));
	connect(spinYPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensY(double)));
	connect(spinRadius, SIGNAL(valueChanged(double)), this, SLOT(setNewLensRadius(double)));
	connect(spinStrength, SIGNAL(valueChanged(double)), this, SLOT(setNewLensStrength(double)));
	connect(buttonMagnify, SIGNAL(toggled(bool)), this, SLOT(changeLens()));
}

void LensDialog::removeLens()
{
	LensItem *item = lensList.takeAt(currentLens);
	scene.removeItem(item);
	delete item;
	if (lensList.count() > 1)
		buttonRemove->setEnabled(true);
	else
		buttonRemove->setEnabled(false);
	currentLens = lensList.count() - 1;
	lensList[currentLens]->setSelected(true);
	lensList[currentLens]->updateEffect();
	lensSelected(lensList[currentLens]);
}

void LensDialog::changeLens()
{
	double s = qAbs(lensList[currentLens]->strength);
	if (buttonMagnify->isChecked())
		lensList[currentLens]->setStrength(s * -1.0);
	else
		lensList[currentLens]->setStrength(s);
	lensList[currentLens]->updateEffect();
}

void LensDialog::selectionHasChanged()
{
	bool setter = true;
	if (scene.selectedItems().count() == 0)
		setter = false;
	spinXPos->setEnabled(setter);
	spinYPos->setEnabled(setter);
	spinRadius->setEnabled(setter);
	spinStrength->setEnabled(setter);
	buttonMagnify->setEnabled(setter);
	buttonFishEye->setEnabled(setter);
	if (lensList.count() == 1)
		buttonRemove->setEnabled(false);
	else
		buttonRemove->setEnabled(setter);
}

void LensDialog::lensSelected(LensItem *item)
{
	disconnect(spinXPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensX(double)));
	disconnect(spinYPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensY(double)));
	disconnect(spinRadius, SIGNAL(valueChanged(double)), this, SLOT(setNewLensRadius(double)));
	disconnect(spinStrength, SIGNAL(valueChanged(double)), this, SLOT(setNewLensStrength(double)));
	disconnect(buttonMagnify, SIGNAL(toggled(bool)), this, SLOT(changeLens()));
	QPointF p = item->mapToScene(item->rect().center());
	spinXPos->setValue(p.x());
	spinYPos->setValue(p.y());
	spinRadius->setValue(item->rect().width() / 2.0);
	if (item->strength < 0.0)
		buttonMagnify->setChecked(true);
	else
		buttonFishEye->setChecked(true);
	spinStrength->setValue(qAbs(item->strength));
	currentLens = lensList.indexOf(item);
	if (currentLens < 0)
		currentLens = 0;
	connect(spinXPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensX(double)));
	connect(spinYPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensY(double)));
	connect(spinRadius, SIGNAL(valueChanged(double)), this, SLOT(setNewLensRadius(double)));
	connect(spinStrength, SIGNAL(valueChanged(double)), this, SLOT(setNewLensStrength(double)));
	connect(buttonMagnify, SIGNAL(toggled(bool)), this, SLOT(changeLens()));
}

void LensDialog::setLensPositionValues(QPointF p)
{
	disconnect(spinXPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensX(double)));
	disconnect(spinYPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensY(double)));
	spinXPos->setValue(p.x());
	spinYPos->setValue(p.y());
	connect(spinXPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensX(double)));
	connect(spinYPos, SIGNAL(valueChanged(double)), this, SLOT(setNewLensY(double)));
}

void LensDialog::setNewLensX(double x)
{
	QRectF r = lensList[currentLens]->rect();
	r.moveCenter(QPointF(x, r.center().y()));
	lensList[currentLens]->setRect(r);
	lensList[currentLens]->updateEffect();
}

void LensDialog::setNewLensY(double y)
{
	QRectF r = lensList[currentLens]->rect();
	r.moveCenter(QPointF(r.center().x(), y));
	lensList[currentLens]->setRect(r);
	lensList[currentLens]->updateEffect();
}

void LensDialog::setNewLensRadius(double radius)
{
	QRectF r = lensList[currentLens]->rect();
	QPointF center = r.center();
	r.setWidth(radius * 2.0);
	r.setHeight(radius * 2.0);
	QPointF centerN = r.center();
	r.translate(center.x() - centerN.x(), center.y() - centerN.y());
	setLensPositionValues(lensList[currentLens]->mapToScene(r.center()));
	lensList[currentLens]->setRect(r);
	lensList[currentLens]->updateEffect();
}

void LensDialog::setNewLensStrength(double s)
{
	if (buttonMagnify->isChecked())
		lensList[currentLens]->setStrength(s * -1.0);
	else
		lensList[currentLens]->setStrength(s);
	lensList[currentLens]->updateEffect();
}
