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
#include "util.h"
#include "util_icon.h"
#include "fpointarray.h"
#include "sccolorengine.h"

#include <QPixmap>
#include <QPainter>
#include <QMatrix>

PathFinderDialog::PathFinderDialog(QWidget* parent, ScribusDoc* doc, PageItem *shape1, PageItem *shape2) : QDialog( parent )
{
	currDoc = doc;
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
	otherColorComboLine->insertItems(currDoc->PageColors, ColorCombo::fancyPixmaps);
	otherColorComboFill->addItem(CommonStrings::tr_NoneColor);
	otherColorComboFill->insertItems(currDoc->PageColors, ColorCombo::fancyPixmaps);
	setCurrentComboItem(otherColorComboLine, shape1->lineColor());
	setCurrentComboItem(otherColorComboFill, shape1->fillColor());
	opMode = 0;
	QMatrix ms;
	ms.rotate(shape1->rotation());
	input1 = ms.map(shape1->PoLine.toQPainterPath(true));
	if (shape1->fillEvenOdd())
		input1.setFillRule(Qt::OddEvenFill);
	else
		input1.setFillRule(Qt::WindingFill);
	double dx = shape2->xPos() - shape1->xPos();
	double dy = shape2->yPos() - shape1->yPos();
	QMatrix mm;
	mm.translate(dx, dy);
	mm.rotate(shape2->rotation());
	input2 = mm.map(shape2->PoLine.toQPainterPath(true));
	if (shape2->fillEvenOdd())
		input2.setFillRule(Qt::OddEvenFill);
	else
		input2.setFillRule(Qt::WindingFill);
	result = QPainterPath();
	source1 = shape1;
	source2 = shape2;
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
	QPainterPath tmp = input1;
	input1 = input2;
	input2 = tmp;
	PageItem* item = source1;
	source1 = source2;
	source2 = item;
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
	QRectF bb = input1.boundingRect().united(input2.boundingRect());
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
	QRectF bb = input1.boundingRect().united(input2.boundingRect());
	p.translate(5, 5);
	p.scale(scale, scale);
	p.translate(-bb.x(), -bb.y());
	p.setPen(Qt::black);
	p.setBrush(getColorFromItem(source1->fillColor(), Qt::blue));
	p.drawPath(result);
	p.setBrush(getColorFromItem(source2->fillColor(), Qt::red));
	p.drawPath(result1);
	p.setBrush(color);
	p.drawPath(result2);
	p.end();
	resultShape->setPixmap(pm);
}

void PathFinderDialog::updateAllPreviews()
{
	QRectF bb = input1.boundingRect().united(input2.boundingRect());
	double scaleX = 90.0 / bb.width();
	double scaleY = 90.0 / bb.height();
	double scale = qMin(scaleX, scaleY);
	updatePreview(sourceShape, input1, getColorFromItem(source1->fillColor(), Qt::blue), scale);
	updatePreview(sourceShape2, input2, getColorFromItem(source2->fillColor(), Qt::red), scale);
	updateResult();
}

void PathFinderDialog::updateResult()
{
	result = QPainterPath();
	result1 = QPainterPath();
	result2 = QPainterPath();
	if (opMode == 0)
	{
		result = input1.united(input2);
	}
	else if (opMode == 1)
	{
		result = input1.subtracted(input2);
	}
	else if (opMode == 2)
	{
		result = input1.intersected(input2);
	}
	else if (opMode == 3)
	{
		QPainterPath part1 = input1.subtracted(input2);
		QPainterPath part2 = input2.subtracted(input1);
		result.addPath(part1);
		result.addPath(part2);
	}
	else if (opMode == 4)
	{
		QPainterPath part1 = input1.subtracted(input2);
		QPainterPath part2 = input2.subtracted(input1);
		QPainterPath part3 = input1.intersected(input2);
		result.addPath(part1);
		result1.addPath(part2);
		result2.addPath(part3);
	}
	QRectF bb = input1.boundingRect().united(input2.boundingRect());
	double scaleX = 90.0 / bb.width();
	double scaleY = 90.0 / bb.height();
	double scale = qMin(scaleX, scaleY);
	QColor cc = Qt::red;
	if (targetGetsSource1Color->isChecked())
	{
		cc = getColorFromItem(source1->fillColor(), Qt::blue);
		targetColor = 0;
		setCurrentComboItem(otherColorComboLine, source1->lineColor());
		setCurrentComboItem(otherColorComboFill, source1->fillColor());
	}
	else if (targetGetsSource2Color->isChecked())
	{
		targetColor = 1;
		cc = getColorFromItem(source2->fillColor(), Qt::red);
		setCurrentComboItem(otherColorComboLine, source2->lineColor());
		setCurrentComboItem(otherColorComboFill, source2->fillColor());
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
		ScColor m_color = currDoc->PageColors[fill];
		out = ScColorEngine::getDisplayColor(m_color, currDoc);
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
