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

#include "sccombobox.h"

#ifdef Q_WS_MAC

#include <QMacStyle>

/// use listbox instead of popupmenu for combo boxes
class ScMacStyle : public QMacStyle {
	virtual int styleHint(StyleHint sh, 
	                                  const QStyleOption * so, 
					  const QWidget * w, 
	                                  QStyleHintReturn * r) const
	{
        	return sh != QStyle::SH_ComboBox_Popup && QMacStyle::styleHint(sh, so, w, r); 
	}
};
#endif

ScComboBox::ScComboBox( QWidget* parent ) : QComboBox(parent)
{
#ifdef Q_WS_MAC
	setStyle( new ScMacStyle );
#endif
//Qt4	setListBox( new Q3ListBox( this, "in-combo", Qt::WType_Popup ) );
}

