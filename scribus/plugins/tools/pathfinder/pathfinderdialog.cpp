/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          PathFinder.cpp  -  description
                             -------------------
    begin                : Fri Jun 29 2007
    copyright            : (C) 2007 by Franz Schmid
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
#include "pathfinderdialog.h"

#include "fpointarray.h"
#include "pageitem.h"
#include "sccolorengine.h"
#include "scribusdoc.h"
#include "util.h"
#include "util_icon.h"

#include <QPixmap>
#include <QPainter>
#include <QTransform>

PathFinderDialog::PathFinderDialog(QWidget* parent, ScribusDoc* doc, PageItem *shape1, PageItem *shape2) : QDialog( parent )
{
	m_doc = doc;
	setupUi(this);
	setModal(true);
	setWindowIcon(QIcon(loadIcon("AppIcon.png")));
	opCombine->setText( QString::null );
	opSubtraction->setText( QString::null );
	opIntersection->setText( QString::null );
	opExclusion->setText( QString::null );
	opParts->setText( QString::null );
	opExclusion->setIcon(QIcon(loadIcon("pathexclusion.png")));
	opIntersection->setIcon(QIcon(loadIcon("pathintersection.png")));
	opParts->setIcon(QIcon(loadIcon("pathparts.png")));
	opSubtraction->setIcon(QIcon(loadIcon("pathsubtraction.png")));
	opCombine->setIcon(QIcon(loadIcon("pathunite.png")));
	otherColorComboLine->addItem(CommonStrings::tr_NoneColor);
	otherColorComboLine->insertItems(m_doc->PageColors, ColorCombo::fancyPixmaps);
	otherColorComboFill->addItem(CommonStrings::tr_NoneColor);
	otherColorComboFill->insertItems(m_doc->PageColors, ColorCombo::fancyPixmaps);
	setCurrentComboItem(otherColorComboLine, shape1->lineColor());
	setCurrentComboItem(otherColorComboFill, shape1->fillColor());
	opMode = 0;
	QTransform ms;
	ms.rotate(shape1->rotation());
	m_input1 = ms.map(shape1->PoLine.toQPainterPath(true));
	if (shape1->fillEvenOdd())
		m_input1.setFillRule(Qt::OddEvenFill);
	else
		m_input1.setFillRule(Qt::WindingFill);
	double dx = shape2->xPos() - shape1->xPos();
	double dy = shape2->yPos() - shape1->yPos();
	QTransform mm;
	mm.translate(dx, dy);
	mm.rotate(shape2->rotation());
	m_input2 = mm.map(shape2->PoLine.toQPainterPath(true));
	if (shape2->fillEvenOdd())
		m_input2.setFillRule(Qt::OddEvenFill);
	else
		m_input2.setFillRule(Qt::WindingFill);
	result = QPainterPath();
	m_source1 = shape1;
	m_source2 = shape2;
	swapped = false;
	targetColor = 0;
	keepItem1 = false;
	keepItem2 = false;
	updateAllPreviews();
	connect(targetGetsSource1Color, SIGNAL(clicked()), this, SLOT(updateResult()));
	connect(targetGetsSource2Color, SIGNAL(clicked()), this, SLOT(updateResult()));
	connect(targetGetsOtherColor, SIGNAL(clicked()), this, SLOT(updateResult()));
	connect(otherColorComboFill, SIGNAL(activated(int)), this, SLOT(updateResult()));
	connect(keepSource1, SIGNAL(clicked()), this, SLOT(checkKeep()));
	connect(keepSource2, SIGNAL(clicked()), this, SLOT(checkKeep()));
	connect(opCombine, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(opSubtraction, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(opIntersection, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(opExclusion, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(opParts, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(swapShapes, SIGNAL(clicked()), this, SLOT(swapObjects()));
}

void PathFinderDialog::checkKeep()
{
	keepItem1 = keepSource1->isChecked();
	keepItem2 = keepSource2->isChecked();
}

void PathFinderDialog::newOpMode()
{
	optionGroup->setTitle( tr("Result gets Color of:"));
	if (opCombine->isChecked())
	{
		label->setText("+");
		opMode = 0;
	}
	else if (opSubtraction->isChecked())
	{
		label->setText("-");
		opMode = 1;
	}
	else if (opIntersection->isChecked())
	{
		label->setText(QChar(0x2229));
		opMode = 2;
	}
	else if (opExclusion->isChecked())
	{
		label->setText(QChar(0x2206));
		opMode = 3;
	}
	else if (opParts->isChecked())
	{
		label->setText(" ");
		optionGroup->setTitle( tr("Intersection gets Color of:"));
		opMode = 4;
	}
	updateResult();
}

void PathFinderDialog::swapObjects()
{
	QPainterPath tmp = m_input1;
	m_input1 = m_input2;
	m_input2 = tmp;
	PageItem* item = m_source1;
	m_source1 = m_source2;
	m_source2 = item;
	swapped = !swapped;
	updateAllPreviews();
}

void PathFinderDialog::updatePreview(QLabel *label, QPainterPath &path, QColor color, double scale)
{
	QPixmap pm(100, 100);
	QPainter p;
	p.begin(&pm);
	p.setRenderHint(QPainter::Antialiasing, true);
	QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
	p.fillRect(0, 0, pm.width(), pm.height(), b);
	QRectF bb = m_input1.boundingRect().united(m_input2.boundingRect());
	p.translate(5, 5);
	p.scale(scale, scale);
	p.translate(-bb.x(), -bb.y());
	p.setPen(Qt::black);
	p.setBrush(color);
	p.drawPath(path);
	p.end();
	label->setPixmap(pm);
}

void PathFinderDialog::updatePartPreview(QColor color, double scale)
{
	QPixmap pm(100, 100);
	QPainter p;
	p.begin(&pm);
	p.setRenderHint(QPainter::Antialiasing, true);
	QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
	p.fillRect(0, 0, pm.width(), pm.height(), b);
	QRectF bb = m_input1.boundingRect().united(m_input2.boundingRect());
	p.translate(5, 5);
	p.scale(scale, scale);
	p.translate(-bb.x(), -bb.y());
	p.setPen(Qt::black);
	p.setBrush(getColorFromItem(m_source1->fillColor(), Qt::blue));
	p.drawPath(result);
	p.setBrush(getColorFromItem(m_source2->fillColor(), Qt::red));
	p.drawPath(result1);
	p.setBrush(color);
	p.drawPath(result2);
	p.end();
	resultShape->setPixmap(pm);
}

void PathFinderDialog::updateAllPreviews()
{
	QRectF bb = m_input1.boundingRect().united(m_input2.boundingRect());
	double scaleX = 90.0 / bb.width();
	double scaleY = 90.0 / bb.height();
	double scale = qMin(scaleX, scaleY);
	updatePreview(sourceShape, m_input1, getColorFromItem(m_source1->fillColor(), Qt::blue), scale);
	updatePreview(sourceShape2, m_input2, getColorFromItem(m_source2->fillColor(), Qt::red), scale);
	updateResult();
}

void PathFinderDialog::updateResult()
{
	result = QPainterPath();
	result1 = QPainterPath();
	result2 = QPainterPath();
	if (opMode == 0)
	{
		result = m_input1.united(m_input2);
	}
	else if (opMode == 1)
	{
		result = m_input1.subtracted(m_input2);
	}
	else if (opMode == 2)
	{
		result = m_input1.intersected(m_input2);
	}
	else if (opMode == 3)
	{
		QPainterPath part1 = m_input1.subtracted(m_input2);
		QPainterPath part2 = m_input2.subtracted(m_input1);
		result.addPath(part1);
		result.addPath(part2);
	}
	else if (opMode == 4)
	{
		QPainterPath part1 = m_input1.subtracted(m_input2);
		QPainterPath part2 = m_input2.subtracted(m_input1);
		QPainterPath part3 = m_input1.intersected(m_input2);
		result.addPath(part1);
		result1.addPath(part2);
		result2.addPath(part3);
	}
	QRectF bb = m_input1.boundingRect().united(m_input2.boundingRect());
	double scaleX = 90.0 / bb.width();
	double scaleY = 90.0 / bb.height();
	double scale = qMin(scaleX, scaleY);
	QColor cc = Qt::red;
	if (targetGetsSource1Color->isChecked())
	{
		cc = getColorFromItem(m_source1->fillColor(), Qt::blue);
		targetColor = 0;
		setCurrentComboItem(otherColorComboLine, m_source1->lineColor());
		setCurrentComboItem(otherColorComboFill, m_source1->fillColor());
	}
	else if (targetGetsSource2Color->isChecked())
	{
		targetColor = 1;
		cc = getColorFromItem(m_source2->fillColor(), Qt::red);
		setCurrentComboItem(otherColorComboLine, m_source2->lineColor());
		setCurrentComboItem(otherColorComboFill, m_source2->fillColor());
	}
	else if (targetGetsOtherColor->isChecked())
	{
		cc = getColorFromItem(getOtherFillColor(), Qt::green);
		targetColor = 2;
	}
	if (opMode == 4)
		updatePartPreview(cc, scale);
	else
		updatePreview(resultShape, result, cc, scale);
}

QColor PathFinderDialog::getColorFromItem(QString color, QColor in)
{
	QColor out = in;
	QString fill = color;
	if (fill == CommonStrings::tr_NoneColor)
		fill = CommonStrings::None;
	if (fill != CommonStrings::None)
	{
		ScColor color = m_doc->PageColors[fill];
		out = ScColorEngine::getDisplayColor(color, m_doc);
	}
	return out;
}

const QString PathFinderDialog::getOtherFillColor()
{
	return otherColorComboFill->currentText();
}

const QString PathFinderDialog::getOtherLineColor()
{
	return otherColorComboLine->currentText();
}
