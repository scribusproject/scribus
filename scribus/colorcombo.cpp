/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/*
 *  sccombobox.h
 *  ScribusCVS
 *
 *  Created by Andreas Vox on 01.10.05.
 *  Copyright 2005 under GPL2. All rights reserved.
 *
 */

#include "colorcombo.h"
#include "commonstrings.h"
#include "qdebug.h"

ColorCombo::ColorCombo( QWidget* parent ) : QComboBox(parent)
{
#ifdef Q_WS_MAC
//	setStyle( new ColorCombo::ScMacStyle() );
#endif
	setEditable(false);
	ColorListBox* lb = new ColorListBox(this);
	setModel( lb->model() );
	setItemDelegate( lb->itemDelegate() );
	setView( lb );
}

ColorCombo::ColorCombo( bool rw, QWidget* parent ) : QComboBox(parent)
{
#ifdef Q_WS_MAC
//	setStyle( new ColorCombo::ScMacStyle() );
#endif
	setEditable(rw);
	ColorListBox* lb = new ColorListBox(this);
	setModel( lb->model() );
	setItemDelegate( lb->itemDelegate() );
	setView( lb );
}


QString ColorCombo::currentColor() const
{
		return currentText();
}

void ColorCombo::updateBox(ColorList& list, ColorCombo::PixmapType pixType , bool insertNone)
{
	ColorListBox* clb = dynamic_cast<ColorListBox*>(view());
	if ( clb )
	{
		clb->clear();
		if ( insertNone )
			clb->addItem(CommonStrings::tr_NoneColor);
		if ( pixType == ColorCombo::fancyPixmaps )
			clb->insertItems(list, ColorListBox::fancyPixmap);
		else if ( pixType == ColorCombo::widePixmaps )
			clb->insertItems(list, ColorListBox::widePixmap);
		else if ( pixType == ColorCombo::smallPixmaps )
			clb->insertItems(list, ColorListBox::smallPixmap);
	}
}

void ColorCombo::insertItems(ColorList& list, ColorCombo::PixmapType pixType)
{
	ColorListBox* clb = dynamic_cast<ColorListBox*>(view());
	if ( clb )
	{
		if ( pixType == ColorCombo::fancyPixmaps )
			clb->insertItems(list, ColorListBox::fancyPixmap);
		else if ( pixType == ColorCombo::widePixmaps )
			clb->insertItems(list, ColorListBox::widePixmap);
		else if ( pixType == ColorCombo::smallPixmaps )
			clb->insertItems(list, ColorListBox::smallPixmap);
	}
}

void ColorCombo::insertSmallItem( const ScColor& col, ScribusDoc* doc, const QString& colName )
{
	ColorListBox* clb = dynamic_cast<ColorListBox*>(view());
	if ( clb )
		clb->addItem( new ColorPixmapItem(ColorPixmapValue(col, doc, colName)) );
}

void ColorCombo::insertWideItem ( const ScColor& col, ScribusDoc* doc, const QString& colName )
{
	ColorListBox* clb = dynamic_cast<ColorListBox*>(view());
	if ( clb )
		clb->addItem( new ColorPixmapItem(ColorPixmapValue(col, doc, colName)) );
}

void ColorCombo::insertFancyItem( const ScColor& col, ScribusDoc* doc, const QString& colName )
{
	ColorListBox* clb = dynamic_cast<ColorListBox*>(view());
	if ( clb )
		clb->addItem( new ColorPixmapItem(ColorPixmapValue(col, doc, colName)) );
}

