/***************************************************************************
                          fontcombo.cpp  -  description
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
#include <qstringlist.h>
#include "fontcombo.h"

FontListItem::FontListItem(QComboBox* parent, QString f, QFont fo) : QListBoxItem(parent->listBox())
{
	fontName = f;
	Ifont = fo;
	setText(fontName);
}

int FontListItem::width(const QListBox *lb) const
{
	return lb->fontMetrics().width(text()) + 2;
}

int FontListItem::height(const QListBox *lb) const
{
	QFontMetrics fm(lb->fontMetrics());
	return fm.lineSpacing() + 2;
}

void FontListItem::paint(QPainter *p)
{
	p->setFont(Ifont);
	QFontMetrics fm(p->fontMetrics());
	p->drawText(3, fm.ascent() + fm.leading() / 2, fontName);
}

FontCombo::FontCombo(QWidget* pa, preV *Prefs) : QComboBox(true, pa)
{
	setEditable(false);
	QFont f(font());
	f.setPointSize(f.pointSize()-1);
	setFont(f);
	QStringList rlist;
	rlist.clear();
	SCFontsIterator it(Prefs->AvailFonts);
	for ( ; it.current(); ++it)
	{
		if (it.current()->UseFont)
			rlist.append(it.currentKey());
	}
	rlist.sort();
	clear();
	for (QStringList::ConstIterator it2 = rlist.begin(); it2 != rlist.end(); ++it2)
		insertItem(*it2);
	listBox()->setMinimumWidth(listBox()->maxItemWidth()+24);
}

void FontCombo::RebuildList(preV *Prefs, ScribusDoc *doc)
{
	QStringList rlist;
	clear();
	rlist.clear();
	SCFontsIterator it(Prefs->AvailFonts);
	for ( ; it.current(); ++it)
	{
		if (it.current()->UseFont)
		{
			if (doc != 0)
			{
				if ((doc->DocName == it.current()->PrivateFont) || (it.current()->PrivateFont == ""))
					rlist.append(it.currentKey());
			}
			else
				rlist.append(it.currentKey());
			}
	}
	rlist.sort();
	for (QStringList::ConstIterator it2 = rlist.begin(); it2 != rlist.end(); ++it2)
		insertItem(*it2);
	listBox()->setMinimumWidth(listBox()->maxItemWidth()+24);
}
