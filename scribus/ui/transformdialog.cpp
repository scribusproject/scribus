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
*   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
***************************************************************************/

#include "transformdialog.h"

#include "commonstrings.h"
#include "selection.h"
#include "units.h"
#include "iconmanager.h"

#if defined(_MSC_VER) && !defined(_USE_MATH_DEFINES)
#define _USE_MATH_DEFINES
#endif
#include <cmath>

TransformItem::TransformItem(const QString& text, QListWidget* parent, int type, double val1, double val2) : QListWidgetItem(text, parent, type)
{
	firstValue = val1;
	secondValue = val2;
}

TransformDialog::TransformDialog(QWidget* parent, ScribusDoc *doc) : QDialog(parent)
{
	setupUi(this);
	setModal(true);
	setWindowIcon(IconManager::instance().loadIcon("AppIcon.png"));
	transformStack->setCurrentIndex(0);
	newTransformMenu = new QMenu(buttonAdd);
	newTransformMenu->addAction( tr("Scaling"), this, SLOT(newScaling()));
	newTransformMenu->addAction( tr("Translation"), this, SLOT(newTranslation()));
	newTransformMenu->addAction( tr("Rotation"), this, SLOT(newRotation()));
	newTransformMenu->addAction( tr("Skewing"), this, SLOT(newSkewing()));
	buttonAdd->setMenu(newTransformMenu);
	scaleLink->setChecked(true);
	buttonUp->setText( "" );
	buttonUp->setIcon(IconManager::instance().loadIcon("16/go-up.png"));
	buttonDown->setText( "" );
	buttonDown->setIcon(IconManager::instance().loadIcon("16/go-down.png"));
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	m_doc = doc;
	m_unitRatio = unitGetRatioFromIndex(m_doc->unitIndex());
	m_suffix = unitGetSuffixFromIndex(m_doc->unitIndex());
	translateHorizontal->setNewUnit(m_doc->unitIndex());
	translateVertical->setNewUnit(m_doc->unitIndex());
	rotationValue->setWrapping( true );
	rotationValue->setValues( -180.0, 180.0, 1, 0);
	rotationValue->setSuffix(unitGetSuffixFromIndex(6));
	horizontalSkew->setSuffix(unitGetSuffixFromIndex(6));
	verticalSkew->setSuffix(unitGetSuffixFromIndex(6));
	linkSkew->setChecked(true);
	numberOfCopies->setValue(0);
    basePoint->setSelectedAnchor(m_doc->rotationMode());
	connect(transformSelector, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(setCurrentTransform(QListWidgetItem*)));
	connect(horizontalScale, SIGNAL(valueChanged(double)), this, SLOT(changeHScale(double)));
	connect(verticalScale, SIGNAL(valueChanged(double)), this, SLOT(changeVScale(double)));
	connect(scaleLink, SIGNAL(clicked()), this, SLOT(toggleLink()));
	connect(translateHorizontal, SIGNAL(valueChanged(double)), this, SLOT(changeHTranslation(double)));
	connect(translateVertical, SIGNAL(valueChanged(double)), this, SLOT(changeVTranslation(double)));
	connect(rotationValue, SIGNAL(valueChanged(double)), this, SLOT(changeRotation(double)));
	connect(horizontalSkew, SIGNAL(valueChanged(double)), this, SLOT(changeHSkew(double)));
	connect(verticalSkew, SIGNAL(valueChanged(double)), this, SLOT(changeVSkew(double)));
	connect(linkSkew, SIGNAL(clicked()), this, SLOT(toggleLinkSkew()));
	connect(buttonUp, SIGNAL(clicked()), this, SLOT(moveTransformUp()));
	connect(buttonDown, SIGNAL(clicked()), this, SLOT(moveTransformDown()));
	connect(buttonRemove, SIGNAL(clicked()), this, SLOT(removeTransform()));
}

void TransformDialog::newScaling()
{
	TransformItem *item = new TransformItem( tr("Scale"), transformSelector, 1001, 100.0, 100.0);
	transformSelector->setCurrentItem(item);
	setCurrentTransform(item);
	item->setText( tr("Scale H = %1 % V = %2 %").arg(100.0).arg(100.0));
	buttonRemove->setEnabled(true);
}

void TransformDialog::newTranslation()
{
	TransformItem *item = new TransformItem( tr("Translate"), transformSelector, 1002, 0.0, 0.0);
	transformSelector->setCurrentItem(item);
	setCurrentTransform(item);
	item->setText( tr("Translate H = %1%2 V = %3%4").arg(0.0).arg(m_suffix).arg(0.0).arg(m_suffix));
	buttonRemove->setEnabled(true);
}

void TransformDialog::newRotation()
{
	TransformItem *item = new TransformItem( tr("Rotate"), transformSelector, 1003, 0.0, 0.0);
	transformSelector->setCurrentItem(item);
	setCurrentTransform(item);
	item->setText( tr("Rotate Angle = %1%2").arg(0.0).arg(unitGetSuffixFromIndex(6)));
	buttonRemove->setEnabled(true);
}

void TransformDialog::newSkewing()
{
	TransformItem *item = new TransformItem( tr("Skew"), transformSelector, 1004, 0.0, 0.0);
	transformSelector->setCurrentItem(item);
	setCurrentTransform(item);
	item->setText( tr("Skew H = %1%2 V = %3%4").arg(0.0).arg(unitGetSuffixFromIndex(6)).arg(0.0).arg(unitGetSuffixFromIndex(6)));
	buttonRemove->setEnabled(true);
}

void TransformDialog::setCurrentTransform(QListWidgetItem* item)
{
	if (item == nullptr)
	{
		transformStack->setCurrentIndex(0);
		buttonRemove->setEnabled(false);
		buttonUp->setEnabled(false);
		buttonDown->setEnabled(false);
		return;
	}

	TransformItem *ite = (TransformItem*) item;
	switch (item->type())
	{
		case 1001:
			transformStack->setCurrentIndex(1);
			horizontalScale->setValue(ite->firstValue);
			verticalScale->setValue(ite->secondValue);
			break;
		case 1002:
			transformStack->setCurrentIndex(2);
			translateHorizontal->setValue(ite->firstValue);
			translateVertical->setValue(ite->secondValue);
			break;
		case 1003:
			transformStack->setCurrentIndex(3);
			rotationValue->setValue(ite->firstValue);
			break;
		case 1004:
			transformStack->setCurrentIndex(4);
			horizontalSkew->setValue(ite->firstValue);
			verticalSkew->setValue(ite->secondValue);
			break;
		default:
			transformStack->setCurrentIndex(0);
			break;
	}
	if (transformSelector->count() > 1)
	{
		buttonUp->setEnabled(true);
		buttonDown->setEnabled(true);
		if (transformSelector->currentRow() == 0)
			buttonUp->setEnabled(false);
		if (transformSelector->currentRow() == transformSelector->count()-1)
			buttonDown->setEnabled(false);
	}
	else
	{
		buttonUp->setEnabled(false);
		buttonDown->setEnabled(false);
	}
	buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}

void TransformDialog::changeHScale(double val)
{
	QListWidgetItem* item = transformSelector->currentItem();
	TransformItem *ite = (TransformItem*)item;
	ite->firstValue = val;
	if (scaleLink->isChecked())
	{
		verticalScale->setValue(horizontalScale->value());
		ite->secondValue = val;
	}
	item->setText( tr("Scale H = %1 % V = %2 %").arg(ite->firstValue).arg(ite->secondValue));
}

void TransformDialog::changeVScale(double val)
{
	QListWidgetItem* item = transformSelector->currentItem();
	TransformItem *ite = (TransformItem*)item;
	ite->secondValue = val;
	if (scaleLink->isChecked())
	{
		horizontalScale->setValue(verticalScale->value());
		ite->firstValue = val;
	}
	item->setText( tr("Scale H = %1 % V = %2 %").arg(ite->firstValue).arg(ite->secondValue));
}

void TransformDialog::toggleLink()
{
	if (scaleLink->isChecked())
		verticalScale->setValue(horizontalScale->value());
	QListWidgetItem* item = transformSelector->currentItem();
	TransformItem *ite = (TransformItem*)item;
	item->setText( tr("Scale H = %1 % V = %2 %").arg(ite->firstValue).arg(ite->secondValue));
}

void TransformDialog::changeHTranslation(double val)
{
	QListWidgetItem* item = transformSelector->currentItem();
	TransformItem *ite = (TransformItem*)item;
	ite->firstValue = val;
	item->setText( tr("Translate H = %1%2 V = %3%4").arg(ite->firstValue).arg(m_suffix).arg(ite->secondValue).arg(m_suffix));
}

void TransformDialog::changeVTranslation(double val)
{
	QListWidgetItem* item = transformSelector->currentItem();
	TransformItem *ite = (TransformItem*)item;
	ite->secondValue = val;
	item->setText( tr("Translate H = %1%2 V = %3%4").arg(ite->firstValue).arg(m_suffix).arg(ite->secondValue).arg(m_suffix));
}

void TransformDialog::changeRotation(double val)
{
	QListWidgetItem* item = transformSelector->currentItem();
	TransformItem *ite = (TransformItem*)item;
	ite->firstValue = -val;
	item->setText( tr("Rotate Angle = %1%2").arg(-ite->firstValue).arg(unitGetSuffixFromIndex(6)));
}

void TransformDialog::changeHSkew(double val)
{
	QListWidgetItem* item = transformSelector->currentItem();
	TransformItem *ite = (TransformItem*)item;
	ite->firstValue = val;
	if (linkSkew->isChecked())
	{
		verticalSkew->setValue(horizontalSkew->value());
		ite->secondValue = val;
	}
	item->setText( tr("Skew H = %1%2 V = %3%4").arg(ite->firstValue).arg(unitGetSuffixFromIndex(6)).arg(ite->secondValue).arg(unitGetSuffixFromIndex(6)));
}

void TransformDialog::changeVSkew(double val)
{
	QListWidgetItem* item = transformSelector->currentItem();
	TransformItem *ite = (TransformItem*)item;
	ite->secondValue = val;
	if (linkSkew->isChecked())
	{
		horizontalSkew->setValue(verticalSkew->value());
		ite->firstValue = val;
	}
	item->setText( tr("Skew H = %1%2 V = %3%4").arg(ite->firstValue).arg(unitGetSuffixFromIndex(6)).arg(ite->secondValue).arg(unitGetSuffixFromIndex(6)));
}

void TransformDialog::toggleLinkSkew()
{
	if (linkSkew->isChecked())
		verticalSkew->setValue(horizontalSkew->value());
	QListWidgetItem* item = transformSelector->currentItem();
	TransformItem *ite = (TransformItem*)item;
	item->setText( tr("Skew H = %1%2 V = %3%4").arg(ite->firstValue).arg(unitGetSuffixFromIndex(6)).arg(ite->secondValue).arg(unitGetSuffixFromIndex(6)));
}

void TransformDialog::moveTransformUp()
{
	int curr = transformSelector->currentRow();
	if (curr == 0)
		return;
	QListWidgetItem *it = transformSelector->takeItem(curr);
	transformSelector->insertItem(curr-1, it);
	transformSelector->setCurrentItem(it);
	setCurrentTransform(it);
}

void TransformDialog::moveTransformDown()
{
	int curr = transformSelector->currentRow();
	if (curr == transformSelector->count()-1)
		return;
	QListWidgetItem *it = transformSelector->takeItem(curr);
	transformSelector->insertItem(curr+1, it);
	transformSelector->setCurrentItem(it);
	setCurrentTransform(it);
}

void TransformDialog::removeTransform()
{
	int curr = transformSelector->currentRow();
	QListWidgetItem *it = transformSelector->takeItem(curr);
	delete it;
	transformSelector->clearSelection();
	if (transformSelector->count() == 0)
	{
		transformStack->setCurrentIndex(0);
		buttonRemove->setEnabled(false);
		buttonUp->setEnabled(false);
		buttonDown->setEnabled(false);
		buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
	}
	else
	{
		transformSelector->setCurrentItem(transformSelector->item(qMax(curr-1, 0)));
		transformSelector->currentItem()->setSelected(true);
		setCurrentTransform(transformSelector->currentItem());
		buttonRemove->setEnabled(true);
	}
}

QTransform TransformDialog::getTransformMatrix()
{
	QTransform ret = QTransform();
	for (int a = 0; a < transformSelector->count(); a++)
	{
		QListWidgetItem *it = transformSelector->item(a);
		TransformItem *ite = (TransformItem*)it;
		switch (it->type())
		{
			case 1001:
				ret.scale(ite->firstValue / 100.0, ite->secondValue / 100.0);
				break;
			case 1002:
				ret.translate(ite->firstValue / m_unitRatio, ite->secondValue / m_unitRatio);
				break;
			case 1003:
				ret.rotate(ite->firstValue);
				break;
			case 1004:
				ret.shear(-sin(ite->firstValue / 180.0 * M_PI), -sin(ite->secondValue / 180.0 * M_PI));
				break;
			default:
				break;
		}
	}
	return ret;
}

int TransformDialog::getCount()
{
	return numberOfCopies->value();
}

BasePointWidget::AnchorPoint TransformDialog::getBasepoint()
{
    return basePoint->selectedAnchor();
}
