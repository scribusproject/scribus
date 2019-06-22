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

#include "scpage.h"
#include "util.h"

ParaStyleComboBox::ParaStyleComboBox(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	addItem( CommonStrings::trDefaultParagraphStyle );
	currentDoc = nullptr;
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void ParaStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	currentDoc = newCurrentDoc;
	updateFormatList();
}

void ParaStyleComboBox::setFormat(const QString& name)
{
	setCurrentComboItem(this, name.isEmpty() ? CommonStrings::trDefaultParagraphStyle : name);
}

void ParaStyleComboBox::updateFormatList()
{
	QString oldStyleName;
	if (currentDoc && this->count() > 0)
		oldStyleName = this->currentText();

	bool sigBlocked = this->blockSignals(true);
	clear();
	if (currentDoc != nullptr)
	{
		QStringList st;
		addItem( CommonStrings::trDefaultParagraphStyle );
		for (int x = 0; x < currentDoc->paragraphStyles().count(); ++x)
		{
			if ( !currentDoc->paragraphStyles()[x].name().isEmpty() && !currentDoc->paragraphStyles()[x].isDefaultStyle())
				st.append(currentDoc->paragraphStyles()[x].name());
		}
		st.sort();
		addItems(st);

		if (oldStyleName.length() > 0)
		{
			int newIndex = this->findText(oldStyleName);
			if (newIndex >= 0)
				setCurrentIndex(newIndex);
		}
	}
	QListView *tmpView = dynamic_cast<QListView*>(view());
	if (tmpView)
	{
		int tmpWidth = tmpView->sizeHintForColumn(0);
		if (tmpWidth > 0)
			tmpView->setMinimumWidth(tmpWidth + 24);
	}
	this->blockSignals(sigBlocked);
}

void ParaStyleComboBox::selFormat(int e)
{
	if (e == 0)
		emit newStyle(QString());
	else
		emit newStyle(currentText());
}

CharStyleComboBox::CharStyleComboBox(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	addItem( CommonStrings::trDefaultCharacterStyle );
	currentDoc = nullptr;
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void CharStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	currentDoc = newCurrentDoc;
	updateFormatList();
}

void CharStyleComboBox::setFormat(const QString& name)
{
	setCurrentComboItem(this, name.isEmpty() ? CommonStrings::trDefaultCharacterStyle : name);
}

void CharStyleComboBox::updateFormatList()
{
	QString oldStyleName;
	if (currentDoc && this->count() > 0)
		oldStyleName = this->currentText();

	bool sigBlocked = this->blockSignals(true);
	clear();
	if (currentDoc != nullptr)
	{
		QStringList st;
		addItem( CommonStrings::trDefaultCharacterStyle );
		for (int x = 0; x < currentDoc->charStyles().count(); ++x)
		{
			if ( !currentDoc->charStyles()[x].name().isEmpty() &&
			    !currentDoc->charStyles()[x].isDefaultStyle())
				st.append(currentDoc->charStyles()[x].name());
		}
		st.sort();
		addItems(st);

		if (oldStyleName.length() > 0)
		{
			int newIndex = this->findText(oldStyleName);
			if (newIndex >= 0)
				setCurrentIndex(newIndex);
		}
	}
	QListView *tmpView = dynamic_cast<QListView*>(view());
	if (tmpView)
	{
		int tmpWidth = tmpView->sizeHintForColumn(0);
		if (tmpWidth > 0)
			tmpView->setMinimumWidth(tmpWidth + 24);
	}
	this->blockSignals(sigBlocked);
}

void CharStyleComboBox::selFormat(int e)
{
	if (e == 0)
		emit newStyle(QString());
	else
		emit newStyle(currentText());
}

CellStyleComboBox::CellStyleComboBox(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	addItem( CommonStrings::trDefaultCellStyle );
	currentDoc = nullptr;
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void CellStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	currentDoc = newCurrentDoc;
	updateFormatList();
}

void CellStyleComboBox::setFormat(const QString& name)
{
	setCurrentComboItem(this, name.isEmpty() ? CommonStrings::trDefaultCellStyle : name);
}

void CellStyleComboBox::updateFormatList()
{
	QString oldStyleName;
	if (currentDoc && this->count() > 0)
		oldStyleName = this->currentText();

	bool sigBlocked = this->blockSignals(true);
	clear();
	if (currentDoc != nullptr)
	{
		QStringList st;
		addItem( CommonStrings::trDefaultCellStyle  );
		for (int x = 0; x < currentDoc->cellStyles().count(); ++x)
		{
			if ( !currentDoc->cellStyles()[x].name().isEmpty() &&
			    !currentDoc->cellStyles()[x].isDefaultStyle())
				st.append(currentDoc->cellStyles()[x].name());
		}
		st.sort();
		addItems(st);

		if (oldStyleName.length() > 0)
		{
			int newIndex = this->findText(oldStyleName);
			if (newIndex >= 0)
				setCurrentIndex(newIndex);
		}
	}
	QListView *tmpView = dynamic_cast<QListView*>(view());
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	this->blockSignals(sigBlocked);
}

void CellStyleComboBox::selFormat(int e)
{
	if (e == 0)
	{
		emit newStyle(QString());
	}
	else
	{
		emit newStyle(currentText());
	}
}

TableStyleComboBox::TableStyleComboBox(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	addItem( CommonStrings::trDefaultTableStyle );
	currentDoc = nullptr;
	connect(this, SIGNAL(activated(int)), this, SLOT(selFormat(int)));
}

void TableStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	currentDoc = newCurrentDoc;
	updateFormatList();
}

void TableStyleComboBox::setFormat(const QString& name)
{
	setCurrentComboItem(this, name.isEmpty() ? CommonStrings::trDefaultTableStyle : name);
}

void TableStyleComboBox::updateFormatList()
{
	QString oldStyleName;
	if (currentDoc && this->count() > 0)
		oldStyleName = this->currentText();

	bool sigBlocked = this->blockSignals(true);
	clear();
	if (currentDoc != nullptr)
	{
		QStringList st;
		addItem( CommonStrings::trDefaultTableStyle );
		for (int x = 0; x < currentDoc->tableStyles().count(); ++x)
		{
			if ( !currentDoc->tableStyles()[x].name().isEmpty() &&
			    !currentDoc->tableStyles()[x].isDefaultStyle())
				st.append(currentDoc->tableStyles()[x].name());
		}
		st.sort();
		addItems(st);

		if (oldStyleName.length() > 0)
		{
			int newIndex = this->findText(oldStyleName);
			if (newIndex >= 0)
				setCurrentIndex(newIndex);
		}
	}
	QListView *tmpView = dynamic_cast<QListView*>(view());
	int tmpWidth = tmpView->sizeHintForColumn(0);
	if (tmpWidth > 0)
		tmpView->setMinimumWidth(tmpWidth + 24);
	this->blockSignals(sigBlocked);
}

void TableStyleComboBox::selFormat(int e)
{
	if (e == 0)
	{
		emit newStyle(QString());
	}
	else
	{
		emit newStyle(currentText());
	}
}
