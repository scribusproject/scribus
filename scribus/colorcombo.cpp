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

#include <qlistbox.h>
#include "colorcombo.h"
#include "colorcombo.moc"
#include "commonstrings.h"


ColorCombo::ColorCombo( QWidget* parent, const char* name ) : QComboBox(false, parent, name)
{
#ifdef QT_MAC
	setStyle( new ColorCombo::ScMacStyle() );
#endif
	setListBox( new ColorListBox( this, "in-combo", WType_Popup ) );
}

ColorCombo::ColorCombo( bool rw, QWidget* parent, const char* name ) : QComboBox(rw, parent, name)
{
#ifdef QT_MAC
	setStyle( new ColorCombo::ScMacStyle() );
#endif
	setListBox( new ColorListBox( this, "in-combo", WType_Popup ) );
}

void ColorCombo::updateBox(ColorList& list, ColorCombo::PixmapType pixType , bool insertNone)
{
	ColorListBox* clb = (ColorListBox*) listBox();
	if ( clb )
	{
		clb->clear();
		if ( insertNone )
			clb->insertItem(CommonStrings::NoneColor);
		if ( pixType == ColorCombo::fancyPixmaps )
			clb->updateBox(list, ColorListBox::fancyPixmap, false);
		else if ( pixType == ColorCombo::widePixmaps )
			clb->updateBox(list, ColorListBox::widePixmap, false);
		else if ( pixType == ColorCombo::smallPixmaps )
			clb->updateBox(list, ColorListBox::smallPixmap, false);
	}
}

void ColorCombo::insertSmallItem( const ScColor& col, const QString& colName )
{
	ColorListBox* clb = (ColorListBox*) listBox();
	if ( clb )
		clb->insertItem( new ColorSmallPixmapItem(col, colName) );
}

void ColorCombo::insertWideItem ( const ScColor& col, const QString& colName )
{
	ColorListBox* clb = (ColorListBox*) listBox();
	if ( clb )
		clb->insertItem( new ColorWidePixmapItem(col, colName) );
}

void ColorCombo::insertFancyItem( const ScColor& col, const QString& colName )
{
	ColorListBox* clb = (ColorListBox*) listBox();
	if ( clb )
		clb->insertItem( new ColorFancyPixmapItem(col, colName) );
}

