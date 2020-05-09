/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smnumformatcombo.h"

SMNumFormatCombo::SMNumFormatCombo(QWidget *parent)
	: NumFormatCombo(parent)
{
	
}

void SMNumFormatCombo::setCurrentFormat(NumFormat format)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	setFont(false);
	m_hasParent = false;
	m_parentFormat = Type_1_2_3;
	
	NumFormatCombo::setCurrentFormat(format);
}

void SMNumFormatCombo::setCurrentFormat(NumFormat format, bool isParentValue)
{
	disconnect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
	m_hasParent = true;
	m_parentFormat = format;
	setFont(!isParentValue);
	if (!isParentValue && !m_useParentValue)
	{
		m_useParentValue = true;
		addItem( tr("Use Parent Value"));
	}

	NumFormatCombo::setCurrentFormat(format);
	connect(this, SIGNAL(highlighted(int)), this, SLOT(currentChanged()));
}

void SMNumFormatCombo::setParentFormat(NumFormat format)
{
	m_hasParent = true;
	m_parentFormat = format;
}

bool SMNumFormatCombo::useParentFormat()
{
	bool ret = false;

	if (m_useParentValue && m_hasParent)
	{
		ret = currentIndex() == (count() - 1);
		if (ret)
		{
			removeItem(count() - 1);
			setFont(false);
			setCurrentFormat(m_parentFormat, true);
			m_useParentValue = false;
		}
	}

	return ret;
}

void SMNumFormatCombo::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	NumFormatCombo::setFont(f);
}

void SMNumFormatCombo::currentChanged()
{
	if (m_hasParent && !m_useParentValue)
	{
		setFont(true);
		addItem( tr("Use Parent Value"));
		m_useParentValue = true;
	}
}
