/*
 *  sccombobox.h
 *  ScribusCVS
 *
 *  Created by Andreas Vox on 01.10.05.
 *  Copyright 2005 under GPL2. All rights reserved.
 *
 */
#include <qcombobox.h>


/*!
    \class ScComboBox sccombobox.h
    \brief The ScComboBox widget always uses a listbox instead of a popup, 
           independent of any QStyle.
*/
class ScComboBox : public QComboBox {

public: 
	ScComboBox( QWidget* parent=0, const char* name=0 );
	ScComboBox( bool rw, QWidget* parent=0, const char* name=0 );
};

