/***************************************************************************
                          pageback.h  -  description
                             -------------------
    begin                : Sun Apr 8 2001
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

#ifndef PAGEBACK_H
#define PAGEBACK_H

#include <qwidget.h>
#include <qpainter.h>

/*!
 * \brief Page background
 * \author Franz Schmid
 */

class PageBack : public QWidget  {
public: 
	PageBack(QWidget *pa);
	~PageBack() {};
	void paintEvent(QPaintEvent *);
};

#endif
