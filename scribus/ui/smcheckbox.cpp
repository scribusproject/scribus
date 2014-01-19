/*
For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
*/

#include "smcheckbox.h"

SMCheckBox::SMCheckBox(QWidget *parent)
: QCheckBox(parent),
  m_hasParent(false),
  m_useParentValue(false),
  m_pValue(false)
{

}

void SMCheckBox::setChecked(bool val)
{
	disconnect(this, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
	m_hasParent = false;
	m_pValue = false;
	setFont(false);

	QCheckBox::setChecked(val);
}

void SMCheckBox::setChecked(bool val, bool isParentVal)
{
	disconnect(this, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
	m_hasParent = true;
	m_pValue = val;
	setFont(!isParentVal);

	QCheckBox::setChecked(val);
	connect(this, SIGNAL(stateChanged(int)), this, SLOT(slotStateChanged(int)));
}

void SMCheckBox::setParentValue(bool val)
{
	m_hasParent = true;
	m_pValue = val;
}

bool SMCheckBox::useParentValue()
{
	bool ret = m_useParentValue;
	m_useParentValue = false;
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
	if(m_hasParent)
		setFont(true);
}
