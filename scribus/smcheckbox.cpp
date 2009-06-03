/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smcheckbox.h"

SMCheckBox::SMCheckBox(QWidget *parent)
: QCheckBox(parent),
  hasParent_(false),
  useParentValue_(false),
  pValue_(false)
{

}

void SMCheckBox::setChecked(bool val)
{
	disconnect(this, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
	hasParent_ = false;
	pValue_ = false;
	setFont(false);

	QCheckBox::setChecked(val);
}

void SMCheckBox::setChecked(bool val, bool isParentVal)
{
	disconnect(this, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
	hasParent_ = true;
	pValue_ = val;
	setFont(!isParentVal);

	QCheckBox::setChecked(val);
	connect(this, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
}

void SMCheckBox::setParentValue(bool val)
{
	hasParent_ = true;
	pValue_ = val;
}

bool SMCheckBox::useParentValue()
{
	bool ret = useParentValue_;
	useParentValue_ = false;
	return ret;
}

void SMCheckBox::setFont(bool wantBold)
{
	QFont f(font());
	f.setBold(wantBold);
	QCheckBox::setFont(f);
}

void SMCheckBox::slotStateChanged(int)
{
	if(hasParent_)
		setFont(true);
}
