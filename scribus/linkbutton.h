/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include <QToolButton>
#include "scribusapi.h"

/**
  *@author Franz Schmid
  */

class SCRIBUS_API LinkButton : public QToolButton
{
public: 
	/*!
	\author Franz Schmid
	\brief Constructor for Link buttons
	\param pa Parent Window
	 */
	LinkButton(QWidget *pa);
	~LinkButton() {};
	/*!
	\author Franz Schmid
	\brief Returns size of QPixmap, see the Qt-Documentation for further explaining of that.
	\retval QSize size
	*/
	virtual QSize sizeHint() const;
	/*!
	\author Franz Schmid
	\brief Returns minimum size of QPixmap, see the Qt-Documentation for further explaining of that.
	\retval QSize size
	*/
	virtual QSize minimumSizeHint() const;
};
 
#endif
