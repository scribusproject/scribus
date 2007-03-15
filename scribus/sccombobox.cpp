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
#include "sccombobox.h"

#ifdef QT_MAC

#include <qmacstyle_mac.h>

/// use listbox instead of popupmenu for combo boxes
class ScMacStyle : public QMacStyle {
	virtual int ScMacStyle::styleHint(StyleHint sh, const QWidget * w, 
	                                  const QStyleOption & so, 
	                                  QStyleHintReturn * r) const
	{
        	return sh != QStyle::SH_ComboBox_Popup && QMacStyle::styleHint(sh, w, so, r); 
	}
};
#endif

ScComboBox::ScComboBox( QWidget* parent, const char* name ) : Q3ComboBox(false, parent, name) 
{
#ifdef QT_MAC
	setStyle( new ScMacStyle );
#endif
	setListBox( new Q3ListBox( this, "in-combo", Qt::WType_Popup ) );
}

ScComboBox::ScComboBox( bool rw, QWidget* parent, const char* name ) :Q3ComboBox(rw, parent, name)
{
#ifdef QT_MAC
	setStyle( new ScMacStyle );
#endif
	setListBox( new Q3ListBox( this, "in-combo", Qt::WType_Popup ) );
}
