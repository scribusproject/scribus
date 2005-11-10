/***************************************************************************
                          linkbutton.cpp  -  description
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

#include "linkbutton.h"
#include "icons6.h"
#include <qicon.h>
//Added by qt3to4:
#include <QPixmap>

/*!
 \fn LinkButton::LinkButton(QWidget *pa)
 \author Franz Schmid
 \date
 \brief Constructor for Link buttons
 \param pa Parent Window
 \retval None
 */

LinkButton::LinkButton(QWidget *pa) : QToolButton(pa)
{
	setBackgroundMode(Qt::PaletteBackground);
	QIcon a = QIcon();
	a.setPixmap(QPixmap(ChainC), QIcon::Automatic, QIcon::Normal, QIcon::On);
	a.setPixmap(QPixmap(ChainO), QIcon::Automatic, QIcon::Normal, QIcon::Off);
	setIconSet(a);
}

/*!
 \fn QSize LinkButton::sizeHint()
 \author Franz Schmid
 \date
 \brief Returns size of QPixmap, see the Qt-Documentation for further explaining of that.
 \param None
 \retval QSize size
 */

QSize LinkButton::sizeHint()
{
	return QSize(QPixmap(ChainC).width(),QPixmap(ChainC).height());
}

/*!
 \fn QSize LinkButton::minimumSizeHint()
 \author Franz Schmid
 \date
 \brief Returns minimum size of QPixmap, see the Qt-Documentation for further explaining of that.
 \param None
 \retval QSize size
 */

QSize LinkButton::minimumSizeHint()
{
	return QSize(QPixmap(ChainC).width(),QPixmap(ChainC).height());
}
