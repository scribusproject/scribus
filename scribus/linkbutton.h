/***************************************************************************
                          linkbutton.h  -  description
                             -------------------
    begin                : Wed Jul 11 2001
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

#ifndef LINKBUTTON_H
#define LINKBUTTON_H

#include <qtoolbutton.h>
#include "scribusapi.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API LinkButton : public QToolButton  
{
public: 
	LinkButton(QWidget *pa);
	~LinkButton() {};
	virtual QSize sizeHint();
	virtual QSize minimumSizeHint();
};

#endif
