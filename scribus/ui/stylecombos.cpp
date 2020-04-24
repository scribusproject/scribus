/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/
/***************************************************************************
                          stylecombos.cpp  -  description
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
#include "stylecombos.h"

#include <QListView>

#include "scpage.h"
#include "util.h"

ParaStyleComboBox::ParaStyleComboBox(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	addItem( tr("[Inherited Style]") );
	addItem( CommonStrings::trDefaultParagraphStyle );
	connect(this, SIGNAL(activated(int)), this, SLOT(selectedStyle(int)));
}

QString ParaStyleComboBox::currentStyle() const
{
	if (this->currentIndex() == 0)
		return QString();

	QString currStyleName = this->currentText();
	if (currStyleName == CommonStrings::trDefaultParagraphStyle)
		currStyleName = CommonStrings::DefaultParagraphStyle;
	return currStyleName;
}

void ParaStyleComboBox::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QComboBox::changeEvent(e);
}

void ParaStyleComboBox::languageChange()
{
	bool sigBlocked = this->blockSignals(true);
	int styleIndex = this->currentIndex();

	updateStyleList();
	if (styleIndex >= 0)
		this->setCurrentIndex(styleIndex);

	this->blockSignals(sigBlocked);
}

void ParaStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	m_doc = newCurrentDoc;
	updateStyleList();
}

void ParaStyleComboBox::setStyle(const QString& name)
{
	QString parStyleName = name;
	if (parStyleName == CommonStrings::DefaultParagraphStyle)
		parStyleName = CommonStrings::trDefaultParagraphStyle;
	setCurrentComboItem(this, parStyleName.isEmpty() ? tr("[Inherited Style]") : parStyleName);
}

void ParaStyleComboBox::updateStyleList()
{
	QString oldStyleName;
	if (m_doc && this->count() > 0)
		oldStyleName = this->currentText();

	bool sigBlocked = this->blockSignals(true);
	clear();
	if (m_doc != nullptr)
	{
		QStringList st;
		addItem( tr("[Inherited Style]") );
		addItem( CommonStrings::trDefaultParagraphStyle );
		for (int i = 0; i < m_doc->paragraphStyles().count(); ++i)
		{
			const ParagraphStyle& paraStyle = m_doc->paragraphStyles()[i];
			if (!paraStyle.name().isEmpty() && !paraStyle.isDefaultStyle())
				st.append(paraStyle.name());
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

void ParaStyleComboBox::selectedStyle(int index)
{
	if (index == 0)
	{
		emit newStyle(QString());
		return;
	}
	
	QString newStyleName = currentText();
	if (newStyleName == CommonStrings::trDefaultParagraphStyle)
		newStyleName = CommonStrings::DefaultParagraphStyle;
	emit newStyle(newStyleName);
}

CharStyleComboBox::CharStyleComboBox(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	addItem( tr("[Inherited Style]") );
	addItem( CommonStrings::trDefaultCharacterStyle );
	connect(this, SIGNAL(activated(int)), this, SLOT(selectedStyle(int)));
}

QString CharStyleComboBox::currentStyle() const
{
	if (this->currentIndex() == 0)
		return QString();

	QString currStyleName = this->currentText();
	if (currStyleName == CommonStrings::trDefaultCharacterStyle)
		currStyleName = CommonStrings::DefaultCharacterStyle;
	return currStyleName;
}

void CharStyleComboBox::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QComboBox::changeEvent(e);
}

void CharStyleComboBox::languageChange()
{
	bool sigBlocked = this->blockSignals(true);
	int styleIndex = this->currentIndex();

	updateStyleList();
	if (styleIndex >= 0)
		this->setCurrentIndex(styleIndex);

	this->blockSignals(sigBlocked);
}

void CharStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	m_doc = newCurrentDoc;
	updateStyleList();
}

void CharStyleComboBox::setStyle(const QString& name)
{
	QString charStyleName = name;
	if (charStyleName == CommonStrings::DefaultCharacterStyle)
		charStyleName = CommonStrings::trDefaultCharacterStyle;
	setCurrentComboItem(this, charStyleName.isEmpty() ? tr("[Inherited Style]") : charStyleName);
}

void CharStyleComboBox::updateStyleList()
{
	QString oldStyleName;
	if (m_doc && this->count() > 0)
		oldStyleName = this->currentText();

	bool sigBlocked = this->blockSignals(true);
	clear();
	if (m_doc != nullptr)
	{
		QStringList st;
		addItem( tr("[Inherited Style]") );
		addItem( CommonStrings::trDefaultCharacterStyle );
		for (int i = 0; i < m_doc->charStyles().count(); ++i)
		{
			const CharStyle& charStyle = m_doc->charStyles()[i];
			if (!charStyle.name().isEmpty() && !charStyle.isDefaultStyle())
				st.append(charStyle.name());
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

void CharStyleComboBox::selectedStyle(int index)
{
	if (index == 0)
	{
		emit newStyle(QString());
		return;
	}
	
	QString newStyleName = currentText();
	if (newStyleName == CommonStrings::trDefaultCharacterStyle)
		newStyleName = CommonStrings::DefaultCharacterStyle;
	emit newStyle(newStyleName);
}

CellStyleComboBox::CellStyleComboBox(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	addItem( tr("[Inherited Style]") );
	addItem( CommonStrings::trDefaultCellStyle );
	connect(this, SIGNAL(activated(int)), this, SLOT(selectedStyle(int)));
}

QString CellStyleComboBox::currentStyle() const
{
	if (this->currentIndex() == 0)
		return QString();

	QString currStyleName = this->currentText();
	if (currStyleName == CommonStrings::trDefaultCellStyle)
		currStyleName = CommonStrings::DefaultCellStyle;
	return currStyleName;
}

void CellStyleComboBox::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QComboBox::changeEvent(e);
}

void CellStyleComboBox::languageChange()
{
	bool sigBlocked = this->blockSignals(true);
	int styleIndex = this->currentIndex();

	updateStyleList();
	if (styleIndex >= 0)
		this->setCurrentIndex(styleIndex);

	this->blockSignals(sigBlocked);
}

void CellStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	m_doc = newCurrentDoc;
	updateStyleList();
}

void CellStyleComboBox::setStyle(const QString& name)
{
	QString cellStyleName = name;
	if (cellStyleName == CommonStrings::DefaultCellStyle)
		cellStyleName = CommonStrings::trDefaultCellStyle;
	setCurrentComboItem(this, cellStyleName.isEmpty() ? tr("[Inherited Style]") : cellStyleName);
}

void CellStyleComboBox::updateStyleList()
{
	QString oldStyleName;
	if (m_doc && this->count() > 0)
		oldStyleName = this->currentText();

	bool sigBlocked = this->blockSignals(true);
	clear();
	if (m_doc != nullptr)
	{
		QStringList st;
		addItem( tr("[Inherited Style]") );
		addItem( CommonStrings::trDefaultCellStyle  );
		for (int i = 0; i < m_doc->cellStyles().count(); ++i)
		{
			const CellStyle& cellStyle = m_doc->cellStyles()[i];
			if (!cellStyle.name().isEmpty() && !cellStyle.isDefaultStyle())
				st.append(cellStyle.name());
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

void CellStyleComboBox::selectedStyle(int index)
{
	if (index == 0)
	{
		emit newStyle(QString());
		return;
	}
	
	QString newStyleName = currentText();
	if (newStyleName == CommonStrings::trDefaultCellStyle)
		newStyleName = CommonStrings::DefaultCellStyle;
	emit newStyle(newStyleName);
}

TableStyleComboBox::TableStyleComboBox(QWidget* parent) : QComboBox(parent)
{
	setEditable(false);
	addItem( tr("[Inherited Style]") );
	addItem( CommonStrings::trDefaultTableStyle );
	connect(this, SIGNAL(activated(int)), this, SLOT(selectedStyle(int)));
}

QString TableStyleComboBox::currentStyle() const
{
	if (this->currentIndex() == 0)
		return QString();

	QString currStyleName = this->currentText();
	if (currStyleName == CommonStrings::trDefaultTableStyle)
		currStyleName = CommonStrings::DefaultTableStyle;
	return currStyleName;
}

void TableStyleComboBox::changeEvent(QEvent *e)
{
	if (e->type() == QEvent::LanguageChange)
	{
		languageChange();
		return;
	}
	QComboBox::changeEvent(e);
}

void TableStyleComboBox::languageChange()
{
	bool sigBlocked = this->blockSignals(true);
	int styleIndex = this->currentIndex();

	updateStyleList();
	if (styleIndex >= 0)
		this->setCurrentIndex(styleIndex);

	this->blockSignals(sigBlocked);
}

void TableStyleComboBox::setDoc(ScribusDoc *newCurrentDoc)
{
	m_doc = newCurrentDoc;
	updateStyleList();
}

void TableStyleComboBox::setStyle(const QString& name)
{
	QString tableStyleName = name;
	if (tableStyleName == CommonStrings::DefaultTableStyle)
		tableStyleName = CommonStrings::trDefaultTableStyle;
	setCurrentComboItem(this, tableStyleName.isEmpty() ? tr("[Inherited Style]") : tableStyleName);
}

void TableStyleComboBox::updateStyleList()
{
	QString oldStyleName;
	if (m_doc && this->count() > 0)
		oldStyleName = this->currentText();

	bool sigBlocked = this->blockSignals(true);
	clear();
	if (m_doc != nullptr)
	{
		QStringList st;
		addItem( tr("[Inherited Style]") );
		addItem( CommonStrings::trDefaultTableStyle );
		for (int i = 0; i < m_doc->tableStyles().count(); ++i)
		{
			const TableStyle& tableStyle = m_doc->tableStyles()[i];
			if (!tableStyle.name().isEmpty() && !tableStyle.isDefaultStyle())
				st.append(tableStyle.name());
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

void TableStyleComboBox::selectedStyle(int index)
{
	if (index == 0)
	{
		emit newStyle(QString());
		return;
	}
	
	QString newStyleName = currentText();
	if (newStyleName == CommonStrings::trDefaultTableStyle)
		newStyleName = CommonStrings::DefaultTableStyle;
	emit newStyle(newStyleName);
}
