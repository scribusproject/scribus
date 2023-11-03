/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          transparencypalette.cpp  -  description
                             -------------------
    begin                : Tue Nov 17 2009
    copyright            : (C) 2009 by Franz Schmid
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

#include "transparencypalette.h"
#include "sccolorengine.h"
#include "scpainter.h"
#include "scpattern.h"
#include "scribus.h"
#include "iconmanager.h"
#include "util.h"
#include "util_math.h"

TransparencyPalette::TransparencyPalette(QWidget* parent) : QWidget(parent)
{
	TGradDia = new GradientVectorDialog(this->parentWidget());
	TGradDia->hide();
	setupUi(this);
	editLineSelector->setIcon(IconManager::instance().loadIcon("16/color-stroke.png"));
	editFillSelector->setIcon(IconManager::instance().loadIcon("16/color-fill.png"));
	editFillSelector->setChecked(true);
	strokeOpacity->setDecimals(0);
	fillOpacity->setDecimals(0);
	editFillSelectorButton();

	connect(editLineSelector, SIGNAL(clicked()), this, SLOT(editLineSelectorButton()));
	connect(editFillSelector, SIGNAL(clicked()), this, SLOT(editFillSelectorButton()));
	connect(strokeOpacity, SIGNAL(valueChanged(double)), this, SLOT(slotTransS(double)));
	connect(fillOpacity, SIGNAL(valueChanged(double)), this, SLOT(slotTransF(double)));
	connect(blendModeFill, SIGNAL(activated(int)), this, SIGNAL(NewBlend(int)));
	connect(blendModeStroke, SIGNAL(activated(int)), this, SIGNAL(NewBlendS(int)));
	connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	connect(gradEdit, SIGNAL(gradientChanged()), this, SIGNAL(gradientChanged()));
	connect(gradEditButton, SIGNAL(clicked()), this, SLOT(editGradientVector()));
	connect(TGradDia, SIGNAL(NewSpecial(double,double,double,double,double,double,double,double,double,double)), this, SIGNAL(NewSpecial(double,double,double,double,double,double,double,double,double,double)));
	connect(TGradDia, SIGNAL(paletteShown(bool)), this, SLOT(setActiveGradDia(bool)));
	connect(gradientType, SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotGrad(int)));
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	connect(editPatternProps, SIGNAL(clicked()), this, SLOT(changePatternProps()));
	connect(transpCalcGradient, SIGNAL(clicked()), this, SLOT(switchGradientMode()));
	connect(transpCalcPattern, SIGNAL(clicked()), this, SLOT(switchPatternMode()));
	connect(usePatternInverted, SIGNAL(clicked()), this, SLOT(switchPatternMode()));
}

void TransparencyPalette::connectSignals()
{
	connect(gradEdit, SIGNAL(gradientChanged()), this, SIGNAL(gradientChanged()));
	connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotGrad(int)));
	connect(gradientType, SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
	connect(transpCalcGradient, SIGNAL(clicked()), this, SLOT(switchGradientMode()));
	connect(transpCalcPattern, SIGNAL(clicked()), this, SLOT(switchPatternMode()));
	connect(usePatternInverted, SIGNAL(clicked()), this, SLOT(switchPatternMode()));
}

void TransparencyPalette::disconnectSignals()
{
	disconnect(gradEdit, SIGNAL(gradientChanged()), this, SIGNAL(gradientChanged()));
	disconnect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	disconnect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotGrad(int)));
	disconnect(gradientType, SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
	disconnect(transpCalcGradient, SIGNAL(clicked()), this, SLOT(switchGradientMode()));
	disconnect(transpCalcPattern, SIGNAL(clicked()), this, SLOT(switchPatternMode()));
	disconnect(usePatternInverted, SIGNAL(clicked()), this, SLOT(switchPatternMode()));
}

void TransparencyPalette::setCurrentItem(PageItem* item)
{
	currentItem = item;
	disconnectSignals();

	if (!currentItem || !currentDoc)
		return;

	if (currentItem->isGroup())
		hideSelectionButtons();
	else
		showSelectionButtons();

	setActTrans(currentItem->fillTransparency(), currentItem->lineTransparency());
	setActBlend(currentItem->fillBlendmode(), currentItem->lineBlendmode());
	gradEdit->setGradient(currentItem->mask_gradient);
	if (!currentItem->gradientMask().isEmpty())
	{
		setCurrentComboItem(namedGradient, currentItem->gradientMask());
		gradEdit->setGradientEditable(false);
	}
	else
	{
		namedGradient->setCurrentIndex(0);
		gradEdit->setGradientEditable(true);
	}
	if (currentItem->maskType() == 0)
		tabWidget->setCurrentIndex(0);
	else if ((currentItem->maskType() == 1) || (currentItem->maskType() == 2) || (currentItem->maskType() == 4) || (currentItem->maskType() == 5))
		tabWidget->setCurrentIndex(1);
	else
		tabWidget->setCurrentIndex(2);
	if (patternList->isEmpty())
		tabWidget->setTabEnabled(2, false);
	else
		tabWidget->setTabEnabled(2, true);
	transpCalcGradient->setChecked(false);
	transpCalcPattern->setChecked(false);
	usePatternInverted->setChecked(false);
	if ((currentItem->maskType() == 4) || (currentItem->maskType() == 5))
		transpCalcGradient->setChecked(true);
	if ((currentItem->maskType() == 6) || (currentItem->maskType() == 7))
		transpCalcPattern->setChecked(true);
	if ((currentItem->maskType() == 7) || (currentItem->maskType() == 8))
		usePatternInverted->setChecked(true);
	if ((currentItem->maskType() == 1) || (currentItem->maskType() == 4))
		gradientType->setCurrentIndex(0);
	else if ((currentItem->maskType() == 2) || (currentItem->maskType() == 5))
		gradientType->setCurrentIndex(1);
	if(TGradDia && gradEditButton->isChecked())
		TGradDia->setValues(currentItem->GrMaskStartX, currentItem->GrMaskStartY, currentItem->GrMaskEndX, currentItem->GrMaskEndY, currentItem->GrMaskFocalX, currentItem->GrMaskFocalY, currentItem->GrMaskScale, currentItem->GrMaskSkew, 0, 0);
	
	bool mirrorX, mirrorY;
	const ScMaskTransform& maskTrans = currentItem->maskTransform();
	currentItem->maskFlip(mirrorX, mirrorY);
	setCurrentMaskPattern(currentItem->patternMask(), maskTrans, mirrorX, mirrorY);

	connectSignals();
}

void TransparencyPalette::setDocument(ScribusDoc* doc)
{
	this->disconnect(SIGNAL(NewTrans(double)));
	this->disconnect(SIGNAL(NewTransS(double)));
	this->disconnect(SIGNAL(NewGradient(int)));
	this->disconnect(SIGNAL(NewBlend(int)));
	this->disconnect(SIGNAL(NewBlendS(int)));
	this->disconnect(SIGNAL(NewPattern(QString)));
	this->disconnect(SIGNAL(NewPatternProps(double,double,double,double,double,double,double,bool,bool)));
	
	if (currentDoc)
	{
		disconnect(currentDoc->scMW(), SIGNAL(UpdateRequest(int)), this, nullptr);
	}

	currentDoc = doc;

	if (doc != nullptr)
	{
		gradEdit->setColors(doc->PageColors);
		currentUnit = doc->unitIndex();

		updateColorList();

		connect(this, SIGNAL(NewTrans(double)), doc, SLOT(itemSelection_SetItemFillTransparency(double)));
		connect(this, SIGNAL(NewTransS(double)), doc, SLOT(itemSelection_SetItemLineTransparency(double)));
		connect(this, SIGNAL(NewBlend(int)), doc, SLOT(itemSelection_SetItemFillBlend(int)));
		connect(this, SIGNAL(NewBlendS(int)), doc, SLOT(itemSelection_SetItemLineBlend(int)));
		connect(this, SIGNAL(NewGradient(int)), doc, SLOT(itemSelection_SetItemGradMask(int)));
		connect(this, SIGNAL(NewPattern(QString)), doc, SLOT(itemSelection_SetItemPatternMask(QString)));
		connect(this, SIGNAL(NewPatternProps(double,double,double,double,double,double,double,bool,bool)), doc, SLOT(itemSelection_SetItemPatternMaskProps(double,double,double,double,double,double,double,bool,bool)));
		connect(doc->scMW(), SIGNAL(UpdateRequest(int)), this, SLOT(handleUpdateRequest(int)));
	}
}

void TransparencyPalette::handleUpdateRequest(int updateFlags)
{
	if (updateFlags & reqColorsUpdate)
		updateColorList();
}

void TransparencyPalette::updateColorList()
{
	if (!currentDoc)
		return;

	if (currentItem)
		disconnectSignals();

	this->setColors(currentDoc->PageColors);
	this->setPatterns(&currentDoc->docPatterns);
	this->setGradients(&currentDoc->docGradients);

	if (currentItem)
		setCurrentItem(currentItem);
}

void TransparencyPalette::showSelectionButtons()
{
	editLineSelector->show();
	editLineSelector->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	editFillSelector->show();
	editFillSelector->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
}
void TransparencyPalette::hideSelectionButtons()
{
	editLineSelector->hide();
	editLineSelector->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
	editFillSelector->hide();
	editFillSelector->setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored));
	stackedWidget->setCurrentIndex(1);
	editFillSelector->setChecked(true);
}

void TransparencyPalette::updateFromItem()
{
	setCurrentItem(currentItem);
}

void TransparencyPalette::updateCList()
{
	gradEdit->setColors(colorList);
}

void TransparencyPalette::updateGradientList()
{
	disconnect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	namedGradient->clear();
	namedGradient->setIconSize(QSize(48, 12));
	namedGradient->addItem( tr("Custom"));
	QStringList patK = gradientList->keys();
	patK.sort();
	for (int i = 0; i < patK.count(); i++)
	{
		VGradient gr = gradientList->value(patK[i]);
		QImage pixm(48, 12, QImage::Format_ARGB32_Premultiplied);
		QPainter pb;
		QBrush b(QColor(205,205,205), IconManager::instance().loadPixmap("testfill.png"));
		pb.begin(&pixm);
		pb.fillRect(0, 0, 48, 12, b);
		pb.end();
		ScPainter *p = new ScPainter(&pixm, 48, 12);
		p->setPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin);
		p->setFillMode(ScPainter::Gradient);
		p->fill_gradient = gr;
		p->setGradient(VGradient::linear, FPoint(0,6), FPoint(48, 6), FPoint(0,0), 1, 0);
		p->drawRect(0, 0, 48, 12);
		p->end();
		delete p;
		QPixmap pm;
		pm = QPixmap::fromImage(pixm);
		namedGradient->addItem(pm, patK[i]);
	}
	connect(namedGradient, SIGNAL(activated(const QString&)), this, SLOT(setNamedGradient(const QString&)));
}

void TransparencyPalette::setGradients(QHash<QString, VGradient> *docGradients)
{
	gradientList = docGradients;
	updateGradientList();
}

void TransparencyPalette::setColors(const ColorList& newColorList)
{
	colorList.clear();
	colorList = newColorList;
	updateCList();
}

void TransparencyPalette::slotGrad(int number)
{
	if (number == 1)
	{
		disconnect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
		if (!currentItem->gradientMask().isEmpty())
		{
			setCurrentComboItem(namedGradient, currentItem->gradientMask());
			gradEdit->setGradient(gradientList->value(currentItem->gradientMask()));
			gradEdit->setGradientEditable(false);
		}
		else
		{
			namedGradient->setCurrentIndex(0);
			gradEdit->setGradient(currentItem->mask_gradient);
			gradEdit->setGradientEditable(true);
		}
		if (gradientType->currentIndex() == 0)
		{
			if (transpCalcGradient->isChecked())
				emit NewGradient(GradMask_LinearLumAlpha);
			else
				emit NewGradient(GradMask_Linear);
		}
		else
		{
			if (transpCalcGradient->isChecked())
				emit NewGradient(GradMask_RadialLumAlpha);
			else
				emit NewGradient(GradMask_Radial);
		}
		connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	}
	else if (number == 2)
	{
		if (transpCalcPattern->isChecked())
		{
			if (usePatternInverted->isChecked())
				emit NewGradient(GradMask_PatternLumAlphaInverted);
			else
				emit NewGradient(GradMask_PatternLumAlpha);
		}
		else
		{
			if (usePatternInverted->isChecked())
				emit NewGradient(GradMask_PatternInverted);
			else
				emit NewGradient(GradMask_Pattern);
		}
	}
	else
		emit NewGradient(GradMask_None);
}

void TransparencyPalette::slotGradType(int type)
{
	if (type == 0)
	{
		if (transpCalcGradient->isChecked())
			emit NewGradient(GradMask_LinearLumAlpha);
		else
			emit NewGradient(GradMask_Linear);
	}
	else
	{
		if (transpCalcGradient->isChecked())
			emit NewGradient(GradMask_RadialLumAlpha);
		else
			emit NewGradient(GradMask_Radial);
	}
}

void TransparencyPalette::setNamedGradient(const QString &name)
{
	if (namedGradient->currentIndex() == 0)
	{
		gradEdit->setGradient(currentItem->mask_gradient);
		currentItem->setGradientMask("");
		gradEdit->setGradientEditable(true);
	}
	else
	{
		gradEdit->setGradient(gradientList->value(name));
		gradEdit->setGradientEditable(false);
		currentItem->setGradientMask(name);
	}
	if (gradientType->currentIndex() == 0)
	{
		if (transpCalcGradient->isChecked())
			emit NewGradient(GradMask_LinearLumAlpha);
		else
			emit NewGradient(GradMask_Linear);
	}
	else
	{
		if (transpCalcGradient->isChecked())
			emit NewGradient(GradMask_RadialLumAlpha);
		else
			emit NewGradient(GradMask_Radial);
	}
}

void TransparencyPalette::switchGradientMode()
{
	if (gradientType->currentIndex() == 0)
	{
		if (transpCalcGradient->isChecked())
			emit NewGradient(GradMask_LinearLumAlpha);
		else
			emit NewGradient(GradMask_Linear);
	}
	else
	{
		if (transpCalcGradient->isChecked())
			emit NewGradient(GradMask_RadialLumAlpha);
		else
			emit NewGradient(GradMask_Radial);
	}
}

void TransparencyPalette::switchPatternMode()
{
	if (transpCalcPattern->isChecked())
	{
		if (usePatternInverted->isChecked())
			emit NewGradient(GradMask_PatternLumAlphaInverted);
		else
			emit NewGradient(GradMask_PatternLumAlpha);
	}
	else
	{
		if (usePatternInverted->isChecked())
			emit NewGradient(GradMask_PatternInverted);
		else
			emit NewGradient(GradMask_Pattern);
	}
}

void TransparencyPalette::editGradientVector()
{
	if (gradEditButton->isChecked())
	{
		TGradDia->unitChange(currentDoc->unitIndex());
		TGradDia->setValues(currentItem->GrMaskStartX, currentItem->GrMaskStartY, currentItem->GrMaskEndX, currentItem->GrMaskEndY, currentItem->GrMaskFocalX, currentItem->GrMaskFocalY, currentItem->GrMaskScale, currentItem->GrMaskSkew, 0, 0);
		if ((currentItem->GrMask == GradMask_Linear) || (currentItem->GrMask == GradMask_LinearLumAlpha))
			TGradDia->selectLinear();
		else
			TGradDia->selectRadial();
		TGradDia->show();
	}
	else
	{
		TGradDia->hide();
	}
	emit editGradient();
}

void TransparencyPalette::setActiveGradDia(bool active)
{
	if (!active)
	{
		gradEditButton->setChecked(false);
		emit editGradient();
	}
}

void TransparencyPalette::setSpecialGradient(double x1, double y1, double x2, double y2, double fx, double fy, double sg, double sk)
{
	if (TGradDia)
		TGradDia->setValues(x1, y1, x2, y2, fx, fy, sg, sk, 0, 0);
}

void TransparencyPalette::hideEditedPatterns(QStringList names)
{
	for (int a = 0; a < names.count(); a++)
	{
		QList<QListWidgetItem*> items = patternBox->findItems(names[a], Qt::MatchExactly);
		if (items.count() > 0)
			items[0]->setHidden(true);
	}
}

void TransparencyPalette::updatePatternList()
{
	disconnect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	patternBox->clear();
	patternBox->setIconSize(QSize(48, 48));
	QStringList patK = patternList->keys();
	patK.sort();
	for (int a = 0; a < patK.count(); a++)
	{
		ScPattern sp = patternList->value(patK[a]);
		QPixmap pm;
		if (sp.getPattern()->width() >= sp.getPattern()->height())
			pm = QPixmap::fromImage(sp.getPattern()->scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm = QPixmap::fromImage(sp.getPattern()->scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(pm2, patK[a], patternBox);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	patternBox->clearSelection();
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
}

void TransparencyPalette::setPatterns(QHash<QString, ScPattern> *docPatterns)
{
	patternList = docPatterns;
	updatePatternList();
}

void TransparencyPalette::selectPattern(QListWidgetItem *c)
{
	if (c == nullptr)
		return;
	emit NewPattern(c->text());
}

void TransparencyPalette::setCurrentMaskPattern(const QString& pattern, const ScMaskTransform& maskTrans, bool mirrorX, bool mirrorY)
{
	disconnect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	QList<QListWidgetItem*> itl = patternBox->findItems(pattern, Qt::MatchExactly);
	if (itl.count() != 0)
	{
		QListWidgetItem *it = itl[0];
		patternBox->setCurrentItem(it);
	}
	else
		patternBox->clearSelection();
	m_maskTransform = maskTrans;
	m_Pattern_mirrorX = mirrorX;
	m_Pattern_mirrorY = mirrorY;
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
}

void TransparencyPalette::changePatternProps()
{
	PatternPropsDialog *dia = new PatternPropsDialog(this, currentUnit, false);
	dia->spinXscaling->setValue(m_maskTransform.scaleX * 100.0);
	dia->spinYscaling->setValue(m_maskTransform.scaleY * 100.0);
	if (m_maskTransform.scaleX == m_maskTransform.scaleY)
		dia->keepScaleRatio->setChecked(true);
	dia->spinXoffset->setValue(m_maskTransform.offsetX);
	dia->spinYoffset->setValue(m_maskTransform.offsetY);
	dia->spinAngle->setValue(m_maskTransform.rotation);
	double asina = atan(m_maskTransform.skewX);
	dia->spinXSkew->setValue(asina / (M_PI / 180.0));
	double asinb = atan(m_maskTransform.skewY);
	dia->spinYSkew->setValue(asinb / (M_PI / 180.0));
	dia->FlipH->setChecked(m_Pattern_mirrorX);
	dia->FlipV->setChecked(m_Pattern_mirrorY);
	connect(dia, SIGNAL(NewPatternProps(double,double,double,double,double,double,double,bool,bool)), this, SIGNAL(NewPatternProps(double,double,double,double,double,double,double,bool,bool)));
	dia->exec();
	m_maskTransform.scaleX = dia->spinXscaling->value() / 100.0;
	m_maskTransform.scaleY = dia->spinYscaling->value() / 100.0;
	m_maskTransform.offsetX = dia->spinXoffset->value();
	m_maskTransform.offsetY = dia->spinYoffset->value();
	m_maskTransform.rotation = dia->spinAngle->value();
	double a    = M_PI / 180.0 * dia->spinXSkew->value();
	double b    = M_PI / 180.0 * dia->spinYSkew->value();
	double sina = tan(a);
	double sinb = tan(b);
	m_maskTransform.skewX = sina;
	m_maskTransform.skewY = sinb;
	m_Pattern_mirrorX = dia->FlipH->isChecked();
	m_Pattern_mirrorY = dia->FlipV->isChecked();
	delete dia;
}

void TransparencyPalette::setActTrans(double val, double val2)
{
	disconnect(strokeOpacity, SIGNAL(valueChanged(double)), this, SLOT(slotTransS(double)));
	disconnect(fillOpacity, SIGNAL(valueChanged(double)), this, SLOT(slotTransF(double)));
	strokeOpacity->setValue(qRound(100 - (val2 * 100)));
	fillOpacity->setValue(qRound(100 - (val * 100)));
	connect(strokeOpacity, SIGNAL(valueChanged(double)), this, SLOT(slotTransS(double)));
	connect(fillOpacity, SIGNAL(valueChanged(double)), this, SLOT(slotTransF(double)));
}

void TransparencyPalette::setActBlend(int val, int val2)
{
	disconnect(blendModeFill, SIGNAL(activated(int)), this, SIGNAL(NewBlend(int)));
	disconnect(blendModeStroke, SIGNAL(activated(int)), this, SIGNAL(NewBlendS(int)));
	blendModeFill->setCurrentIndex(val);
	blendModeStroke->setCurrentIndex(val2);
	connect(blendModeFill, SIGNAL(activated(int)), this, SIGNAL(NewBlend(int)));
	connect(blendModeStroke, SIGNAL(activated(int)), this, SIGNAL(NewBlendS(int)));
}

void TransparencyPalette::slotTransS(double val)
{
	emit NewTransS((100 - val) / 100.0);
}

void TransparencyPalette::slotTransF(double val)
{
	emit NewTrans((100 - val) / 100.0);
}

void TransparencyPalette::editLineSelectorButton()
{
	if (editLineSelector->isChecked())
	{
		stackedWidget->setCurrentIndex(0);
		editFillSelector->setChecked(false);
	}
	updateFromItem();
}

void TransparencyPalette::editFillSelectorButton()
{
	if (editFillSelector->isChecked())
	{
		stackedWidget->setCurrentIndex(1);
		editLineSelector->setChecked(false);
	}
	updateFromItem();
}

void TransparencyPalette::unitChange(double, double, int unitIndex)
{
	if (TGradDia)
		TGradDia->unitChange(unitIndex);
	currentUnit = unitIndex;
}

