/***************************************************************************
                          linecombo.cpp  -  description
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

#include "linecombo.h"
#include "icons5.h"

LineCombo::LineCombo(QWidget* pa) : QComboBox(true, pa)
{
	setEditable(false);
  insertItem(QPixmap(solidL));
  insertItem(QPixmap(dashL));
  insertItem(QPixmap(dotL));
  insertItem(QPixmap(dashdotL));
  insertItem(QPixmap(dashdotdotL));
}

