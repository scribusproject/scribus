/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/*
 *  Created by Andreas Vox on 01.10.05.
 *  Copyright 2005 under GPL2. All rights reserved.
 *
 */

#include "colorcombo.h"
#include "colorlistmodel.h"
#include "commonstrings.h"
// #include "qdebug.h"

ColorCombo::ColorCombo(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	ColorListBox* lb = new ColorListBox(this);
	setModel( lb->model() );
	setItemDelegate( lb->itemDelegate() );
	setView( lb );
}

ColorCombo::ColorCombo(ColorCombo::PixmapType type, QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	ColorListBox::PixmapType clbType = ColorListBox::smallPixmap;
	if (type == ColorCombo::widePixmaps)
		clbType = ColorListBox::widePixmap;
	else if (type == ColorCombo::fancyPixmaps)
		clbType = ColorListBox::fancyPixmap;
	ColorListBox* lb = new ColorListBox(clbType, this);
	setModel( lb->model() );
	setItemDelegate( lb->itemDelegate() );
	setView( lb );
}

ColorCombo::ColorCombo(bool rw, QWidget* parent) : QComboBox(parent)
{
	setEditable(rw);
	ColorListBox* lb = new ColorListBox(this);
	setModel( lb->model() );
	setItemDelegate( lb->itemDelegate() );
	setView( lb );
}


QString ColorCombo::currentColor() const
{
	QString colorName(currentText());
	if (colorName == CommonStrings::tr_NoneColor)
		colorName = CommonStrings::None;
	return colorName;
}

void ColorCombo::setColors(ColorList& list, bool insertNone)
{
	ColorListBox* clb = qobject_cast<ColorListBox*>(view());
	if (!clb)
		return;

	ColorListModel* clm = qobject_cast<ColorListModel*>(clb->model());
	if (!clm)
		return;

	clm->setColorList(list, insertNone);
}

void ColorCombo::setCurrentColor(QString colorName)
{
	if (colorName == CommonStrings::None)
		colorName = CommonStrings::tr_NoneColor;
	
	int index = findText(colorName);
	if (index >= 0)
		this->setCurrentIndex(index);
}

void ColorCombo::setPixmapType(ColorCombo::PixmapType type)
{
	ColorListBox* clb = qobject_cast<ColorListBox*>(view());
	if (!clb)
		return;

	if (type == ColorCombo::smallPixmaps)
		clb->setPixmapType(ColorListBox::smallPixmap);
	else if (type == ColorCombo::widePixmaps)
		clb->setPixmapType(ColorListBox::widePixmap);
	else if (type == ColorCombo::fancyPixmaps)
		clb->setPixmapType(ColorListBox::fancyPixmap);
}

void ColorCombo::initColorList(ColorList *colorList, ScribusDoc *doc, const QString& colorValue)
{
	clear();
	setPixmapType(ColorCombo::fancyPixmaps);
	setColors(*colorList, true);

	if (colorValue == CommonStrings::None)
	{
		setCurrentIndex(0);
		return;
	}

	int index = 0;
	ColorList::Iterator endOfColorList(colorList->end());
	for (ColorList::Iterator itc = colorList->begin(); itc != endOfColorList; ++itc)
	{
		++index;
		if (itc.key() == colorValue)
		{
			setCurrentIndex(index);
			break;
		}
	}
}
