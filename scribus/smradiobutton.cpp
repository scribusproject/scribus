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
		hasParent_ ( false ),
		useParentValue_ ( false ),
		pValue_ ( false )
{
}

void SMRadioButton::setChecked(bool val)
{
	hasParent_ = false;
	pValue_ = false;
	setFont(false);

	QRadioButton::setChecked(val);
}

void SMRadioButton::setChecked(bool val, bool isParentVal)
{
	hasParent_ = true;
	pValue_ = val;
	setFont(!isParentVal);

	QRadioButton::setChecked(val);
}

void SMRadioButton::setParentValue(bool val)
{
	hasParent_ = true;
	pValue_ = val;
}

bool SMRadioButton::useParentValue()
{
	bool ret = useParentValue_;
	useParentValue_ = false;
	return ret;
}

void SMRadioButton::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	QRadioButton::setFont(f);
}



