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
#include "util_gui.h"

// Note : use temporarily editingFinished() signal to track value changes in stopPos, stopOpacity and
// stopShade spinboxes. valueChanged() cause focus problems in these widgets due to signals emitted
// by ScribusDoc::itemSelection_SetFillGradient() and itemSelection_SetStrokeGradient()

GradientEditor::GradientEditor(QWidget *pa) : QFrame(pa)
{
	setupUi(this);

	stopPos->setDecimals(0);
	stopOpacity->setDecimals(0);
	stopShade->setDecimals(0);

	colorListBox = new ColorListBox(ColorListBox::fancyPixmap);
	buttonColor->setContextWidget(colorListBox);
	setExtendVisible(false);
	initExtend();

	connect(stopPos    , SIGNAL(valueChanged(double)), this, SLOT(changePos(double)));
	connect(stopOpacity, SIGNAL(valueChanged(double)), this, SLOT(setStopColor()));
	connect(stopShade  , SIGNAL(valueChanged(double)), this, SLOT(setStopColor()));
	connect(Preview, SIGNAL(selectedStop(VColorStop*)), this, SLOT(slotDisplayStop(VColorStop*)));
	connect(Preview, SIGNAL(selectedPosition(double)), this, SLOT(setPos(double)));
	connect(colorListBox, SIGNAL(itemSelectionChanged()), this, SLOT(setStopColor()));
	connect(this, SIGNAL(gradientChanged()), this, SLOT(updateColorButton()));
	connect(comboExtend, &QComboBox::currentIndexChanged, this, &GradientEditor::repeatMethodChanged);
}

void GradientEditor::setGradient(const VGradient& grad)
{
	Preview->setGradient(grad);
	Preview->updateDisplay();
}

const VGradient& GradientEditor::gradient() const
{
	return Preview->gradient();
}

void GradientEditor::setColors(ColorList &colorList)
{
	m_colorList = colorList;
	colorListBox->setColors(colorList, true);
}

void GradientEditor::setPos(double p)
{
	QSignalBlocker sig1(stopPos);
	stopPos->setValue(qRound(p * 100));
}

void GradientEditor::setGradTrans(double val)
{
	QSignalBlocker sig1(stopOpacity);
	stopOpacity->setValue(qRound(val * 100));
}

void GradientEditor::slotDisplayStop(const VColorStop* stop)
{
	setPos(stop->rampPoint);
	slotColor(stop->name, stop->shade);
	setGradTrans(stop->opacity);
	updateColorButton();
	emit gradientChanged();
}

void GradientEditor::slotColor(const QString& name, int shade)
{
	QSignalBlocker sig1(colorListBox);
	QSignalBlocker sig2(stopShade);

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
		stopShade->setEnabled(Preview->isEditable());
		stopOpacity->setEnabled(Preview->isEditable());
	}
	colorListBox->setCurrentColor(nam);
}

void GradientEditor::setGradientEditable(bool val)
{
	stopShade->setEnabled(val);
	stopOpacity->setEnabled(val);
	buttonColor->setEnabled(val);
	stopPos->setEnabled(val);
	Preview->setIsEditable(val);
}

VGradient::VGradientRepeatMethod GradientEditor::repeatMethod() const
{
	return qvariant_cast<VGradient::VGradientRepeatMethod>(comboExtend->currentData());
}

void GradientEditor::setRepeatMethod(VGradient::VGradientRepeatMethod method)
{
	QSignalBlocker sigExtend(comboExtend);

	int index = comboExtend->findData(method);
	if ( index != -1 )
		comboExtend->setCurrentIndex(index);
	else
		comboExtend->setCurrentIndex(0);

}

void GradientEditor::setExtendVisible(bool visible)
{
	extendLabel->setVisible(visible);
}

void GradientEditor::changePos(double v)
{
	Preview->setActiveStopPosition(v / 100.0);
	emit gradientChanged();
}

QColor GradientEditor::setColor(const QString& colorName, int shad)
{
	const ScColor& color = m_colorList[colorName];
	return ScColorEngine::getShadeColorProof(color, m_colorList.document(), shad);
}

void GradientEditor::setStopColor()
{
	QString color = colorListBox->currentColor();

	if (color == CommonStrings::tr_NoneColor)
	{
		Preview->setActiveStopColor(QColor(0, 0, 0, 0), CommonStrings::None, 100, 0.0);
		stopShade->setEnabled(false);
		stopOpacity->setEnabled(false);
	}
	else
	{
		Preview->setActiveStopColor(setColor(color, stopShade->value()), color, stopShade->value(), stopOpacity->value() / 100.0);
		stopShade->setEnabled(Preview->isEditable());
		stopOpacity->setEnabled(Preview->isEditable());
	}
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
	initExtend();
	retranslateUi(this);
}

void GradientEditor::updateColorButton()
{
	QString colorName = colorListBox->currentColor();
	QBrush brush;

	if (colorName == CommonStrings::tr_NoneColor || colorName == CommonStrings::None)
		brush = renderEmptyPattern(buttonColor->backgroundDotSize());
	else
	{
		const ScColor& color = m_colorList[colorName];

		QColor qColorShade = ScColorEngine::getDisplayColor(color, m_colorList.document(), stopShade->value());
		QColor qColor = ScColorEngine::getDisplayColor(color, m_colorList.document(), 100.0);

		brush = QBrush(renderColor(buttonColor->backgroundDotSize(), qColor, qColorShade, stopOpacity->value() / 100.0));
	}

	buttonColor->setBackground(brush);
}

void GradientEditor::initExtend()
{
	int oldGradientExtendIndex = comboExtend->currentIndex();
	QSignalBlocker sigExtend(comboExtend);
	comboExtend->clear();
	comboExtend->addItem( tr("None"), VGradient::none );
	comboExtend->addItem( tr("Pad"), VGradient::pad );
	comboExtend->setCurrentIndex(oldGradientExtendIndex);
}

bool GradientEditor::event(QEvent * event)
{
	if (event->type() == QEvent::ToolTip)
	{
		const auto *helpEvent = static_cast<QHelpEvent *>(event);
		QToolTip::showText(helpEvent->globalPos(), tr("Add, change or remove color stops here"), Preview, QRect(10, 43, Preview->width() - 20, 13));
	}
	return QWidget::event(event);
}

