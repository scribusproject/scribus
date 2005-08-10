/***************************************************************************
                          linecombo.h  -  description
                             -------------------
    begin                : Thu Jul 12 2001
    copyright            : (C) 2001 by Franz Schmid
    email                : Franz.Schmid@altmuehlnet.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LINECOMBO_H
#define LINECOMBO_H

#include <qcombobox.h>
#include <qpixmap.h>

#include "scribusapi.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API LineCombo : public QComboBox  
{
public: 
	LineCombo(QWidget* pa);
	~LineCombo() {};
};

#endif
