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

Spalette::Spalette(QWidget* parent) : QComboBox(true, parent, "Sfloat")
{
//	setMinimumSize(QSize(10,static_cast<int>(font().pointSize()*2.5)));
//	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, 0, 0,
//  										 sizePolicy().hasHeightForWidth() ) );
	setEditable(false);
	insertItem( tr("No Style"));
	doc = 0;
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void Spalette::SetFormats(ScribusDoc *dd)
{
	doc = dd;
	updateFList();
}

void Spalette::setFormat(int e)
{
	if (e < 5)
		setCurrentItem(0);
	else
		setCurrentText(doc->Vorlagen[e].Vname);
}

void Spalette::updateFList()
{
	disconnect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
	clear();
	if (doc != 0)
	{
		QStringList st;
		st.clear();
		insertItem( tr("No Style"));
		for (uint x = 5; x < doc->Vorlagen.count(); ++x)
			st.append(doc->Vorlagen[x].Vname);
		st.sort();
		insertStringList(st);
	}
	listBox()->setMinimumWidth(listBox()->maxItemWidth()+24);
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void Spalette::selFormat(int e)
{
	if (e == 0)
		{
		emit NewStyle(0);
		return;
		}
	for (uint x = 5; x < doc->Vorlagen.count(); ++x)
		{
		if (doc->Vorlagen[x].Vname == currentText())
			{
			emit NewStyle(x);
			break;
			}
		}
}
