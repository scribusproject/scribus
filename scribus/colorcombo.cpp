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

#include <q3listbox.h>
#include "colorcombo.h"
//#include "colorcombo.moc"
#include "commonstrings.h"


ColorCombo::ColorCombo( QWidget* parent, const char* name ) : Q3ComboBox(false, parent, name)
{
#ifdef QT_MAC
	setStyle( new ColorCombo::ScMacStyle() );
#endif
	setListBox( new ColorListBox( this, "in-combo", Qt::WType_Popup ) );
}

ColorCombo::ColorCombo( bool rw, QWidget* parent, const char* name ) : Q3ComboBox(rw, parent, name)
{
#ifdef QT_MAC
	setStyle( new ColorCombo::ScMacStyle() );
#endif
	setListBox( new ColorListBox( this, "in-combo", Qt::WType_Popup ) );
}

void ColorCombo::updateBox(ColorList& list, ColorCombo::PixmapType pixType , bool insertNone)
{
	ColorListBox* clb = (ColorListBox*) listBox();
	if ( clb )
	{
		clb->clear();
		if ( insertNone )
			clb->insertItem(CommonStrings::tr_NoneColor);
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
	ColorListBox* clb = (ColorListBox*) listBox();
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
	ColorListBox* clb = (ColorListBox*) listBox();
	if ( clb )
		clb->insertItem( new ColorSmallPixmapItem(col, doc, colName) );
}

void ColorCombo::insertWideItem ( const ScColor& col, ScribusDoc* doc, const QString& colName )
{
	ColorListBox* clb = (ColorListBox*) listBox();
	if ( clb )
		clb->insertItem( new ColorWidePixmapItem(col, doc, colName) );
}

void ColorCombo::insertFancyItem( const ScColor& col, ScribusDoc* doc, const QString& colName )
{
	ColorListBox* clb = (ColorListBox*) listBox();
	if ( clb )
		clb->insertItem( new ColorFancyPixmapItem(col, doc, colName) );
}

