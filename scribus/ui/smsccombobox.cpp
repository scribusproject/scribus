/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smsccombobox.h"


SMScComboBox::SMScComboBox(QWidget *parent)
: ScComboBox(parent),
  m_hasParent(false),
  m_useParentValue(false),
  m_pItem(0)
{
	
}

void SMScComboBox::setCurrentItem(int i)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	m_hasParent = false;
	m_pItem = 0;
	ScComboBox::setCurrentIndex(i);
}

void SMScComboBox::setCurrentItem(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	m_hasParent = true;
	m_pItem = i;
	setFont(!isParentValue);
	if (!isParentValue && !m_useParentValue)
	{
		m_useParentValue = true;
		addItem( tr("Use Parent Value"));
	}

	ScComboBox::setCurrentIndex(i);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMScComboBox::setCurrentItemByData(int i)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	m_hasParent = false;
	m_pItem = 0;
	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toInt() == i)
			ScComboBox::setCurrentIndex(idx);
	}
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMScComboBox::setCurrentItemByData(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	m_hasParent = true;
	setFont(!isParentValue);
	if (!isParentValue && !m_useParentValue)
	{
		m_useParentValue = true;
		addItem( tr("Use Parent Value"));
	}

	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toInt() == i)
		{
			ScComboBox::setCurrentIndex(idx);
			m_pItem = idx;
		}
	}
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMScComboBox::setCurrentItemByData(double d)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	m_hasParent = false;
	m_pItem = 0;
	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toDouble() == d)
			ScComboBox::setCurrentIndex(idx);
	}
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMScComboBox::setCurrentItemByData(double d, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	m_hasParent = true;
	setFont(!isParentValue);
	if (!isParentValue && !m_useParentValue)
	{
		m_useParentValue = true;
		addItem( tr("Use Parent Value"));
	}

	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toDouble() == d)
		{
			ScComboBox::setCurrentIndex(idx);
			m_pItem = idx;
		}
	}
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

int SMScComboBox::getItemIndexForData(int i)
{
	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toInt() == i)
		{
			return idx;
		}
	}
	return 0;
}

int SMScComboBox::getItemIndexForData(double d)
{
	for(int idx(0); idx < count(); ++idx)
	{
		if(itemData(idx).toDouble() == d)
		{
			return idx;
		}
	}
	return 0;
}



void SMScComboBox::setParentItem(int i)
{
	m_hasParent = true;
	m_pItem = i;
}

bool SMScComboBox::useParentValue()
{
	bool ret = false;

	if (m_useParentValue && m_hasParent)
	{
		ret = currentIndex() == (count() - 1);
		if (ret)
		{
			removeItem(count() - 1);
			setFont(false);
			setCurrentItem(m_pItem, true);
			m_useParentValue = false;
		}
	}

	return ret;
}

void SMScComboBox::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	ScComboBox::setFont(f);
}

void SMScComboBox::currentChanged()
{
	if (m_hasParent && !m_useParentValue)
	{
		setFont(true);
		addItem( tr("Use Parent Value"));
		m_useParentValue = true;
	}
}
