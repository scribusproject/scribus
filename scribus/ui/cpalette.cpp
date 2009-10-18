/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          cpalette.cpp  -  description
                             -------------------
    begin                : Wed Apr 25 2001
    copyright            : (C) 2001 by Franz Schmid
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

#include "cpalette.h"

#include <QAbstractItemView>
#include <QEvent>
#include <QFont>
#include <QFrame>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPixmap>
#include <QRect>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStandardItem>
#include <QToolButton>
#include <QToolTip>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QDebug>

#include "colorlistbox.h"
#include "sccombobox.h"
#include "scribusdoc.h"
#include "scrspinbox.h"
#include "gradienteditor.h"
#include "units.h"
#include "page.h"
#include "pageitem.h"
#include "util_icon.h"
#include "commonstrings.h"
#include "linkbutton.h"
#include "sccolorengine.h"
#include "scpainter.h"
#include "scpattern.h"
#include "util.h"

GradientVectorDialog::GradientVectorDialog(QWidget* parent) : ScrPaletteBase( parent, "GradientVectorPalette", false, 0 )
{
	freeGradientLayout = new QGridLayout(this);
	freeGradientLayout->setMargin(5);
	freeGradientLayout->setSpacing(5);
	GTextX1 = new QLabel("X1:", this );
	freeGradientLayout->addWidget( GTextX1, 0, 0 );
	GTextY1 = new QLabel("Y1:", this );
	freeGradientLayout->addWidget( GTextY1, 1, 0 );
	gX1 = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gX1, 0, 1 );
	gY1 = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gY1, 1, 1 );
	GTextX2 = new QLabel("X2:", this );
	freeGradientLayout->addWidget( GTextX2, 0, 2 );
	GTextY2 = new QLabel("Y2:", this );
	freeGradientLayout->addWidget( GTextY2, 1, 2 );
	gX2 = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gX2, 0, 3 );
	gY2 = new ScrSpinBox( -3000, 3000, this, 0);
	freeGradientLayout->addWidget( gY2, 1, 3 );
	connect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	languageChange();
}

void GradientVectorDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void GradientVectorDialog::languageChange()
{
	setWindowTitle( tr( "Gradient Vector" ));
	resize(minimumSizeHint());
}

void GradientVectorDialog::setValues(double x1, double y1, double x2, double y2)
{
	disconnect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	gX1->setValue(x1);
	gX2->setValue(x2);
	gY1->setValue(y1);
	gY2->setValue(y2);
	connect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
}

void GradientVectorDialog::changeSpecial()
{
	emit NewSpecial(gX1->value(), gY1->value(), gX2->value(), gY2->value());
}

void GradientVectorDialog::unitChange(int unitIndex)
{
	disconnect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	disconnect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	gX1->setNewUnit(unitIndex);
	gY1->setNewUnit(unitIndex);
	gX2->setNewUnit(unitIndex);
	gY2->setNewUnit(unitIndex);
	connect(gX1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gX2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY1, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
	connect(gY2, SIGNAL(valueChanged(double)), this, SLOT(changeSpecial()));
}

PatternPropsDialog::PatternPropsDialog(QWidget* parent, int unitIndex) : QDialog( parent )
{
	setModal(true);
	frame3Layout = new QVBoxLayout( this );
	frame3Layout->setMargin(0);
	frame3Layout->setSpacing(2);

	groupOffset = new QGroupBox( this );
	groupOffsetLayout = new QHBoxLayout( groupOffset );
	groupOffsetLayout->setSpacing( 2 );
	groupOffsetLayout->setMargin( 3 );
	groupOffsetLayout->setAlignment( Qt::AlignTop );
	textLabel1 = new QLabel( groupOffset );
	groupOffsetLayout->addWidget( textLabel1 );
	spinXoffset = new ScrSpinBox( -3000, 3000, groupOffset, 0);
	groupOffsetLayout->addWidget( spinXoffset );
	textLabel2 = new QLabel( groupOffset );
	groupOffsetLayout->addWidget( textLabel2 );
	spinYoffset = new ScrSpinBox( -3000, 3000, groupOffset, 0);
	groupOffsetLayout->addWidget( spinYoffset );
	frame3Layout->addWidget( groupOffset );

	groupScale = new QGroupBox( this );
	groupScaleLayout = new QGridLayout( groupScale );
	groupScaleLayout->setSpacing( 2 );
	groupScaleLayout->setMargin( 3 );
	groupScaleLayout->setAlignment( Qt::AlignTop );
	textLabel5 = new QLabel( groupScale );
	groupScaleLayout->addWidget( textLabel5, 0, 0 );
	spinXscaling = new ScrSpinBox( 1, 500, groupScale, 0);
	spinXscaling->setValue( 100 );
	groupScaleLayout->addWidget( spinXscaling, 0, 1 );
	textLabel6 = new QLabel( groupScale );
	groupScaleLayout->addWidget( textLabel6, 1, 0 );
	spinYscaling = new ScrSpinBox( 1, 500, groupScale, 0 );
	groupScaleLayout->addWidget( spinYscaling, 1, 1 );
	keepScaleRatio = new LinkButton( groupScale );
	keepScaleRatio->setCheckable( true );
	keepScaleRatio->setAutoRaise( true );
	keepScaleRatio->setMaximumSize( QSize( 15, 32767 ) );
	groupScaleLayout->addWidget( keepScaleRatio, 0, 2, 2, 1 );
	frame3Layout->addWidget( groupScale );
	groupRotation = new QGroupBox( this );
	groupRotationLayout = new QHBoxLayout( groupRotation );
	groupRotationLayout->setSpacing( 2 );
	groupRotationLayout->setMargin( 3 );
	groupRotationLayout->setAlignment( Qt::AlignTop );
	textLabel7 = new QLabel( groupRotation );
	groupRotationLayout->addWidget( textLabel7 );
	spinAngle = new ScrSpinBox( -180, 180, groupRotation, 6 );
	groupRotationLayout->addWidget( spinAngle );
	frame3Layout->addWidget( groupRotation );
	buttonLayout = new QHBoxLayout;
	buttonLayout->setMargin(0);
	buttonLayout->setSpacing(5);
	QSpacerItem* hspacing = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
	buttonLayout->addItem(hspacing);
	buttonOk = new QPushButton( this );
	buttonOk->setAutoDefault( TRUE );
	buttonOk->setDefault( TRUE );
	buttonLayout->addWidget( buttonOk );
	frame3Layout->addLayout( buttonLayout );
	spinXoffset->setNewUnit(unitIndex);
	spinYoffset->setNewUnit(unitIndex);
	languageChange();
	connect(spinXoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinYoffset, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	connect(keepScaleRatio, SIGNAL(clicked()), this, SLOT(ToggleKette()));
	connect(spinAngle, SIGNAL(valueChanged(double)), this, SLOT(changePatternProps()));
	connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));
}

void PatternPropsDialog::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

void PatternPropsDialog::languageChange()
{
	setWindowTitle( tr( "Pattern Properties" ));
	QString ptSuffix=tr(" pt");
	QString pctSuffix=tr(" %");
	groupOffset->setTitle( tr( "Offsets" ) );
	textLabel1->setText( tr( "X:" ) );
	spinXoffset->setSuffix( ptSuffix );
	textLabel2->setText( tr( "Y:" ) );
	spinYoffset->setSuffix( ptSuffix );
	groupScale->setTitle( tr( "Scaling" ) );
	textLabel5->setText( tr( "X-Scale:" ) );
	spinXscaling->setSuffix( pctSuffix );
	textLabel6->setText( tr( "Y-Scale:" ) );
	spinYscaling->setSuffix( pctSuffix );
	groupRotation->setTitle( tr( "Rotation" ) );
	textLabel7->setText( tr( "Angle" ) );
	buttonOk->setText( tr("Close"));
	resize(minimumSizeHint());
}

void PatternPropsDialog::changePatternProps()
{
	emit NewPatternProps(spinXscaling->value(), spinYscaling->value(), spinXoffset->value(), spinYoffset->value(), spinAngle->value());
}

void PatternPropsDialog::ToggleKette()
{
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepScaleRatio->isChecked())
	{
		spinYscaling->setValue(spinXscaling->value());
		changePatternProps();
		keepScaleRatio->setChecked(true);
	}
	else
		keepScaleRatio->setChecked(false);
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void PatternPropsDialog::HChange()
{
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepScaleRatio->isChecked())
		spinYscaling->setValue(spinXscaling->value());
	changePatternProps();
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

void PatternPropsDialog::VChange()
{
	disconnect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	disconnect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
	if (keepScaleRatio->isChecked())
		spinXscaling->setValue(spinYscaling->value());
	changePatternProps();
	connect(spinXscaling, SIGNAL(valueChanged(double)), this, SLOT(HChange()));
	connect(spinYscaling, SIGNAL(valueChanged(double)), this, SLOT(VChange()));
}

Cpalette::Cpalette(QWidget* parent) : QWidget(parent)
{
	currentItem = NULL;
	patternList = NULL;
	CGradDia = NULL;
	CGradDia = new GradientVectorDialog(this->parentWidget());
	CGradDia->hide();
	setupUi(this);
	editLineColorSelector->setIcon(QIcon(loadIcon("16/color-stroke.png")));
	editFillColorSelector->setIcon(QIcon(loadIcon("16/color-fill.png")));
	connect(editLineColorSelector, SIGNAL(clicked()), this, SLOT(editLineColorSelectorButton()));
	connect(editFillColorSelector, SIGNAL(clicked()), this, SLOT(editFillColorSelectorButton()));
	connect(strokeOpacity, SIGNAL(valueChanged(int)), this, SLOT(slotTransS(int)));
	connect(fillOpacity, SIGNAL(valueChanged(int)), this, SLOT(slotTransF(int)));
	connect(blendModeFill, SIGNAL(activated(int)), this, SIGNAL(NewBlend(int)));
	connect(blendModeStroke, SIGNAL(activated(int)), this, SIGNAL(NewBlendS(int)));
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	connect(fillShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewBrushShade(int)));
	connect(strokeShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewPenShade(int)));
	connect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	connect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
	connect(gradEditButton, SIGNAL(clicked()), this, SLOT(editGradientVector()));
	connect(displayAllColors, SIGNAL(clicked()), this, SLOT(ToggleColorDisplay()));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotGrad(int)));
	connect(CGradDia, SIGNAL(NewSpecial(double, double, double, double)), this, SIGNAL(NewSpecial(double, double, double, double)));
	connect(CGradDia, SIGNAL(paletteShown(bool)), this, SLOT(setActiveGradDia(bool)));
	connect(gradientType, SIGNAL(activated(int)), this, SLOT(slotGradType(int)));
	connect(gradEdit, SIGNAL(gradientChanged()), this, SIGNAL(gradientChanged()));
	connect(editPatternProps, SIGNAL(clicked()), this, SLOT(changePatternProps()));
	connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	connect(overPrintCombo, SIGNAL(activated(int)), this, SIGNAL(NewOverprint(int)));
	connect(patternBoxStroke, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPatternS(QListWidgetItem*)));
	connect(tabWidgetStroke, SIGNAL(currentChanged(int)), this, SLOT(slotGradStroke(int)));
	connect(editPatternPropsStroke, SIGNAL(clicked()), this, SLOT(changePatternPropsStroke()));
	editFillColorSelector->setChecked(true);
	editFillColorSelectorButton();
	tabWidgetStroke->setTabEnabled(1, false);
}

void Cpalette::setCurrentItem(PageItem* item)
{
	currentItem = item;
}

void Cpalette::setDocument(ScribusDoc* doc)
{
	currentDoc = doc;
	if (doc == NULL)
	{
		colorListStroke->cList = NULL;
		colorListFill->cList = NULL;
	}
	else
	{
		colorListStroke->cList = &doc->PageColors;
		colorListFill->cList = &doc->PageColors;
		gradEdit->setColors(doc->PageColors);
		currentUnit = doc->unitIndex();
	}
}

void Cpalette::updateFromItem()
{
	if (currentItem == NULL)
		return;
	if (!currentDoc)
		return;
	updateCList();
	setActTrans(currentItem->fillTransparency(), currentItem->lineTransparency());
	setActBlend(currentItem->fillBlendmode(), currentItem->lineBlendmode());
	if (currentItem->doOverprint)
		setActOverprint(1);
	else
		setActOverprint(0);
	setActFarben(currentItem->lineColor(), currentItem->fillColor(), currentItem->lineShade(), currentItem->fillShade());
	ChooseGrad(currentItem->GrType);
	gradEdit->setGradient(currentItem->fill_gradient);
	disconnect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	if (!currentItem->gradient().isEmpty())
	{
		setCurrentComboItem(namedGradient, currentItem->gradient());
		gradEdit->setGradientEditable(false);
	}
	else
	{
		namedGradient->setCurrentIndex(0);
		gradEdit->setGradientEditable(true);
	}
	if (patternList->count() == 0)
	{
		tabWidgetStroke->setTabEnabled(2, false);
		tabWidget->setTabEnabled(2, false);
	}
	else
	{
		tabWidgetStroke->setTabEnabled(2, true);
		tabWidget->setTabEnabled(2, true);
	}
	if (currentItem->strokePattern().isEmpty())
		tabWidgetStroke->setCurrentIndex(0);
	else
		tabWidgetStroke->setCurrentIndex(2);
	connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
}

void Cpalette::updateCList()
{
	disconnect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	disconnect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
	colorListStroke->clear();
	colorListStroke->addItem(CommonStrings::tr_NoneColor);
	colorListFill->clear();
	colorListFill->addItem(CommonStrings::tr_NoneColor);
	if (displayAllColors->isChecked())
	{
		if (currentDoc != NULL)
			currentDoc->getUsedColors(colorList);
	}
	gradEditStroke->setColors(colorList);
	gradEdit->setColors(colorList);
	colorListFill->insertItems(colorList, ColorListBox::fancyPixmap);
	colorListStroke->insertItems(colorList, ColorListBox::fancyPixmap);
	if (colorListFill->currentItem())
		colorListFill->currentItem()->setSelected(false);
	if (colorListStroke->currentItem())
		colorListStroke->currentItem()->setSelected(false);
	connect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	connect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
}

void Cpalette::ToggleColorDisplay()
{
	if (currentDoc != NULL)
	{
		colorListStroke->cList = &currentDoc->PageColors;
		colorListFill->cList = &currentDoc->PageColors;
		colorList = currentDoc->PageColors;
		updateFromItem();
	}
}

void Cpalette::setActTrans(double val, double val2)
{
	disconnect(strokeOpacity, SIGNAL(valueChanged(int)), this, SLOT(slotTransS(int)));
	disconnect(fillOpacity, SIGNAL(valueChanged(int)), this, SLOT(slotTransF(int)));
	strokeOpacity->setValue(qRound(100 - (val2 * 100)));
	fillOpacity->setValue(qRound(100 - (val * 100)));
	connect(strokeOpacity, SIGNAL(valueChanged(int)), this, SLOT(slotTransS(int)));
	connect(fillOpacity, SIGNAL(valueChanged(int)), this, SLOT(slotTransF(int)));
}

void Cpalette::setActBlend(int val, int val2)
{
	disconnect(blendModeFill, SIGNAL(activated(int)), this, SIGNAL(NewBlend(int)));
	disconnect(blendModeStroke, SIGNAL(activated(int)), this, SIGNAL(NewBlendS(int)));
	blendModeFill->setCurrentIndex(val);
	blendModeStroke->setCurrentIndex(val2);
	connect(blendModeFill, SIGNAL(activated(int)), this, SIGNAL(NewBlend(int)));
	connect(blendModeStroke, SIGNAL(activated(int)), this, SIGNAL(NewBlendS(int)));
}

void Cpalette::setActOverprint(int val)
{
	disconnect(overPrintCombo, SIGNAL(activated(int)), this, SIGNAL(NewOverprint(int)));
	overPrintCombo->setCurrentIndex(val);
	connect(overPrintCombo, SIGNAL(activated(int)), this, SIGNAL(NewOverprint(int)));
}

void Cpalette::setActFarben(QString p, QString b, int shp, int shb)
{
	disconnect(fillShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewBrushShade(int)));
	disconnect(strokeShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewPenShade(int)));
	disconnect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	disconnect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
	strokeShade->setValue(shp);
	fillShade->setValue(shp);
	if ((p != CommonStrings::None) && (!p.isEmpty()))
	{
		QList<QListWidgetItem *> cCol = colorListStroke->findItems(p, Qt::MatchExactly);
		if (cCol.count() != 0)
			colorListStroke->setCurrentItem(cCol[0]);
	}
	else
		colorListStroke->setCurrentRow(0);
	if ((b != CommonStrings::None) && (!b.isEmpty()))
	{
		QList<QListWidgetItem *> cCol = colorListFill->findItems(b, Qt::MatchExactly);
		if (cCol.count() != 0)
			colorListFill->setCurrentItem(cCol[0]);
	}
	else
		colorListFill->setCurrentRow(0);
	connect(fillShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewBrushShade(int)));
	connect(strokeShade, SIGNAL(valueChanged(int)), this, SIGNAL(NewPenShade(int)));
	connect(colorListStroke, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorS(QListWidgetItem*)));
	connect(colorListFill, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(selectColorF(QListWidgetItem*)));
}

void Cpalette::selectColorS(QListWidgetItem *item)
{
	ColorPixmapItem* c = dynamic_cast<ColorPixmapItem*>(item);
	if (c != NULL)	
		sFarbe = c->colorName();
	else if (! item->data(Qt::DisplayRole).toString().isEmpty()) 
		sFarbe = item->data(Qt::DisplayRole).toString();
	else
		return;
	emit NewPen(sFarbe);
}

void Cpalette::selectColorF(QListWidgetItem *item)
{
	ColorPixmapItem* c = dynamic_cast<ColorPixmapItem*>(item);
	if (c != NULL)	
		sFarbe = c->colorName();
	else if (! item->data(Qt::DisplayRole).toString().isEmpty()) 
		sFarbe = item->data(Qt::DisplayRole).toString();
	else
		return;
	emit NewBrush(sFarbe);
}

void Cpalette::slotTransS(int val)
{
	emit NewTransS(static_cast<double>(100 - val) / 100.0);
}

void Cpalette::slotTransF(int val)
{
	emit NewTrans(static_cast<double>(100 - val) / 100.0);
}

void Cpalette::SetColors(ColorList newColorList)
{
	colorList.clear();
	colorList = newColorList;
	updateCList();
}

void Cpalette::editLineColorSelectorButton()
{
	if (editLineColorSelector->isChecked())
	{
		stackedWidget->setCurrentIndex(0);
		editFillColorSelector->setChecked(false);
	}
	updateFromItem();
}

void Cpalette::editFillColorSelectorButton()
{
	if (editFillColorSelector->isChecked())
	{
		stackedWidget->setCurrentIndex(1);
		editLineColorSelector->setChecked(false);
	}
	updateFromItem();
}

void Cpalette::updateGradientList()
{
	disconnect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	namedGradient->clear();
	namedGradient->setIconSize(QSize(48, 12));
	namedGradient->addItem( tr("Custom"));
	namedGradientStroke->clear();
	namedGradientStroke->setIconSize(QSize(48, 12));
	namedGradientStroke->addItem( tr("Custom"));
	for (QMap<QString, VGradient>::Iterator it = gradientList->begin(); it != gradientList->end(); ++it)
	{
		QImage pixm(48, 12, QImage::Format_ARGB32);
		QPainter pb;
		QBrush b(QColor(205,205,205), loadIcon("testfill.png"));
		pb.begin(&pixm);
		pb.fillRect(0, 0, 48, 12, b);
		pb.end();
		ScPainter *p = new ScPainter(&pixm, 48, 12);
		p->setPen(Qt::black);
		p->setLineWidth(1);
		p->setFillMode(2);
		p->fill_gradient = it.value();
		p->setGradient(VGradient::linear, FPoint(0,6), FPoint(48, 6));
		p->drawRect(0, 0, 48, 12);
		p->end();
		delete p;
		QPixmap pm;
		pm = QPixmap::fromImage(pixm);
		namedGradient->addItem(pm, it.key());
		namedGradientStroke->addItem(pm, it.key());
	}
	connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
}

void Cpalette::SetGradients(QMap<QString, VGradient> *docGradients)
{
	gradientList = docGradients;
	updateGradientList();
}

void Cpalette::setNamedGradient(const QString &name)
{
	if (namedGradient->currentIndex() == 0)
	{
		gradEdit->setGradient(currentItem->fill_gradient);
		currentItem->setGradient("");
		gradEdit->setGradientEditable(true);
	}
	else
	{
		gradEdit->setGradient(gradientList->value(name));
		gradEdit->setGradientEditable(false);
		currentItem->setGradient(name);
	}
}

void Cpalette::updatePatternList()
{
	disconnect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
	patternBox->clear();
	patternBox->setIconSize(QSize(48, 48));
	patternBoxStroke->clear();
	patternBoxStroke->setIconSize(QSize(48, 48));
	for (QMap<QString, ScPattern>::Iterator it = patternList->begin(); it != patternList->end(); ++it)
	{
		QPixmap pm;
		if (it.value().getPattern()->width() >= it.value().getPattern()->height())
			pm=QPixmap::fromImage(it.value().getPattern()->scaledToWidth(48, Qt::SmoothTransformation));
		else
			pm=QPixmap::fromImage(it.value().getPattern()->scaledToHeight(48, Qt::SmoothTransformation));
		QPixmap pm2(48, 48);
		pm2.fill(palette().color(QPalette::Base));
		QPainter p;
		p.begin(&pm2);
		p.drawPixmap(24 - pm.width() / 2, 24 - pm.height() / 2, pm);
		p.end();
		QListWidgetItem *item = new QListWidgetItem(pm2, it.key(), patternBox);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		QListWidgetItem *itemS = new QListWidgetItem(pm2, it.key(), patternBoxStroke);
		itemS->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	}
	patternBox->clearSelection();
	patternBoxStroke->clearSelection();
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
}

void Cpalette::SetPatterns(QMap<QString, ScPattern> *docPatterns)
{
	patternList = docPatterns;
	updatePatternList();
}

void Cpalette::selectPattern(QListWidgetItem *c)
{
	if (c == NULL)
		return;
	emit NewPattern(c->text());
}

void Cpalette::selectPatternS(QListWidgetItem *c)
{
	if (c == NULL)
		return;
	emit NewPatternS(c->text());
}

void Cpalette::setActPatternStroke(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation)
{
	disconnect(patternBoxStroke, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPatternS(QListWidgetItem*)));
	QList<QListWidgetItem*> itl = patternBoxStroke->findItems(pattern, Qt::MatchExactly);
	if (itl.count() != 0)
	{
		QListWidgetItem *it = itl[0];
		patternBoxStroke->setCurrentItem(it);
	}
	else
		patternBoxStroke->clearSelection();
	m_Pattern_scaleXS = scaleX;
	m_Pattern_scaleYS = scaleX;
	m_Pattern_offsetXS = offsetX;
	m_Pattern_offsetYS = offsetY;
	m_Pattern_rotationS = rotation;
	connect(patternBoxStroke, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPatternS(QListWidgetItem*)));
}

void Cpalette::setActPattern(QString pattern, double scaleX, double scaleY, double offsetX, double offsetY, double rotation)
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
	m_Pattern_scaleX = scaleX;
	m_Pattern_scaleY = scaleX;
	m_Pattern_offsetX = offsetX;
	m_Pattern_offsetY = offsetY;
	m_Pattern_rotation = rotation;
	connect(patternBox, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(selectPattern(QListWidgetItem*)));
}

void Cpalette::slotGradStroke(int number)
{
	if (number == 1)
	{
//		disconnect(namedGradientStroke, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
		if (!currentItem->strokeGradient().isEmpty())
		{
			setCurrentComboItem(namedGradientStroke, currentItem->strokeGradient());
			gradEditStroke->setGradient(gradientList->value(currentItem->strokeGradient()));
			gradEditStroke->setGradientEditable(false);
		}
		else
		{
			namedGradientStroke->setCurrentIndex(0);
			gradEditStroke->setGradient(currentItem->stroke_gradient);
			gradEditStroke->setGradientEditable(true);
		}
//		if (gradientType->currentIndex() == 0)
//			emit NewGradient(6);
//		else
//			currentItem->GrType = 7;
//		connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	}
//	else if (number == 2)
//		emit NewGradient(8);
	else
		emit NewPatternS("");
}

void Cpalette::ChooseGrad(int number)
{
	disconnect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotGrad(int)));
	if (number==-1)
	{
		tabWidget->setCurrentIndex(0);
	}
	if (number == 0)
		tabWidget->setCurrentIndex(0);
	else if ((number > 0) && (number < 8))
	{
		if ((number == 5) || (number == 7))
			gradientType->setCurrentIndex(1);
		else
			gradientType->setCurrentIndex(0);
		tabWidget->setCurrentIndex(1);
	}
	else
	{
		if (patternList->count() == 0)
		{
			tabWidget->setCurrentIndex(0);
			emit NewGradient(0);
		}
		else
			tabWidget->setCurrentIndex(2);
	}
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(slotGrad(int)));
}

void Cpalette::slotGrad(int number)
{
	if (number == 1)
	{
		disconnect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
		if (!currentItem->gradient().isEmpty())
		{
			setCurrentComboItem(namedGradient, currentItem->gradient());
			gradEdit->setGradient(gradientList->value(currentItem->gradient()));
			gradEdit->setGradientEditable(false);
		}
		else
		{
			namedGradient->setCurrentIndex(0);
			gradEdit->setGradient(currentItem->fill_gradient);
			gradEdit->setGradientEditable(true);
		}
		if (gradientType->currentIndex() == 0)
			emit NewGradient(6);
		else
			emit NewGradient(7);
		connect(namedGradient, SIGNAL(activated(const QString &)), this, SLOT(setNamedGradient(const QString &)));
	}
	else if (number == 2)
		emit NewGradient(8);
	else
		emit NewGradient(0);
}

void Cpalette::slotGradType(int type)
{
	if (type == 0)
		emit NewGradient(6);
	else
		emit NewGradient(7);
}

void Cpalette::setActGradient(int typ)
{
	ChooseGrad(typ);
}

void Cpalette::editGradientVector()
{
	if (gradEditButton->isChecked())
	{
		CGradDia->unitChange(currentDoc->unitIndex());
		CGradDia->setValues(currentItem->GrStartX, currentItem->GrStartY, currentItem->GrEndX, currentItem->GrEndY);
		CGradDia->show();
	}
	else
	{
		CGradDia->hide();
	}
	emit editGradient();
}

void Cpalette::setActiveGradDia(bool active)
{
	if (!active)
	{
		gradEditButton->setChecked(false);
		emit editGradient();
	}
}

void Cpalette::setSpecialGradient(double x1, double y1, double x2, double y2)
{
	if (CGradDia)
		CGradDia->setValues(x1, y1, x2, y2);
}

void Cpalette::changePatternProps()
{
	PatternPropsDialog *dia = new PatternPropsDialog(this, currentUnit);
	dia->spinXscaling->setValue(m_Pattern_scaleX);
	dia->spinYscaling->setValue(m_Pattern_scaleY);
	dia->spinXoffset->setValue(m_Pattern_offsetX);
	dia->spinYoffset->setValue(m_Pattern_offsetY);
	dia->spinAngle->setValue(m_Pattern_rotation);
	connect(dia, SIGNAL(NewPatternProps(double, double, double, double, double)), this, SIGNAL(NewPatternProps(double, double, double, double, double)));
	dia->exec();
	m_Pattern_scaleX = dia->spinXscaling->value();
	m_Pattern_scaleY = dia->spinYscaling->value();
	m_Pattern_offsetX = dia->spinXoffset->value();
	m_Pattern_offsetY = dia->spinYoffset->value();
	m_Pattern_rotation = dia->spinAngle->value();
	delete dia;
	tabWidget->setCurrentIndex(2);
	emit NewGradient(8);
}

void Cpalette::changePatternPropsStroke()
{
	PatternPropsDialog *dia = new PatternPropsDialog(this, currentUnit);
	dia->spinXscaling->setValue(m_Pattern_scaleXS);
	dia->spinYscaling->setValue(m_Pattern_scaleYS);
	dia->spinXoffset->setValue(m_Pattern_offsetXS);
	dia->spinYoffset->setValue(m_Pattern_offsetYS);
	dia->spinAngle->setValue(m_Pattern_rotationS);
	connect(dia, SIGNAL(NewPatternProps(double, double, double, double, double)), this, SIGNAL(NewPatternPropsS(double, double, double, double, double)));
	dia->exec();
	m_Pattern_scaleXS = dia->spinXscaling->value();
	m_Pattern_scaleYS = dia->spinYscaling->value();
	m_Pattern_offsetXS = dia->spinXoffset->value();
	m_Pattern_offsetYS = dia->spinYoffset->value();
	m_Pattern_rotationS = dia->spinAngle->value();
	delete dia;
}

void Cpalette::unitChange(double, double, int unitIndex)
{
	if (CGradDia)
		CGradDia->unitChange(unitIndex);
	currentUnit = unitIndex;
}

void Cpalette::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
	}
	else
		QWidget::changeEvent(e);
}

/*
void Cpalette::languageChange()
{
	QString ptSuffix=tr(" pt");
	QString pctSuffix=tr(" %");
	PM1->setSuffix(pctSuffix);
	TransSpin->setSuffix(pctSuffix);
	groupOffset->setTitle( tr( "Offsets" ) );
	textLabel1->setText( tr( "X:" ) );
	spinXoffset->setSuffix( ptSuffix );
	textLabel2->setText( tr( "Y:" ) );
	spinYoffset->setSuffix( ptSuffix );
	groupScale->setTitle( tr( "Scaling" ) );
	textLabel5->setText( tr( "X-Scale:" ) );
	spinXscaling->setSuffix( pctSuffix );
	textLabel6->setText( tr( "Y-Scale:" ) );
	spinYscaling->setSuffix( pctSuffix );
	groupRotation->setTitle( tr( "Rotation" ) );
	textLabel7->setText( tr( "Angle" ) );

	ShadeTxt->setText( tr( "Shade:" ) );
	TransTxt->setText( tr( "Opacity:" ) );
	gradEditButton->setText( tr("Move Vector"));

	int oldGradient=gradientQCombo->currentIndex();
	gradientQCombo->clear();
	gradientQCombo->addItem( tr("Normal"));
	gradientQCombo->addItem( tr("Horizontal Gradient"));
	gradientQCombo->addItem( tr("Vertical Gradient"));
	gradientQCombo->addItem( tr("Diagonal Gradient"));
	gradientQCombo->addItem( tr("Cross Diagonal Gradient"));
	gradientQCombo->addItem( tr("Radial Gradient"));
	gradientQCombo->addItem( tr("Free linear Gradient"));
	gradientQCombo->addItem( tr("Free radial Gradient"));
	gradientQCombo->addItem( tr("Pattern"));
	gradientQCombo->setCurrentIndex(oldGradient);
	TransGroup->setTitle( tr( "Transparency Settings" ));
	TransTxt2->setText( tr( "Blend Mode:" ) );
	blendMode->clear();
	blendMode->addItem( tr("Normal"));
	blendMode->addItem( tr("Darken"));
	blendMode->addItem( tr("Lighten"));
	blendMode->addItem( tr("Multiply"));
	blendMode->addItem( tr("Screen"));
	blendMode->addItem( tr("Overlay"));
	blendMode->addItem( tr("Hard Light"));
	blendMode->addItem( tr("Soft Light"));
	blendMode->addItem( tr("Difference"));
	blendMode->addItem( tr("Exclusion"));
	blendMode->addItem( tr("Color Dodge"));
	blendMode->addItem( tr("Color Burn"));
	blendMode->addItem( tr("Hue"));
	blendMode->addItem( tr("Saturation"));
	blendMode->addItem( tr("Color"));
	blendMode->addItem( tr("Luminosity"));
	displayAllColors->setText( tr( "Display only used Colors" ));

	editLineColorSelector->setToolTip( tr( "Edit Line Color Properties" ) );
	editFillColorSelector->setToolTip( tr( "Edit Fill Color Properties" ) );
	PM1->setToolTip( tr( "Saturation of color" ) );
	gradientQCombo->setToolTip( tr( "Normal or gradient fill method" ) );
	TransSpin->setToolTip( tr( "Set the transparency for the color selected" ) );
	gradEditButton->setToolTip( "<qt>" + tr( "Move the start of the gradient vector with the left mouse button pressed and move the end of the gradient vector with the right mouse button pressed" ) + "</qt>");
	displayAllColors->setToolTip( "<qt>" + tr( "Display all colors from the document color list, or only the already used colors" ) + "</qt>");
}
*/
