/***************************************************************************
                          fontcombo.h  -  description
                             -------------------
    begin                : Die Jun 17 2003
    copyright            : (C) 2003 by Franz Schmid
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

#ifndef FONTCOMBO_H
#define FONTCOMBO_H

#include <qcombobox.h>
#include <qlistbox.h>
#include <qfont.h>
#include <qpainter.h>
#include "scribusdoc.h"
/**
  *@author Franz Schmid
  */

class FontListItem : public QListBoxItem
{
public:
    FontListItem(QComboBox* parent, QString f, QFont fo);
    virtual ~FontListItem();

    virtual int width(const QListBox *) const;
    virtual int height(const QListBox *) const;

protected:
    virtual void paint(QPainter *p);

private:
    QFont Ifont;
    QString fontName;
};

class FontCombo : public QComboBox  {
public:
	FontCombo(QWidget* pa, preV *Prefs);
	void RebuildList(preV *Prefs);
	~FontCombo() {};
};

#endif
