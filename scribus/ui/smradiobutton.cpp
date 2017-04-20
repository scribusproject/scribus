//
// C++ Implementation: smradiobutton
//
// Description: a clone of SMCheckBox
//
//
// Author: Pierre Marchand <pierremarc@oep-h.com>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "smradiobutton.h"

SMRadioButton::SMRadioButton ( QWidget * parent )
		: QRadioButton ( parent ),
		m_hasParent ( false ),
		m_useParentValue ( false ),
		m_pValue ( false )
{
}

void SMRadioButton::setChecked(bool val)
{
	m_hasParent = false;
	m_pValue = false;
	setFont(false);

	QRadioButton::setChecked(val);
}

void SMRadioButton::setChecked(bool val, bool isParentVal)
{
	m_hasParent = true;
	m_pValue = val;
	setFont(!isParentVal);

	QRadioButton::setChecked(val);
}

void SMRadioButton::setParentValue(bool val)
{
	m_hasParent = true;
	m_pValue = val;
}

bool SMRadioButton::useParentValue()
{
	bool ret = m_useParentValue;
	m_useParentValue = false;
	return ret;
}

void SMRadioButton::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	QRadioButton::setFont(f);
}



