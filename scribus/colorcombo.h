/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/*
 *  colorcombo.h
 *  ScribusCVS
 *
 *  Created by Jean Ghali on 18.02.06.
 *  Copyright 2006 under GPL2. All rights reserved.
 *
 */

#ifndef COLORCOMBO_H
#define COLORCOMBO_H

#include <qcombobox.h>
#include "colorm.h"

#ifdef QT_MAC
#include <qmacstyle_mac.h>
#endif

/*!
    \class ColorCombo colorcombo.h
    \brief The ColorCombo widget is a combo box for displaying list of colors.
*/
class ColorCombo : public QComboBox 
{
	Q_OBJECT

private:

#ifdef QT_MAC
	/// use listbox instead of popupmenu for combo boxes
	class ScMacStyle : public QMacStyle 
	{
		virtual int ScMacStyle::styleHint(StyleHint sh, const QWidget * w, 
	                                  const QStyleOption & so, 
	                                  QStyleHintReturn * r) const
		{
        	return sh != QStyle::SH_ComboBox_Popup && QMacStyle::styleHint(sh, w, so, r); 
		}
	};
#endif

public:

	enum PixmapType
	{
		smallPixmaps,
		widePixmaps,
		fancyPixmaps
	};

	ColorCombo( QWidget* parent=0, const char* name=0 );
	ColorCombo( bool rw, QWidget* parent=0, const char* name=0 );

	void updateBox(ColorList& list, ColorCombo::PixmapType pixType, bool insertNone = true);

	void insertSmallItem( const ScColor& col, const QString& colName );
	void insertWideItem ( const ScColor& col, const QString& colName );
	void insertFancyItem( const ScColor& col, const QString& colName );
};

#endif
