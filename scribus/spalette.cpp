/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
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

#include "scribusdoc.h"
#include "spalette.h"

#include <QListView>

#include "page.h"
#include "util.h"

ParaStyleComboBox::ParaStyleComboBox(QWidget* parent) : QComboBox(parent)
{
//	setMinimumSize(QSize(10,static_cast<int>(font().pointSize()*2.5)));
//	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, 0, 0,
//  										 sizePolicy().hasHeightForWidth() ) );
	setEditable(false);
	addItem( tr("No Style"));
	currentDoc = NULL;
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void ParaStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	currentDoc = newCurrentDoc;
	updateFormatList();
}

void ParaStyleComboBox::setFormat(QString name)
{
	setCurrentComboItem(this, name.isEmpty() ? tr("No Style") : name);
}

void ParaStyleComboBox::updateFormatList()
{
	disconnect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
	clear();
	if (currentDoc != NULL)
	{
		QStringList st;
		st.clear();
		addItem( tr("No Style"));
		for (int x = 0; x < currentDoc->paragraphStyles().count(); ++x)
			if ( !currentDoc->paragraphStyles()[x].name().isEmpty() )
				st.append(currentDoc->paragraphStyles()[x].name());
		st.sort();
		addItems(st);
	}
	QListView *tmpView = dynamic_cast<QListView*>(view());
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void ParaStyleComboBox::selFormat(int e)
{
	if (e == 0)
	{
		emit newStyle(QString::null);
	}
	else
	{
		emit newStyle(currentText());
	}
}

CharStyleComboBox::CharStyleComboBox(QWidget* parent) : QComboBox(parent)
{
//	setMinimumSize(QSize(10,static_cast<int>(font().pointSize()*2.5)));
//	setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)3, (QSizePolicy::SizeType)1, 0, 0,
//  										 sizePolicy().hasHeightForWidth() ) );
	setEditable(false);
	addItem( tr("No Style"));
	currentDoc = NULL;
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void CharStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	currentDoc = newCurrentDoc;
	updateFormatList();
}

void CharStyleComboBox::setFormat(QString name)
{
	setCurrentComboItem(this, name.isEmpty() ? tr("No Style") : name);
}

void CharStyleComboBox::updateFormatList()
{
	disconnect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
	clear();
	if (currentDoc != NULL)
	{
		QStringList st;
		st.clear();
		addItem( tr("No Style"));
		for (int x = 0; x < currentDoc->charStyles().count(); ++x)
			if ( !currentDoc->charStyles()[x].name().isEmpty() )
				st.append(currentDoc->charStyles()[x].name());
		st.sort();
		addItems(st);
	}
	QListView *tmpView = dynamic_cast<QListView*>(view());
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void CharStyleComboBox::selFormat(int e)
{
	if (e == 0)
	{
		emit newStyle(QString::null);
	}
	else
	{
		emit newStyle(currentText());
	}
}
