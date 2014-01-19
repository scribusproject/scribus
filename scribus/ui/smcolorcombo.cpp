/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smcolorcombo.h"
#include "util.h"


SMColorCombo::SMColorCombo(QWidget *parent)
: ColorCombo(parent),
  m_hasParent(false),
  m_useParentValue(false),
  m_pItem(0)
{
	
}

SMColorCombo::SMColorCombo(bool rw, QWidget* parent)
: ColorCombo(rw, parent),
  m_hasParent(false),
  m_useParentValue(false),
  m_pItem(0),
  m_pText(QString::null)
{
	
}

void SMColorCombo::setCurrentItem(int i)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	m_hasParent = false;
	m_pItem = 0;
	m_pText = QString::null;
	ColorCombo::setCurrentIndex(i);
}

void SMColorCombo::setCurrentItem(int i, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	m_hasParent = true;
	m_pItem = i;
	m_pText = QString::null;
	ColorCombo::setCurrentIndex(i);
	setFont(!isParentValue);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMColorCombo::setCurrentText(const QString &s)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	m_hasParent = false;
	m_pItem = -1;
	m_pText = s;
	setCurrentComboItem(this, s);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMColorCombo::setCurrentText(const QString &s, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	m_hasParent = true;
	m_pItem = -1;
	m_pText = s;
	setCurrentComboItem(this, s);
	setFont(!isParentValue);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMColorCombo::setParentItem(int i)
{
	m_hasParent = true;
	m_pItem = i;
	m_pText = QString::null;
}

void SMColorCombo::setParentText(const QString &s)
{
	m_hasParent = true;
	m_pText = s;
}

bool SMColorCombo::useParentValue()
{
	bool ret = false;

	if (m_useParentValue && m_hasParent)
	{
		ret = currentIndex() == (count() - 1);
		if (ret)
		{
			removeItem(count() - 1);
			setFont(false);
			if (!m_pText.isNull())
				setCurrentText(m_pText, true);
			else
				setCurrentItem(m_pItem, true);
			m_useParentValue = false;
		}
	}

	return ret;
}

void SMColorCombo::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	ColorCombo::setFont(f);
}

void SMColorCombo::currentChanged()
{
	if (m_hasParent && !m_useParentValue)
	{
		setFont(true);
		addItem( tr("Use Parent Value"));
		m_useParentValue = true;
	}
}
