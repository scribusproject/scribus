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

#ifndef SCCOMBOBOX_H
#define SCCOMBOBOX_H

#include <QComboBox>

/*!
    \class ScComboBox sccombobox.h
    \brief The ScComboBox widget always uses a listbox instead of a popup, 
           independent of any QStyle.
*/
class ScComboBox : public QComboBox
{

public: 
	ScComboBox( QWidget* parent=0);
};

#endif
