/***************************************************************************
                          spalette.cpp  -  description
                             -------------------
    begin                : Wed Apr 25 2001
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

#include "spalette.h"
#include "spalette.moc"

Spalette::Spalette(QWidget* parent) : QListBox(parent, "Sfloat")
{
  setMinimumSize(QSize(10,static_cast<int>(font().pointSize()*2.5)));
  setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, 0, 0, sizePolicy().hasHeightForWidth() ) );
	insertItem(tr("No Style"));
  doc = 0;
  connect(this, SIGNAL(clicked(QListBoxItem*)), this, SLOT(selFormat(QListBoxItem*)));
  connect(this, SIGNAL(selected(int)), this, SIGNAL(EditSt()));
}

void Spalette::SetFormats(ScribusDoc *dd)
{
	doc = dd;
	updateFList();
}

void Spalette::updateFList()
{
	clear();
	if (doc != 0)
		{
		insertItem(tr("No Style"));
		for (uint x = 5; x < doc->Vorlagen.count(); ++x)
			{
			insertItem(doc->Vorlagen[x].Vname);
			}
		setSelected(currentItem(), false);
		}
}

void Spalette::selFormat(QListBoxItem *c)
{
	if (c == NULL)
  	return;
  if (c->listBox()->currentItem() > 0)
  	emit NewStyle(c->listBox()->currentItem()+4);
  else
  	emit NewStyle(c->listBox()->currentItem());
}
