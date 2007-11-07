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

#include <QPixmap>
#include <QPainter>
#include <QMatrix>

PathFinderDialog::PathFinderDialog(QWidget* parent, PageItem *shape1, PageItem *shape2) : QDialog( parent )
{
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
	// Disable for now
	opExclusion->setEnabled(false);
	opParts->setEnabled(false);
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
	swapped = false;
	updateAllPreviews();
	connect(targetGetsSource1Color, SIGNAL(clicked()), this, SLOT(updateResult()));
	connect(targetGetsSource2Color, SIGNAL(clicked()), this, SLOT(updateResult()));
	connect(opCombine, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(opSubtraction, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(opIntersection, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(opExclusion, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(opParts, SIGNAL(clicked()), this, SLOT(newOpMode()));
	connect(swapShapes, SIGNAL(clicked()), this, SLOT(swapObjects()));
}

void PathFinderDialog::newOpMode()
{
	if (opCombine->isChecked())
		opMode = 0;
	else if (opSubtraction->isChecked())
		opMode = 1;
	else if (opIntersection->isChecked())
		opMode = 2;
	else if (opExclusion->isChecked())
		opMode = 3;
	else if (opParts->isChecked())
		opMode = 4;
	updateResult();
}

void PathFinderDialog::swapObjects()
{
	QPainterPath tmp = input1;
	input1 = input2;
	input2 = tmp;
	swapped = !swapped;
	updateAllPreviews();
}

void PathFinderDialog::updatePreview(QLabel *label, QPainterPath &path, QColor color, double scale)
{
	QPixmap pm(100, 100);
	pm.fill(Qt::white);
	QPainter p;
	p.begin(&pm);
	p.setRenderHint(QPainter::Antialiasing, true);
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

void PathFinderDialog::updateAllPreviews()
{
	QRectF bb = input1.boundingRect().united(input2.boundingRect());
	double scaleX = 90.0 / bb.width();
	double scaleY = 90.0 / bb.height();
	double scale = qMin(scaleX, scaleY);
	updatePreview(sourceShape, input1, Qt::blue, scale);
	updatePreview(sourceShape2, input2, Qt::red, scale);
	updateResult();
}

void PathFinderDialog::updateResult()
{
	result = QPainterPath();
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
/*	else if (opMode == 3)
	{
		QPainterPath part1 = input1.subtracted(input2);
		QPainterPath part2 = input2.subtracted(input1);
		part1.closeSubpath();
		part2.closeSubpath();
		result.addPath(part1);
		result.addPath(part2);
	}
	else if (opMode == 4)
	{
		QPainterPath part1 = input1.subtracted(input2);
		QPainterPath part2 = input2.subtracted(input1);
		QPainterPath part3 = input1.intersected(input2);
		part1.closeSubpath();
		part2.closeSubpath();
		part3.closeSubpath();
		result.addPath(part1);
		result.addPath(part2);
		result.addPath(part3);
	} */
	QRectF bb = input1.boundingRect().united(input2.boundingRect());
	double scaleX = 90.0 / bb.width();
	double scaleY = 90.0 / bb.height();
	double scale = qMin(scaleX, scaleY);
	QColor cc = Qt::red;
	if (targetGetsSource1Color->isChecked())
		cc = Qt::blue;
	updatePreview(resultShape, result, cc, scale);
}
