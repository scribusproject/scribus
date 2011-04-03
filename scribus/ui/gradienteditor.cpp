/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          gradienteditor  -  description
                             -------------------
    begin                : Mit Mai 26 2004
    copyright            : (C) 2004 by Franz Schmid
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

#include "gradienteditor.h"

#include <QEvent>
#include <QToolTip>
#include "util.h"

// Note : use temporarily editingFinished() signal to track value changes in stopPos, stopOpacity and
// stopShade spinboxes. valueChanged() cause focus problems in these widgets due to signals emitted
// by ScribusDoc::itemSelection_SetFillGradient() and itemSelection_SetStrokeGradient()

GradientEditor::GradientEditor(QWidget *pa) : QFrame(pa)
{
	setupUi(this);
	connect(stopPos    , SIGNAL(valueChanged(int)), this, SLOT(changePos(int)));
	connect(stopColor  , SIGNAL(activated(const QString &)), this, SLOT(setStopColor(const QString &)));
	connect(stopOpacity, SIGNAL(valueChanged(int)), this, SLOT(setStopTrans(int)));
	connect(stopShade  , SIGNAL(valueChanged(int)), this, SLOT(setStopShade(int)));
	connect(Preview, SIGNAL(selectedStop(VColorStop*)), this, SLOT(slotDisplayStop(VColorStop*)));
	connect(Preview, SIGNAL(currStep(double)), this, SLOT(setPos(double)));
	connect(Preview, SIGNAL(currStep(double)), this, SIGNAL(gradientChanged()));
}

void GradientEditor::setGradient(VGradient grad)
{
	Preview->setGradient(grad);
	Preview->updateDisplay();
}

const VGradient GradientEditor::gradient()
{
	return Preview->gradient();
}

void GradientEditor::setColors(ColorList &colorList)
{
	m_colorList = colorList;
	stopColor->updateBox(colorList, ColorCombo::fancyPixmaps, true);
}

void GradientEditor::setPos(double p)
{
	stopPos->blockSignals(true);
	stopPos->setValue(qRound(p * 100));
	stopPos->blockSignals(false);
}

void GradientEditor::setGradTrans(double val)
{
	stopOpacity->blockSignals(true);
	stopOpacity->setValue(qRound(val * 100));
	stopOpacity->blockSignals(false);
}

void GradientEditor::slotDisplayStop(VColorStop* stop)
{
	setPos(stop->rampPoint);
	slotColor(stop->name, stop->shade);
	setGradTrans(stop->opacity);
}

void GradientEditor::slotColor(QString name, int shade)
{
	stopColor->blockSignals(true);
	stopShade->blockSignals(true);
	stopShade->setValue(shade);
	QString nam = name;
	if (name == CommonStrings::None)
	{
		stopShade->setEnabled(false);
		stopOpacity->setEnabled(false);
		nam = CommonStrings::tr_NoneColor;
	}
	else
	{
		stopShade->setEnabled(true);
		stopOpacity->setEnabled(true);
	}
	setCurrentComboItem(stopColor, nam);
	stopColor->blockSignals(false);
	stopShade->blockSignals(false);
}

void GradientEditor::setGradientEditable(bool val)
{
	stopShade->setEnabled(val);
	stopOpacity->setEnabled(val);
	stopColor->setEnabled(val);
	stopPos->setEnabled(val);
	Preview->setGradientEditable(val);
}

void GradientEditor::changePos(int v)
{
	Preview->setActStep(static_cast<double>(v) / 100.0);
	emit gradientChanged();
}

QColor GradientEditor::setColor(QString colorName, int shad)
{
	const ScColor& color = m_colorList[colorName];
	return ScColorEngine::getShadeColorProof(color, m_colorList.document(), shad);
}

void GradientEditor::setStopColor(const QString &Color)
{
	if (Color == CommonStrings::tr_NoneColor)
	{
		Preview->setActColor(QColor(255, 255, 255, 0), CommonStrings::None, 100);
		Preview->setActTrans(0.0);
		stopShade->setEnabled(false);
		stopOpacity->setEnabled(false);
	}
	else
	{
		Preview->setActColor(setColor(Color, stopShade->value()), Color, stopShade->value());
		Preview->setActTrans(stopOpacity->value() / 100.0);
		stopShade->setEnabled(true);
		stopOpacity->setEnabled(true);
	}
	emit gradientChanged();
}

void GradientEditor::setStopTrans(int val)
{
	Preview->setActTrans(static_cast<double>(val) / 100.0);
	emit gradientChanged();
}

void GradientEditor::setStopShade(int val)
{
	Preview->setActColor(setColor(stopColor->currentText(), val), stopColor->currentText(), val);
	emit gradientChanged();
}

void GradientEditor::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void GradientEditor::languageChange()
{
	Desc->setText( tr( "Position:" ) );
	stopPos->setSuffix( tr(" %") );
}

bool GradientEditor::event(QEvent * event)
{
	if (event->type() == QEvent::ToolTip) 
	{
		QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
		QToolTip::showText(helpEvent->globalPos(), tr( "Add, change or remove color stops here" ), Preview, QRect(10,43, Preview->width()-20, 13));
	}
	return QWidget::event(event);
}

